#include "stdafx.h"

#include "pge_100.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_100::PGE_100 ()
 : olc::PixelGameEngine ()
 , stars_ ()
{
  sAppName = "Example 100";
}

PGE_100::~PGE_100 ()
{
  for (std::vector<star*>::iterator iterator = stars_.begin ();
       iterator != stars_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_100::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_100::OnUserUpdate (float fElapsedTime)
{
  olc::HWButton button_s = olc::PixelGameEngine::GetMouse (0);
  if (button_s.bPressed || button_s.bHeld)
    stars_.push_back (new star (this));

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (size_t i = 0; i < stars_.size (); ++i)
  {
    for (size_t k = 0; k < stars_.size (); ++k)
      if (i != k)
      {
        olc::vf2d f = stars_[k]->attract (*stars_[i]);
        stars_[i]->applyForce (f);
        stars_[i]->link (*stars_[k], this);
      } // end IF
    stars_[i]->update ();
    stars_[i]->show (this);
    stars_[i]->checkEdges ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
