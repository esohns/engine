#include "stdafx.h"

#include "pge_171.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_171::PGE_171 ()
 : olc::PixelGameEngine ()
 , x_ (0.0f)
 , y_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 171");
}

bool
PGE_171::OnUserCreate ()
{
  return true;
}

bool
PGE_171::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float f = frame_count_i * 0.01f;
  for (int i = 0; i < ENGINE_PGE_171_DEFAULT_H; i++)
    for (int j = 0; j < ENGINE_PGE_171_DEFAULT_H; j++)
    {
      float a = i * 2 - y_ + f;
      float b = x_ + a / ENGINE_PGE_171_DEFAULT_H + f;
      x_ = std::sin (b) - std::cos (a);
      y_ = std::cos (b) - std::sin (a);
      olc::Pixel color;
      color.r = static_cast<uint8_t> (Common_GL_Tools::map (i, 0, ENGINE_PGE_171_DEFAULT_H, 0, 255));
      color.g = static_cast<uint8_t> (Common_GL_Tools::map (j, 0, ENGINE_PGE_171_DEFAULT_H, 0, 255));
      color.b = static_cast<uint8_t> ((x_ + y_) * 255);
      olc::PixelGameEngine::FillRect (half_width_i + static_cast<int32_t> (x_ * olc::PixelGameEngine::ScreenWidth () / 4),
                                      half_height_i + static_cast<int32_t> (y_ * olc::PixelGameEngine::ScreenHeight () / 4),
                                      3, 3,
                                      color);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
