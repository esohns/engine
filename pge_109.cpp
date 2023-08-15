#include "stdafx.h"

#include "pge_109.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_109::PGE_109 ()
 : olc::PixelGameEngine ()
 , balls_ ()
 , mX_ (0.0f)
 , mY_ (0.0f)
{
  sAppName = "Example 109";
}

PGE_109::~PGE_109 ()
{
  for (std::vector<ball*>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_109::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_109_DEFAULT_NUMBER_OF_BALLS; i++)
    balls_.push_back (new ball (this));

  return true;
}

bool
PGE_109::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    olc::PixelGameEngine::Clear (olc::BLACK);

    mX_ += 0.3f * (olc::PixelGameEngine::GetMouseX () - mX_);
    mY_ += 0.3f * (olc::PixelGameEngine::GetMouseY () - mY_);
  } // end IF

  mX_ += 0.3f * (olc::PixelGameEngine::GetMouseX () - mX_);
  mY_ += 0.3f * (olc::PixelGameEngine::GetMouseY () - mY_);

  for (std::vector<ball*>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
    (*iterator)->show (this, mX_, mY_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
