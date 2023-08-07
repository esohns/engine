#include "stdafx.h"

#include "pge_81.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_81::PGE_81 ()
 : olc::PixelGameEngine ()
 , snakes_ ()
 , depart_ (-1)
 , dist_ (1)
 , background_ (olc::WHITE)
{
  sAppName = "Example 81";
}

PGE_81::~PGE_81 ()
{
  for (std::vector<snake*>::iterator iterator = snakes_.begin ();
       iterator != snakes_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_81::OnUserCreate ()
{
  reset ();

  return true;
}

bool
PGE_81::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    reset ();

  for (size_t i = 0; i < snakes_.size (); ++i)
  {
    snakes_[i]->run (this,
                     background_);
    if (snakes_[i]->branche_)
      snakes_.push_back (snakes_[i]->branche_);
  } // end FOR
  for (std::vector<snake*>::iterator iterator = snakes_.begin ();
       iterator != snakes_.end ();
       )
    if (!(*iterator)->vie_)
    {
      delete *iterator;
      iterator = snakes_.erase (iterator);
    } // end IF
    else
      ++iterator;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_81::reset ()
{
  int32_t xStart = 0, yStart = 0;

  depart_ = (depart_ + 1) % 4;
  switch (depart_)
  {
    case 0:
      xStart = 0;
      yStart = 0;
      break;
    case 1:
      xStart = olc::PixelGameEngine::ScreenWidth () - 1;
      yStart = 0;
      break;
    case 2:
      xStart = olc::PixelGameEngine::ScreenWidth () - 1;
      yStart = olc::PixelGameEngine::ScreenHeight () - 1;
      break;
    case 3:
      xStart = 0;
      yStart = olc::PixelGameEngine::ScreenHeight () -1;
      break;
  } // end SWITCH

  olc::PixelGameEngine::Clear (olc::WHITE);

  dist_ = 1;

  for (std::vector<snake*>::iterator iterator = snakes_.begin ();
       iterator != snakes_.end ();
       ++iterator)
    delete *iterator;
  snakes_.clear ();
  snakes_.push_back (new snake (xStart, yStart, Common_Tools::getRandomNumber (0, 3), dist_));
}
