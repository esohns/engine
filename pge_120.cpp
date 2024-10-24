#include "stdafx.h"

#include "pge_120.h"

#include "ace/Log_Msg.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_120::PGE_120 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = "Example 120";
}

PGE_120::~PGE_120 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_120::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_120::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    for (int i = 0; i < ENGINE_PGE_120_DEFAULT_NUMBER_OF_PARTICLES; i++)
      particles_.push_back (new particle (this));
  } // end IF
  if (olc::PixelGameEngine::GetMouse (1).bPressed)
    olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       )
  {
    olc::vf2d origin_s;
    if (origin_s.dist ((*iterator)->velocity_) < 0.2f && (*iterator)->position_.y > olc::PixelGameEngine::ScreenHeight () * 0.99f - (*iterator)->size_)
    {
      delete *iterator;
      iterator = particles_.erase (iterator);
    } // end IF
    else
      ++iterator;
  } // end FOR

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator)->update (this);
    (*iterator)->show (this);
  } // end FOR

  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_120_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_120_DEFAULT_ALPHA_DECAY : 0);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
