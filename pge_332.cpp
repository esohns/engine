#include "stdafx.h"

#include "pge_332.h"

#include "ace/Log_Msg.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_332::PGE_332 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 332");
}

bool
PGE_332::OnUserCreate ()
{
  return true;
}

bool
PGE_332::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_332_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_332_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < 50; i++)
    particles_.push_back (particle (this));

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    (*iterator).run (this);

  for (std::vector<particle>::reverse_iterator iterator = particles_.rbegin ();
       iterator != particles_.rend ();
       )
    if ((*iterator).position_.y >= static_cast<float> (olc::PixelGameEngine::ScreenHeight ()))
      iterator = std::vector<particle>::reverse_iterator (particles_.erase (--iterator.base ()));
    else
      ++iterator;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

