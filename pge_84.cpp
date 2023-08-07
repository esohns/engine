#include "stdafx.h"

#include "pge_84.h"

#include "ace/Log_Msg.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_84::PGE_84 ()
 : olc::PixelGameEngine ()
 , angle_ (0.0f)
{
  sAppName = "Example 84";
}

bool
PGE_84::OnUserCreate ()
{
  return true;
}

bool
PGE_84::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_84_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_84_DEFAULT_ALPHA_DECAY : 0);

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  // *TODO*: what is wrong here; the circles are not touching (see: https://openprocessing.org/sketch/1150505)
  float a = std::sin (angle_);
  float b = std::cos (angle_);
  float c = std::sin (angle_ * 1.5f);
  float d = std::cos (angle_ * 1.5f);
  float e = std::sin (angle_ * 2.0f);
  float f = std::cos (angle_ * 2.0f);
  olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (a * 80.0f), half_height_i + static_cast<int32_t> (b * 80.0f), 100, olc::WHITE, 0xFF);
  olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (a * 80.0f + c * 150.0f), half_height_i + static_cast<int32_t> (b * 80.0f + d * 150.0f), 200, olc::WHITE, 0xFF);
  olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (a * 80.0f + c * 150.0f + e * 110.0f), half_height_i + static_cast<int32_t> (b * 80.0f + d * 150.0f + f * 110.0f), 20, olc::WHITE, 0xFF);

  angle_ += 0.025f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
