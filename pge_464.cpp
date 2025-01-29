#include "stdafx.h"

#include "pge_464.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "engine_common.h"

#include "defines_21.h"

PGE_464::PGE_464 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , tp1_ ()
 , D_ (0.0f)
 , hairs_ ()
{
  sAppName = "Example 464";
}

bool
PGE_464::OnUserCreate ()
{
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  tp1_ = std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  tp1_ = std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  D_ =
    static_cast<float> (std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()));
  initializeHairs ();

  return true;
}

bool
PGE_464::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  static olc::vf2d cntr (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f);
  olc::vf2d p;
  olc::vf2d v (0.0f, 0.0f);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    p = { static_cast<float> (olc::PixelGameEngine::GetMouseX ()), static_cast<float> (olc::PixelGameEngine::GetMouseY ()) };
  else
  {
#if defined (ACE_WIN32) || defined (ACE_WIN64)
    std::chrono::steady_clock::time_point tp2 = std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 = std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
    std::chrono::duration<float> elapsed_time = tp2 - tp1_;
    std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);
  	float t = d.count () / 5000.0f;

    float n =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (t, 1.0f, 0.0f)), -1.0f, 1.0f, 0.0f, 1.0f);
    float r = Common_Math_Tools::lerp (D_ / 6.0f, D_ / 2.0f, n);
    n =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (t, 2.0f, 0.0f)), -1.0f, 1.0f, 0.0f, 1.0f);
    float a = Common_Math_Tools::lerp (-2.0f * static_cast<float> (M_PI), 2.0f * static_cast<float> (M_PI), n);
    p = cntr;
    olc::vf2d v_2 (0.0f, -r);
    float x_rotated_f = v_2.x * std::cos (a) - v_2.y * std::sin (a);
    float y_rotated_f = v_2.x * std::sin (a) + v_2.y * std::cos (a);
    v_2 = { x_rotated_f , y_rotated_f };
    p += v_2;
  } // end ELSE

  if (p.dist (cntr) > D_ / 5.0f)
  {
    v = cntr - p;
    float s = Common_GL_Tools::map (v.mag (), 0.0f, D_ / 2.0f, 4.0f, 0.2f) * ENGINE_PGE_464_DEFAULT_G;
    v = v.norm (); v *= s * 30.0f;
    drawArrow (p, v, 20);
  } // end IF
  moveHairs (v); moveHairs (v); //moveHairs (v); moveHairs (v);

  for (std::vector<struct hair>::iterator iterator = hairs_.begin ();
       iterator != hairs_.end ();
       ++iterator)
    (*iterator).spine.show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_464::initializeHairs ()
{
  hairs_.clear ();

  float r = D_ * 0.1f;
  float hairLen = D_ * 0.5f;
  float sep = hairLen / static_cast<float> (ENGINE_PGE_464_DEFAULT_NUMBER_OF_SEGMENTS);
  for (int i = 0; i < ENGINE_PGE_464_DEFAULT_NUMBER_OF_HAIRS; i++)
  {
    spine spine_s (ENGINE_PGE_464_DEFAULT_NUMBER_OF_SEGMENTS, sep, static_cast<float> (M_PI) / 6.0f);

    float angle_f =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_464_DEFAULT_NUMBER_OF_HAIRS - 1), 0.0f, static_cast<float> (-M_PI));
    olc::vf2d v (r, 0.0f);
    float x_rotated_f = v.x * std::cos (angle_f) - v.y * std::sin (angle_f);
    float y_rotated_f = v.x * std::sin (angle_f) + v.y * std::cos (angle_f);
    v = { x_rotated_f , y_rotated_f };
    static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f);
    olc::vf2d p = center_s + v;
    v = v.norm (); v *= sep;
    olc::vf2d q = p + v;
    olc::vf2d node = p;
    for (int j = 0; j < ENGINE_PGE_464_DEFAULT_NUMBER_OF_SEGMENTS; j++)
    {
      spine_s.nodes.push_back (node);
      node += v;
    } // end FOR

    struct hair hair_s;
    hair_s.p = p; hair_s.q = q; hair_s.spine = spine_s;
    hairs_.push_back (hair_s);
  } // end FOR
}

void
PGE_464::moveHairs (olc::vf2d& v)
{
  for (std::vector<struct hair>::iterator iterator = hairs_.begin ();
       iterator != hairs_.end ();
       ++iterator)
  {
    for (int i = ENGINE_PGE_464_DEFAULT_NUMBER_OF_SEGMENTS - 1; i > 1; i--)
    {
      olc::vf2d& node = (*iterator).spine.nodes[i];
      if (v.mag ())
        node += v;
      node.y += ENGINE_PGE_464_DEFAULT_G;
      (*iterator).spine.setNode (i, node);
    } // end FOR

    (*iterator).spine.setNode (0, (*iterator).p);
    (*iterator).spine.setNode (1, (*iterator).q);
  } // end FOR
}

void
PGE_464::drawArrow (olc::vf2d& p, olc::vf2d& v, float sz)
{
  olc::vf2d q = p + v;
  olc::vf2d u = v;
  u = u.norm (); u *= sz;
  static float a = 120.0f * (static_cast<float> (M_PI) / 180.0f);
  static float b = 150.0f * (static_cast<float> (M_PI) / 180.0f);

  olc::PixelGameEngine::DrawLine (p, q, olc::BLACK, 0xFFFFFFFF);
  for (int i = 0; i < 3; i++)
  {
    float ang = i == 0 ? b : a;
    float x_rotated_f = u.x * std::cos (ang) - u.y * std::sin (ang);
    float y_rotated_f = u.x * std::sin (ang) + u.y * std::cos (ang);
    u = { x_rotated_f, y_rotated_f };
    olc::PixelGameEngine::DrawLine (q, q + u, olc::BLACK, 0xFFFFFFFF);
    q += u;
  } // end FOR
}
