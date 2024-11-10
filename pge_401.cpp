#include "stdafx.h"

#include "pge_401.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_401::PGE_401 ()
 : olc::PixelGameEngine ()
 , walkers_ ()
{
  sAppName = "Example 401";
}

bool
PGE_401::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_401_DEFAULT_NUMBER_OF_WALKERS; ++i)
    walkers_.push_back (walker (this));

  return true;
}

bool
PGE_401::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_401_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_401_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    walkers_.clear ();
    for (int i = 0; i < ENGINE_PGE_401_DEFAULT_NUMBER_OF_WALKERS; ++i)
      walkers_.push_back (walker (this));
  } // end IF

  for (std::vector<walker>::iterator iterator = walkers_.begin ();
       iterator != walkers_.end ();
       ++iterator)
    (*iterator).draw (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
