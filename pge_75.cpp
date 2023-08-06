#include "stdafx.h"

#include "pge_75.h"

#include "ace/Log_Msg.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_75::PGE_75 ()
 : olc::PixelGameEngine ()
 , a_ (0.0f)
{
  sAppName = "Example 75";
}

bool
PGE_75::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_75::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static int frame_count_i = 1;

  for (int i = -9; i <= 9; i++)
    conchoid (a_ + i);
  
  a_ = 9.0f * static_cast<float> (std::sin (frame_count_i / 100.0f));

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_75::conchoid (float a_in)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  for (int i = 0; i < 356; i++)
  {
    int32_t x = half_width_i +
      static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () / 60.0f * (1.0f / std::cos (i) + a_in * std::cos (i)) * std::sin (i));
    int32_t y = half_height_i +
      static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / 60.0f * (1.0f / std::cos (i) + a_in * std::cos (i)) * std::cos (i));
    static int32_t prev_x = x;
    static int32_t prev_y = y;
    //olc::PixelGameEngine::Draw (x, y, olc::WHITE);
    olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, {255, 255, 255, 20}, 0xFFFFFFFF);
    prev_x = x;
    prev_y = y;
  } // end FOR
}
