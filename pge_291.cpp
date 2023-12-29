#include "stdafx.h"

#include "pge_291.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_291::PGE_291 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 291");
}

bool
PGE_291::OnUserCreate ()
{
  return true;
}

bool
PGE_291::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static olc::vf2d half_size_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                                olc::PixelGameEngine::ScreenHeight () / 2.0f);

  olc::PixelGameEngine::Clear (olc::BLACK);

  static float maxD = half_size_s.dist ({0.0f, 0.0f});
  float power =
    1.0f + 5.0f * static_cast<float> (std::pow (0.5f * (1.0f + std::sin ((static_cast<float> (M_PI) / 180.0f) * frame_count_i)), 3));
  for (int i = 0; i < 4000; i++)
  {
    int32_t x =
      Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int32_t y =
      Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    float d =
      static_cast<float> (std::pow (half_size_s.dist ({static_cast<float> (x), static_cast<float> (y)}) / maxD, 0.5f));
    float th =
      std::atan2 (y - half_size_s.y, x - half_size_s.x);
    float fth =
      (th * 3.0f + (static_cast<float> (M_PI) / 180.0f) * (frame_count_i * 2.0f));
    float fth2 =
      (th + (static_cast<float> (M_PI) / 180.0f) * frame_count_i) * 3.0f;
    float f =
      0.5f * (1.0f + std::sin (d * 2.0f * static_cast<float> (M_PI) * 3.0f + fth));
    float fd2 =
      0.5f * (1.0f + std::sin (d * 2.0f * static_cast<float> (M_PI) * 7.0f + fth2 + (static_cast<float> (M_PI) / 180.0f) * frame_count_i));
    uint8_t gray_i = static_cast<uint8_t> (255.0f * std::pow (f, power));
    olc::Pixel color (gray_i, gray_i, gray_i, 255);
    olc::PixelGameEngine::FillCircle (x, y, static_cast<int32_t> (5.0f * fd2), color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
