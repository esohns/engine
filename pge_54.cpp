#include "stdafx.h"

#include "pge_54.h"

#include "ace/Log_Msg.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_54::PGE_54 ()
 : olc::PixelGameEngine ()
 , stars_ ()
{
  sAppName = "Example 54";
}

PGE_54::~PGE_54 ()
{
  for (std::vector<star*>::iterator iterator = stars_.begin ();
       iterator != stars_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_54::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_54_DEFAULT_NUMBER_OF_STARS; i++)
    stars_.push_back (new star (this));

  return true;
}

bool
PGE_54::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < stars_.size (); i++)
  {
    stars_[i]->update (this);
    for (int j = 0; j < stars_.size (); j++)
      if (i > j)
      {
        float d = stars_[i]->position_.dist (stars_[j]->position_);
        if (d <= olc::PixelGameEngine::ScreenWidth () / 10.0f)
          olc::PixelGameEngine::DrawLine (stars_[i]->position_, stars_[j]->position_, olc::WHITE, 0xFFFFFFFF);
      } // end IF
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
