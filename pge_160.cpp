#include "stdafx.h"

#include "pge_160.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_160::PGE_160 ()
 : olc::PixelGameEngine ()
 , rows_ (0)
{
  sAppName = "Example 160";
}

bool
PGE_160::OnUserCreate ()
{
  return true;
}

bool
PGE_160::OnUserUpdate (float fElapsedTime)
{
  if (rows_ < olc::PixelGameEngine::ScreenHeight ())
  {
    rows_++;
    for (int32_t cols = olc::PixelGameEngine::ScreenWidth (); cols--;)
    {
      float x =
        Common_GL_Tools::map (static_cast<float> (cols), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), -0.1f, -0.03f);
      float y =
        Common_GL_Tools::map (static_cast<float> (rows_), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), -0.036f, 0.015f);
      uint8_t colors[5];
      for (int i = 0; i < 5; i++)
      {
        float d = x * x + y * y;
        float newx = std::sin (y / d + x / std::sqrt (5.0f));
        float newy = std::sin (x / d + y / std::sqrt (5.0f));
        x = newx;
        y = newy;
        float rgb = std::abs (280.0f - (x + y) * (x + y) * 128.0f);
        colors[i] = static_cast<uint8_t> (rgb > 255.0f ? 255.0f : rgb < 0.0f ? 0.0f : rgb);
      } // end IF
      olc::Pixel color (colors[2], colors[3], colors[4], 255);
      olc::PixelGameEngine::DrawLine (cols, rows_, cols, rows_ + colors[3], color, 0xFFFFFFFF);
    } // end FOR
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
