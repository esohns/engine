#include "stdafx.h"

#include "pge_319.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_319::PGE_319 ()
 : olc::PixelGameEngine ()
 , balls_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 319");
}

bool
PGE_319::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_319_DEFAULT_NUMBER_OF_BALLS; i++)
    balls_.push_back (ball (this));

  return true;
}

bool
PGE_319::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_319_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_319_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<ball>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
    (*iterator).run (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
