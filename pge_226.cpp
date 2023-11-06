#include "stdafx.h"

#include "pge_226.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_226::PGE_226 ()
 : olc::PixelGameEngine ()
 , objects_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 226");
}

bool
PGE_226::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_226_DEFAULT_NUMBER_OF_OBJECTS; i++)
    objects_.push_back (object (this));

  return true;
}

bool
PGE_226::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_226_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_226_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<object>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator).show (this);
    (*iterator).move (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
