#include "stdafx.h"

#include "pge_60.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_60::PGE_60 ()
 : olc::PixelGameEngine ()
 , agents_ ()
{
  sAppName = "Example 60";
}

PGE_60::~PGE_60 ()
{
  for (std::vector<agent*>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_60::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_60_DEFAULT_NUMBER_OF_AGENTS; i++)
    agents_.push_back (new agent (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1),
                                  Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1)));

  return true;
}

bool
PGE_60::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = static_cast<int> (agents_.size () - 1); i--;)
  {
    agents_[i]->show (this);

    for (int j = i; j--;)
    {
      olc::vf2d delta = agents_[j]->position_ - agents_[i]->position_;
      float dst = delta.mag ();

      bool found_b = false;
      for (std::vector<agent*>::iterator iterator = agents_.begin ();
           iterator != agents_.end ();
           ++iterator)
        if (dst > (*iterator)->position_.dist (agents_[i]->position_) &&
            dst > (*iterator)->position_.dist (agents_[j]->position_))
        {
          found_b = true;
          break;
        } // end IF
      if (!found_b)
      {
        olc::PixelGameEngine::DrawLine (agents_[i]->position_, agents_[j]->position_, olc::WHITE, 0xFFFFFFFF);

        agents_[i]->velocity_ += delta;
        if (dst > ENGINE_PGE_60_DEFAULT_THRESHOLD)
          delta *= -1.0f;
        agents_[j]->velocity_ += delta;
      } // end IF
    } // end FOR
  } // end FOR

  for (std::vector<agent*>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    (*iterator)->update (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
