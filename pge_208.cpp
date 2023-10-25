#include "stdafx.h"

#include "pge_208.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_208::PGE_208 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 208");
}

bool
PGE_208::OnUserCreate ()
{
  return true;
}

bool
PGE_208::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);

  olc::Pixel color = olc::BLACK;
  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    {
      olc::PixelGameEngine::Draw (x, y, color);

      float deem  = static_cast<float> (y);     // This is where the equation is. channel 1
      float deem2 = static_cast<float> (x);     // Channel 2
      float deem3 = static_cast<float> (y + x); // Channel 3
      while (deem3 > static_cast<float> (ENGINE_PGE_208_DEFAULT_SIZE))
        deem3 = deem3 - static_cast<float> (ENGINE_PGE_208_DEFAULT_SIZE);
      deem  /= static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
      deem2 /= static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
      deem3 /= static_cast<float> (ENGINE_PGE_208_DEFAULT_SIZE);
      color.r = static_cast<uint8_t> (deem3 * 255.0f);
      color.g = static_cast<uint8_t> (deem  * 255.0f);
      color.b = static_cast<uint8_t> (deem2 * 255.0f);
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
