#include "stdafx.h"

#include "pge_198.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_198::PGE_198 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 198");
}

bool
PGE_198::OnUserCreate ()
{
  return true;
}

bool
PGE_198::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  int flipFlop = 1;
  for (int32_t y = ENGINE_PGE_198_DEFAULT_SPACING / 2; y < olc::PixelGameEngine::ScreenHeight (); y += ENGINE_PGE_198_DEFAULT_SPACING)
  {
    flipFlop = (flipFlop + 1) % 2;
    for (int32_t x = static_cast<int> (ENGINE_PGE_198_DEFAULT_SPACING * (1.0f - flipFlop * 0.5f)); x < olc::PixelGameEngine::ScreenWidth (); x += ENGINE_PGE_198_DEFAULT_SPACING)
    {
      olc::vf2d dx_y (static_cast<float> (x - olc::PixelGameEngine::GetMouseX ()),
                      static_cast<float> (y - olc::PixelGameEngine::GetMouseY ()));
      float radius = dx_y.mag ();
      float angle = std::atan2 (dx_y.y, dx_y.x) - frame_count_i * (2.0f * static_cast<float> (M_PI)) / 40.0f;
      float variation =
        ENGINE_PGE_198_DEFAULT_MEAN_DIAMETER / 2.0f * std::cos (radius * (2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_198_DEFAULT_SPACING) / 4.0f + angle);
      float varried = ENGINE_PGE_198_DEFAULT_MEAN_DIAMETER * 1.25f + variation;
      olc::PixelGameEngine::FillCircle (x, y, static_cast<int32_t> (varried / 2.0f), olc::WHITE);
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
