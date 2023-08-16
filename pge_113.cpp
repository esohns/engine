#include "stdafx.h"

#include "pge_113.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_113::PGE_113 ()
 : olc::PixelGameEngine ()
 , balls_ ()
{
  sAppName = "Example 113";
}

PGE_113::~PGE_113 ()
{
  for (std::vector<ball*>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_113::OnUserCreate ()
{
  balls_.push_back (new ball (5.0f, 0.0f, this));

  return true;
}

bool
PGE_113::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  olc::PixelGameEngine::Clear ({22, 8, 0, 255});

  for (int i = static_cast<int> (balls_.size () - 1); i >= 0; i--)
  {
    balls_[i]->update (i, balls_);
    balls_[i]->show (this);
    if (balls_[i]->isDead (this))
    {
      std::vector<ball*>::iterator iterator = balls_.begin ();
      std::advance (iterator, i);
      balls_.erase (iterator);
    } // end IF
  } // end FOR

  static float PHI = (1.0f + std::sqrt (5.0f)) / 2.0f;
  balls_.push_back (new ball (5.0f - (frame_count_i % 500) / 50.0f, frame_count_i * PHI * 2.0f * static_cast<float> (M_PI), this));

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
