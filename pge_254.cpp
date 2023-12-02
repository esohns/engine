#include "stdafx.h"

#include "pge_254.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_254::PGE_254 ()
 : olc::PixelGameEngine ()
 , agents_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 254");
}

bool
PGE_254::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_254_DEFAULT_NUMBER_OF_AGENTS; i++)
    agents_.push_back (agent (this));

  return true;
}

bool
PGE_254::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_254_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_254_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    (*iterator).update (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
