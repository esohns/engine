#include "stdafx.h"

#include "pge_143.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_143::PGE_143 ()
 : olc::PixelGameEngine ()
 , timer_ (0.0f)
{
  sAppName = "Example 143";
}

bool
PGE_143::OnUserCreate ()
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_143::OnUserUpdate (float fElapsedTime)
{
  timer_ += 0.1f;

  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  int index = 0;
  for (int y = ENGINE_PGE_143_DEFAULT_W; y--;)
    for (int x = ENGINE_PGE_143_DEFAULT_W; x--;)
    {
      float a =
        std::sin (static_cast<float> (std::sqrt ((x - half_width_i) * (x - half_width_i) + (y - half_height_i) * (y - half_height_i)) / 64.0f)) * static_cast<float> (M_PI);
      float b =
        static_cast<float> (std::pow (static_cast<float> (std::sin (3.0f * std::atan2 ((ENGINE_PGE_143_DEFAULT_W - y) - olc::PixelGameEngine::GetMouseY (),
                                                                                       (ENGINE_PGE_143_DEFAULT_W - x) - olc::PixelGameEngine::GetMouseX ()))), 2));
      p[index].a = static_cast<uint8_t> (std::abs (std::sin (a * a + b * b) + std::sin (a * b + timer_)) * 128.0f);
      index++;
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
