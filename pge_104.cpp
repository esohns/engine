#include "stdafx.h"

#include "pge_104.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_104::PGE_104 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , m_ (0)
 , n_ (0)
{
  sAppName = "Example 104";
}

bool
PGE_104::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_104::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  float t =
    (frame_count_i % ENGINE_PGE_104_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_104_DEFAULT_NUMBER_OF_FRAMES);
  t = easeInOutSine (t);

  if ((frame_count_i % ENGINE_PGE_104_DEFAULT_NUMBER_OF_FRAMES) == 1)
  {
    m_ = Common_Tools::getRandomNumber (2, 6);
    n_ = Common_Tools::getRandomNumber (2, 6);
  } // end IF

  int32_t w = olc::PixelGameEngine::ScreenWidth () / m_;
  int32_t h = olc::PixelGameEngine::ScreenHeight () / n_;

  olc::Pixel fillCol, backCol;
  Common_GL_Color_t color1, color2, lerped_color;
  float theta, no, z;
  for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth (); x += ENGINE_PGE_104_DEFAULT_S)
    for (int32_t y = 0; y < olc::PixelGameEngine::ScreenHeight (); y += ENGINE_PGE_104_DEFAULT_S)
    {
      theta = 2.0f * static_cast<float> (M_PI) * t;
      if (y % (2 * h) < h) theta *= -1.0f;
      if (x % (2 * w) < w) theta *= -1.0f;
      no = loopNoise (x, y, theta, &noise_);
      if (theta < 0)
      {
        fillCol = {0x05, 0x05, 0x05, 255};
        color1.r = 0xFF; color1.g = 0xFB; color1.b = 0xE6;
        color2.r = 0xF9; color2.g = 0xD5; color2.b = 0x31;
        lerped_color =
          Common_GL_Tools::lerpRGB (color1, color2, 1.0f - static_cast<float> (std::pow (2.0f * t - 1.0f, 4)));
        backCol.r = lerped_color.r; backCol.g = lerped_color.g; backCol.b = lerped_color.b;
      } // end IF
      else
      {
        color1.r = 0x05; color1.g = 0x05; color1.b = 0x05;
        color2.r = 0x2B; color2.g = 0x67; color2.b = 0xAF;
        lerped_color =
          Common_GL_Tools::lerpRGB (color1, color2, 1.0f - static_cast<float> (std::pow (2.0f * t - 1.0f, 4)));
        fillCol.r = lerped_color.r; fillCol.g = lerped_color.g; fillCol.b = lerped_color.b;
        backCol = {0xff, 0xfb, 0xe6, 255};
      } // end ELSE
      if (static_cast<int32_t> (std::floor (no * ENGINE_PGE_104_DEFAULT_DENSITY)) % 2 == 0)
      {
        olc::Pixel temp = fillCol;
        fillCol = backCol;
        backCol = temp;
      } // end IF
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y), ENGINE_PGE_104_DEFAULT_S, ENGINE_PGE_104_DEFAULT_S, backCol);
      z =
        static_cast<float> (std::pow (2.0f * (static_cast<int32_t> (no * ENGINE_PGE_104_DEFAULT_DENSITY) % 1) - 1.0f, 2));
      //olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y), static_cast<int32_t> (ENGINE_PGE_104_DEFAULT_S * z), fillCol);
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y), ENGINE_PGE_104_DEFAULT_S, static_cast<int32_t> (ENGINE_PGE_104_DEFAULT_S * z), fillCol);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_104::easeInOutSine (float x)
{
  return -(std::cos (static_cast<float> (M_PI) * x) - 1.0f) / 2.0f;
}

float
PGE_104::loopNoise (int32_t x, int32_t y, float theta,
                    noise::module::Perlin* noise_in)
{
  static double rad = 1.0;
  static double sc = 1.0 / 500.0;
  static double offset = 10.0;
  return static_cast<float> (noise_in->GetValue (offset + rad * cos (x * sc + static_cast<double> (theta)),
                                                 offset + rad * sin (x * sc + static_cast<double> (theta)),
                                                 y * sc));
}
