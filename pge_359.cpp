#include "stdafx.h"

#include "pge_359.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_16.h"

PGE_359::PGE_359 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 359");
}

bool
PGE_359::OnUserCreate ()
{
  return true;
}

bool
PGE_359::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_359_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_359_DEFAULT_ALPHA_DECAY : 0);

  float Am =
    Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), 0.0f, 2.0f * static_cast<float> (M_PI));
  bool is_first_b = true;
  static olc::vf2d prev_s;
  olc::vf2d pos_s;
  for (float L = 0.0f; L <= 1.0f; L += 0.01f)
  {
    float a = 6.0f * L;
    float A = 0.1f / L * std::sin (2.0f * static_cast<float> (M_PI) * L - Am);
    for (float t = 0; t < 2.0f * static_cast<float> (M_PI); t += (2.0f * static_cast<float> (M_PI)) / 500.0f)
    {
      float x = X1 (a, t) * std::cos (A) + Y1 (a, t) * std::sin (A);
      float y = Y1 (a, t) * std::cos (A) - X1 (a, t) * std::sin (A);
      pos_s.x = Common_GL_Tools::map (x, -8.0f, 8.0f, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
      pos_s.y = Common_GL_Tools::map (y, -8.0f, 8.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), 0.0f);
      if (is_first_b)
      {
        is_first_b = false;
        prev_s = pos_s;
      } // end IF
      olc::PixelGameEngine::DrawLine (prev_s, pos_s, olc::RED, 0xFFFFFFFF);

      prev_s = pos_s;
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_359::X1 (float a, float t)
{
  return a * std::sin (t) * std::sin (t) * std::sin (t);
}

float
PGE_359::Y1 (float a, float t)
{
  return 13.0f * a / 16.0f * std::cos (t) - 5.0f * a / 16.0f * std::cos (2.0f * t) - 2.0f * a / 16.0f * std::cos (3.0f * t) - 1.0f * a / 16.0f * std::cos (4.0f * t);
}
