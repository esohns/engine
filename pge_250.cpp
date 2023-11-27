#include "stdafx.h"

#include "pge_250.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_250::PGE_250 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 250");
}

bool
PGE_250::OnUserCreate ()
{
  return true;
}

bool
PGE_250::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  static float pi_f = (1.0f + std::sqrt (5.0f)) / 2.0f;

  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float time_f = frame_count_i / 100.0f;
  olc::vf2d mouse_s (Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), 0.0f, 50.0f),
                     Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseY ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), 0.0f, 20.0f));
  int count_i = static_cast<int> (std::abs (std::cos (time_f / 8.0f) * 1000.0f));
  for (int i = 0; i <= count_i; i++)
  {
    int32_t x =
      static_cast<int32_t> (std::cos (i * pi_f * 2.0f * static_cast<float> (M_PI)) * (i / static_cast<float> (count_i)) * olc::PixelGameEngine::ScreenWidth ());
    int32_t y =
      static_cast<int32_t> (std::sin (i * pi_f * 2.0f * static_cast<float> (M_PI)) * (i / static_cast<float> (count_i)) * olc::PixelGameEngine::ScreenHeight ());
    int32_t r =
      static_cast<int32_t> (std::abs (std::cos (i * mouse_s.y / static_cast<float> (count_i) + time_f * 2.0f) * mouse_s.x) / 2.0f);
    olc::PixelGameEngine::FillCircle (half_width_i + x, half_height_i + y, r, olc::WHITE);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
