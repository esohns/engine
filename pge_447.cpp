#include "stdafx.h"

#include "pge_447.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

#include "engine_common.h"

#include "defines_20.h"

PGE_447::PGE_447 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = "Example 447";
}

bool
PGE_447::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

#if defined (ACE_WIN32) || defined (ACE_WIN64)
  tp1_ = std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  tp1_ = std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  return true;
}

bool
PGE_447::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_447_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_447_DEFAULT_ALPHA_DECAY : 0);

#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 =
    std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 =
    std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - tp1_;
  std::chrono::milliseconds d =
    std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);
	float t = d.count () / 3000.0f;

  float r, g, b;
  Common_Image_Tools::HSVToRGB (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (100.0, t, 0.0)), -1.0f, 1.0f, 0.0f, 360.0f),
                                0.3f,
                                1.0f,
                                r, g, b);
  olc::Pixel color = olc::PixelF (r, g, b);

  static int32_t screen_width_i = olc::PixelGameEngine::ScreenWidth ();
  static int32_t screen_height_i = olc::PixelGameEngine::ScreenHeight ();
  static olc::vf2d center_s (screen_width_i / 2.0f, screen_height_i / 2.0f);
  static float radius_f = std::min (screen_width_i, screen_height_i) * 0.4f;
  std::vector<olc::vf2d> poly = makePolygon (center_s, radius_f, 6);
	for (int i = 0; i < 6; i++)
  {
		float base = Common_GL_Tools::map (static_cast<float> (noise_.GetValue (i * 2.0, t, 0.0)), -1.0f, 1.0f, 0.2f, 0.9f);
		float hgt = Common_GL_Tools::map (static_cast<float> (noise_.GetValue (i * 2.0 + 1.0, t, 0.0)), -1.0f, 1.0f, 0.2f, 0.8f);
    auto insertSpike = [base, hgt](olc::vf2d& a, olc::vf2d& b)
    {
      olc::vf2d v = b - a;
      float vmag = v.mag ();
      olc::vf2d vnorm (-v.y, v.x);
      vnorm *= hgt;
      float k1 = 0.5f - base / 2.0f;
      olc::vf2d p1 = a.lerp (b, k1);
      float k2 = 0.5f + base / 2.0f;
      olc::vf2d p2 = a.lerp (b, k2);
      olc::vf2d mid = a.lerp (b, 0.5f);

      std::vector<olc::vf2d> result = {a, p1, mid + vnorm, p2, b};
      return result;
    };
    auto insertSlab = [base, hgt](olc::vf2d& a, olc::vf2d& b)
    {
      olc::vf2d v = b - a;
      float vmag = v.mag ();
      olc::vf2d vnorm (-v.y, v.x);
      vnorm *= hgt;
      float k1 = 0.5f - base / 2.0f;
      olc::vf2d p1 = a.lerp (b, k1);
      float k2 = 0.5f + base / 2.0f;
      olc::vf2d p2 = a.lerp (b, k2);
      //olc::vf2d mid = a.lerp (b, 0.5f);

      std::vector<olc::vf2d> result = {a, p1, p1 + vnorm, p2 + vnorm, p2, b};
      return result;
    };
    
    poly = applyToEveryEdge (poly, insertSpike);
	} // end IF

  for (std::vector<olc::vf2d>::iterator iterator = poly.begin();
       iterator != poly.end ();
       ++iterator)
    olc::PixelGameEngine::Draw (*iterator, color);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<olc::vf2d>
PGE_447::makePolygon (olc::vf2d& center_in, float radius_in, int sides_in)
{
  std::vector<olc::vf2d> result;

  olc::vf2d v (0.0f, -radius_in);
  float dang = 2.0f * static_cast<float> (M_PI) / sides_in;
  for (int i = 0; i < sides_in; i++)
  {
    result.push_back (center_in + v);

    float x_rot = v.x * std::cos (dang) - v.y * std::sin (dang);
    float y_rot = v.x * std::sin (dang) + v.y * std::cos (dang);
    v = {x_rot, y_rot};
  } // end FOR

  return result;
}

std::vector<olc::vf2d>
PGE_447::applyToEveryEdge (std::vector<olc::vf2d>& pts, std::function<std::vector<olc::vf2d> (olc::vf2d&, olc::vf2d&)> f)
{
  std::vector<olc::vf2d> result_a;

  int n = static_cast<int> (pts.size ());
  for (int i = 0; i < n; i++)
  {
    std::vector<olc::vf2d> sub_result_a = f (pts[i], pts[(i + 1) % n]);
    result_a.insert (result_a.end (), sub_result_a.begin (), sub_result_a.end ());
  } // end FOR

  return result_a;
}
