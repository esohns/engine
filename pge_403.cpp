#include "stdafx.h"

#include "pge_403.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_403::PGE_403 ()
 : olc::PixelGameEngine ()
 , cells_ ()
{
  sAppName = "Example 403";
}

bool
PGE_403::OnUserCreate ()
{
  float step =
    static_cast<float> (olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_403_DEFAULT_PADDING * 2) / ENGINE_PGE_403_DEFAULT_CELL_COUNT;
  for (float y = ENGINE_PGE_403_DEFAULT_PADDING; y < olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_403_DEFAULT_PADDING; y += step)
    for (float x = ENGINE_PGE_403_DEFAULT_PADDING; x < olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_403_DEFAULT_PADDING; x += step)
      cells_.push_back (cell (x + step / 2.0f, y + step / 2.0f));

  return true;
}

bool
PGE_403::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_403_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_403_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<cell>::iterator iterator = cells_.begin ();
       iterator != cells_.end ();
       ++iterator)
  {
    (*iterator).update (this);
    (*iterator).show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
