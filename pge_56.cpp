#include "stdafx.h"

#include "pge_56.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_56::PGE_56 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = "Example 56";
}

bool
PGE_56::OnUserCreate ()
{
  return true;
}

bool
PGE_56::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static unsigned int frame_count_i = 0;

  for (int32_t y = 100; y < olc::PixelGameEngine::ScreenHeight (); y += 5)
  {
    for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth (); ++x)
      olc::PixelGameEngine::Draw (x, static_cast<int32_t> (y - 80.0 / (1.0 + std::pow (x - 150, 4) / 8e6) * noise_.GetValue (x / 30.0 + frame_count_i / 50.0 + y, 0.0, 0.0)));
    //olc::PixelGameEngine::Draw (olc::PixelGameEngine::ScreenHeight () - 1, static_cast<int32_t> (1e4));
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
