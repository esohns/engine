#include "stdafx.h"

#include "pge_270.h"

#include <random>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_270::PGE_270 ()
 : olc::PixelGameEngine ()
 , f_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 270");
}

bool
PGE_270::OnUserCreate ()
{
  return true;
}

bool
PGE_270::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  static float increment_f = static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_270_DEFAULT_COMPLEXITY);
  for (float u = 0.0f; u < 2.0f * static_cast<float> (M_PI); u += increment_f)
  {
    float U = u + f_;
    for (float v = 0.0f; v < static_cast<float> (M_PI); v += increment_f)
    {
      float R =
        std::sin (v * std::abs (std::atan (std::tan (f_ / ENGINE_PGE_270_DEFAULT_FREQUENCY))) * ENGINE_PGE_270_DEFAULT_VELOCITY + f_) * ENGINE_PGE_270_DEFAULT_DISPLACEMENT + ENGINE_PGE_270_DEFAULT_RADIUS;
      float Z = std::cos (U) * std::sin (v) * R;
      uint8_t gray_i = static_cast<uint8_t> (Z * 2.0f);
      olc::Pixel color (gray_i, gray_i, gray_i);
      olc::PixelGameEngine::Draw (half_width_i + static_cast<int32_t> (std::sin (U) * std::sin (v) * ENGINE_PGE_270_DEFAULT_RADIUS),
                                  half_height_i + static_cast<int32_t> (std::cos (v) * ENGINE_PGE_270_DEFAULT_RADIUS),
                                  //Z,
                                  color);
    } // end FOR
  } // end FOR

  f_ += 0.2f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
