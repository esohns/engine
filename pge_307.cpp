#include "stdafx.h"

#include "pge_307.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_307::PGE_307 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 307");
}

bool
PGE_307::OnUserCreate ()
{
  return true;
}

bool
PGE_307::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static olc::vi2d center (olc::PixelGameEngine::ScreenWidth () / 2,
                           olc::PixelGameEngine::ScreenHeight () / 2 + 45);

  //rotate(-.1);

  float start = 2.0f * static_cast<float> (M_PI) + frame_count_i / 30.0f;
  float r, g, b;
  for (float theta = static_cast<float> (M_PI) / 6.0f; theta < 2.0f * static_cast<float> (M_PI) / 3.0f + static_cast<float> (M_PI) / 6.0f; theta += 0.01f)
    for (float t = theta; t < 2.0f * static_cast<float> (M_PI) + static_cast<float> (M_PI) / 6.0f; t += 2.0f * static_cast<float> (M_PI) / 3.0f)
    {
      Common_Image_Tools::HSVToRGB ((std::fmod (std::abs (start - t), 2.0f * static_cast<float> (M_PI)) / (2.0f * static_cast<float> (M_PI))) * 360.0f, 1.0f, 1.0f,
                                    r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
      olc::PixelGameEngine::FillCircle (center.x + static_cast<int32_t> (x (t) * 130.0f),
                                        center.y + static_cast<int32_t> (y (t) * 130.0f),
                                                   static_cast<int32_t> ((z (t) + 1.0f) * 22.5f) + 4,
                                        color);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_307::x (float t)
{
  return std::sin (t) + 2.0f * std::sin (2.0f * t);
}

float
PGE_307::y (float t)
{
  return std::cos (t) - 2.0f * std::cos (2.0f * t);
}

float
PGE_307::z (float t)
{
  return -std::sin (3.0f * t);
}
