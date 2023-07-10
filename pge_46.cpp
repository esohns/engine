#include "stdafx.h"

#include "pge_46.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_46::PGE_46 ()
 : olc::PixelGameEngine ()
 , d_ (0.0f)
 , n_ (0.0f)
{
  sAppName = "Example 46";
}

bool
PGE_46::OnUserCreate ()
{
  return true;
}

bool
PGE_46::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  static int32_t prev_x = 0;
  static int32_t prev_y = 0;
  for (int i = 0; i < 361; i++)
  {
    float k = i * d_;
    float r = ENGINE_PGE_46_DEFAULT_RADIUS * static_cast<float> (std::sin (n_ * k * (M_PI / 180.0f)));
    int32_t x = half_width_i + static_cast<int32_t> (r * std::cos (k * (M_PI / 180.0f)));
    int32_t y = half_height_i + static_cast<int32_t> (r * std::sin (k * (M_PI / 180.0f)));
    //olc::PixelGameEngine::Draw (x, y, olc::WHITE);
    prev_x = (i ? prev_x : x);
    prev_y = (i ? prev_y : y);
    olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, olc::WHITE, 0xFFFFFFFF);
    prev_x = x;
    prev_y = y;
  } // end FOR

  for (int i = 0; i < 361; i++)
  {
    float k = static_cast<float> (i);
    float r = ENGINE_PGE_46_DEFAULT_RADIUS * static_cast<float> (std::sin (n_ * k * (M_PI / 180.0f)));
    int32_t x = half_width_i + static_cast<int32_t> (r * std::cos (k * (M_PI / 180.0f)));
    int32_t y = half_height_i + static_cast<int32_t> (r * std::sin (k * (M_PI / 180.0f)));
    //olc::PixelGameEngine::Draw (x, y, olc::MAGENTA);
    prev_x = (i ? prev_x : x);
    prev_y = (i ? prev_y : y);
    olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, olc::MAGENTA, 0xFFFFFFFF);
    prev_x = x;
    prev_y = y;
  } // end FOR

  d_ += 0.003f;
  n_ += 0.001f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
