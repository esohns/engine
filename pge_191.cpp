#include "stdafx.h"

#include "pge_191.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_191::PGE_191 ()
 : olc::PixelGameEngine ()
 , points_ (ENGINE_PGE_191_DEFAULT_NUMBER_OF_POINTS, olc::vf2d (0.0f, 0.0f))
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 191");
}

bool
PGE_191::OnUserCreate ()
{
  return true;
}

bool
PGE_191::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static olc::Pixel color1 = {0xED, 0x18, 0x51, 255};
  static olc::Pixel color2 = {0x21, 0x07, 0x3E, 255};
  static int32_t frame_count_i = 1;

  float f = frame_count_i * 0.1f;
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();

  // draw screen
  for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    for (int32_t y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    {
      float d = 0.0f;
      for (size_t i = 0; i < points_.size (); i++)
        d += std::sin (points_[i].dist ({ static_cast<float> (x), static_cast<float> (y)}) * 0.08f);
      d /= static_cast<float> (points_.size ());
      p[x + y * olc::PixelGameEngine::ScreenWidth ()] =
        static_cast<int32_t> (std::floor (d * 15.0f + f)) % 7 == 0 ? color1 : color2;
    } // end FOR

  // update points
  for (size_t i = 0; i < points_.size (); i++)
  {
    points_[i].x =
      half_width_i  + half_width_i  * std::sin (frame_count_i * i * 0.0011f) * std::sin (frame_count_i * i * 0.0017f);
    points_[i].y =
      half_height_i + half_height_i * std::sin (frame_count_i * i * 0.0013f) * std::sin (frame_count_i * i * 0.0019f);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
