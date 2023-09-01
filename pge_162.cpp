#include "stdafx.h"

#include "pge_162.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_162::PGE_162 ()
 : olc::PixelGameEngine ()
 , t_ (ENGINE_PGE_162_DEFAULT_W)
{
  sAppName = "Example 162";
}

bool
PGE_162::OnUserCreate ()
{
  return true;
}

bool
PGE_162::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  t_ += 9;

  for (int32_t y = 0; y < ENGINE_PGE_162_DEFAULT_W; y++)
    for (int32_t x = 0; x < ENGINE_PGE_162_DEFAULT_W; x++)
    {
      float a_f = static_cast<float> (std::sqrt ((x - olc::PixelGameEngine::GetMouseX ()) * (x - olc::PixelGameEngine::GetMouseX ()) +
                                                 (y - olc::PixelGameEngine::GetMouseY ()) * (y - olc::PixelGameEngine::GetMouseX ())));
      float b_f = static_cast<float> (std::atan2 (x, y) * ENGINE_PGE_162_DEFAULT_W + t_);
      uint32_t a = (uint32_t)a_f ^ (uint32_t)b_f;
      uint32_t b = a ^ a * 2;
      if (b % 255 == 0)
        olc::PixelGameEngine::DrawRect (x, y, 5, 5, olc::WHITE);
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
