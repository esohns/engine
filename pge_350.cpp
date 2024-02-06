#include "stdafx.h"

#include "pge_350.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_350::PGE_350 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 350");
}

bool
PGE_350::OnUserCreate ()
{
  return true;
}

bool
PGE_350::OnUserUpdate (float fElapsedTime)
{
  static float elapsed_time_f = 0.0f;
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  float elapsed_milli_seconds_f = elapsed_time_f * 1000.0f;
  olc::Pixel color;
  for (int i = 0; i < 400; i++)
  {
    float x =
      (olc::PixelGameEngine::ScreenWidth () / 3.0f)  * std::tan (i / 3.0f - 0.000013f * elapsed_milli_seconds_f) + center_s.x;
    float y =
      (olc::PixelGameEngine::ScreenHeight () / 3.0f) * std::tan (i / 7.0f - 0.0001f   * elapsed_milli_seconds_f) + center_s.y;

    color.r =
      static_cast<uint8_t> (127.0f + (127.0f * std::sin (0.001f  * elapsed_milli_seconds_f)));
    color.g =
      static_cast<uint8_t> (127.0f + (127.0f * std::sin (0.0016f * elapsed_milli_seconds_f)));
    color.b =
      static_cast<uint8_t> (127.0f + (127.0f * std::sin (0.0022f * elapsed_milli_seconds_f)));

    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                      3, color);
  } // end FOR

  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
