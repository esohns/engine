#include "stdafx.h"

#include "pge_20.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_20::PGE_20 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 20";
}

PGE_20::~PGE_20 ()
{
  delete mover_;
  delete spring_;
}

bool
PGE_20::OnUserCreate ()
{
  ACE_NEW_NORETURN (mover_,
                    mover ({ENGINE_PGE_20_DEFAULT_WIDTH / 2, 100}));
  ACE_NEW_NORETURN (spring_,
                    spring ({ENGINE_PGE_20_DEFAULT_WIDTH / 2, 10}, 100));

  return true;
}

bool
PGE_20::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    mover_->clicked (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());
  else if (olc::PixelGameEngine::GetMouse (0).bReleased)
    mover_->stopDragging ();

  // Apply a gravity force to the bob
  olc::vd2d gravity = { 0.0, 2.0 };
  mover_->applyForce (gravity);

  // Connect the bob to the spring (this calculates the force)
  spring_->connect (*mover_);
  // Constrain spring distance between min and max
  spring_->constrainLength (*mover_, 30, 200);

  mover_->update ();
  // If it's being dragged
  mover_->drag (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());

  // Draw everything
  spring_->showLine (this, *mover_); // Draw a line between spring and bob
  mover_->show (this);
  spring_->show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
