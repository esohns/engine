#include "stdafx.h"

#include "pge_334.h"

#include "ace/Log_Msg.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_334::PGE_334 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 334");
}

bool
PGE_334::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  reset ();

  return true;
}

bool
PGE_334::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    reset ();

  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a >= ENGINE_PGE_334_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_334_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator).update (&noise_);
    (*iterator).wrap (this);
    (*iterator).show (&noise_, this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_334::reset ()
{
  particles_.clear ();

  for (int i = 0; i < ENGINE_PGE_334_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (particle (this));

  olc::PixelGameEngine::Clear (olc::BLACK);
}
