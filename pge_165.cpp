#include "stdafx.h"

#include "pge_165.h"

#include "ace/Log_Msg.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_165::PGE_165 ()
 : olc::PixelGameEngine ()
 , t_ (0)
{
  sAppName = "Example 165";
}

bool
PGE_165::OnUserCreate ()
{
  return true;
}

bool
PGE_165::OnUserUpdate (float fElapsedTime)
{
  t_ += 3;

  olc::PixelGameEngine::Clear (olc::BLACK);
 
  for (int32_t y = ENGINE_PGE_165_DEFAULT_W; y--;)
    olc::PixelGameEngine::DrawRect (static_cast<int32_t> (ENGINE_PGE_165_DEFAULT_W / 2 + std::sin (y + t_) / (y + 160.0f) * 70000),
                                    static_cast<int32_t> (ENGINE_PGE_165_DEFAULT_W - y + std::cos (y / 45.0f) * 36),
                                    15, 9,
                                    olc::WHITE);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
