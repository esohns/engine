#include "stdafx.h"

#include "pge_146.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_146::PGE_146 ()
 : olc::PixelGameEngine ()
 , h_ (0.0f)
 , q_ (0.0f)
 , y_ (0)
{
  sAppName = "Example 146";
}

bool
PGE_146::OnUserCreate ()
{
  h_ = ENGINE_PGE_146_DEFAULT_W / 2.0f;
  q_ = ENGINE_PGE_146_DEFAULT_W / 4.0f;

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_146::OnUserUpdate (float fElapsedTime)
{
  olc::Pixel color;
  if (y_ < ENGINE_PGE_146_DEFAULT_W)
    for (int32_t x = ENGINE_PGE_146_DEFAULT_W; x--;)
    {
      float l = f ((x - h_) / q_, (y_ - h_) / q_);
      color.r = static_cast<uint8_t> (l > 255.0f ? 255 : l < 0.0f ? 0 : l);
      color.g = static_cast<uint8_t> (l * 2.0f > 255.0f ? 255 : l < 0.0f ? 0 : l);
      color.b = static_cast<uint8_t> (l * 4.0f > 255.0f ? 255 : l < 0.0f ? 0 : l);
      int64_t y_and_l = static_cast<int64_t> (y_ + l);
      if (y_and_l < 0) y_and_l = 0;
      if (y_and_l > olc::PixelGameEngine::ScreenHeight () - 1) y_and_l = olc::PixelGameEngine::ScreenHeight () - 1;
      olc::PixelGameEngine::DrawLine (x, y_, x, static_cast<int32_t> (y_and_l), color, 0xFFFFFFFF);
    } // end FOR
  y_++;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_146::f (float x, float y, int n)
{
  return n ? f (std::cos (3.0f * x) - std::sin (4.0f * y), x * x - y * y - 2.0f * x * y, n - 1)
           : (x - y) * (x - y) * 24.0f;
}
