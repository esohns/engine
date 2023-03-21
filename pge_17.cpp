#include "stdafx.h"

#include "pge_17.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_17::PGE_17 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 17";
}

PGE_17::~PGE_17 ()
{
}

bool
PGE_17::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_17_DEFAULT_BOIDS; i++)
    flock_.push_back (new boid ());

  return true;
}

bool
PGE_17::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<boid*>::iterator i = flock_.begin ();
       i != flock_.end ();
       i++)
  {
    (*i)->edges ();
    (*i)->flock (flock_);
    (*i)->update ();
    (*i)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
