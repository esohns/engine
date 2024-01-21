#include "stdafx.h"

#include "pge_323.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_323::PGE_323 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 323");
}

bool
PGE_323::OnUserCreate ()
{
  return true;
}

bool
PGE_323::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    {
      float f = frame_count_i / 50.0f;
      float k = 1.0f + 0.5f * std::sin (f * 0.1f);
      float n = 20.0f *
        std::sin (f + std::sin (k * y / 99.0f) * k * x / 161.0f - k * y / 323.0f) *
        std::sin (-f * 0.8f - k * x / 249.0f + std::sin (k * x / 79.0f) * k * y / 118.0f);
      float r, g, b;
      Common_Image_Tools::HSVToRGB (180.0f + 180.0f * std::sin (n), 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
      p[x + y * olc::PixelGameEngine::ScreenWidth ()] = color;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
