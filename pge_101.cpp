#include "stdafx.h"

#include "pge_101.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_101::PGE_101 ()
 : olc::PixelGameEngine ()
 , movers_ ()
{
  sAppName = "Example 101";
}

PGE_101::~PGE_101 ()
{
  for (std::vector<mover*>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_101::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_101_DEFAULT_NUMBER_OF_MOVERS; i++)
  {
    float r = Common_Tools::getRandomNumber (5.0f, 15.0f);
    float x = Common_Tools::getRandomNumber (r / 2.0f, olc::PixelGameEngine::ScreenWidth () - r / 2.0f);
    float y = Common_Tools::getRandomNumber (r / 2.0f, olc::PixelGameEngine::ScreenHeight () - r / 2.0f);
    movers_.push_back (new mover (x, y, r));
  } // end FOR

  return true;
}

bool
PGE_101::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (size_t i = 0; i < movers_.size (); i++)
    for (size_t j = 0; j < movers_.size (); j++)
      if (i != j)
      {
        float dis = movers_[i]->position_.dist (movers_[j]->position_);
        if (dis < ENGINE_PGE_101_DEFAULT_LENGTH)
        {
          //float sw = Common_GL_Tools::map (dis, 0.0f, ENGINE_PGE_101_DEFAULT_LENGTH, 20.0f, 0.0f);
          //strokeWeight(sw);
          olc::PixelGameEngine::DrawLine (movers_[i]->position_, movers_[j]->position_, olc::BLACK, 0xFFFFFFFF);
        } // end IF
      } // end IF

  for (std::vector<mover*>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
    (*iterator)->run (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
