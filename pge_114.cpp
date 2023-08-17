#include "stdafx.h"

#include "pge_114.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_114::PGE_114 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , flag_ (true)
{
  sAppName = "Example 114";
}

PGE_114::~PGE_114 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_114::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_114_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (new particle (this));

  return true;
}

bool
PGE_114::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (size_t i = 0; i < particles_.size (); i++)
  {
    particles_[i]->show (this);
    particles_[i]->update (this);

    for (size_t j = i + 1; j < particles_.size (); j++)
    {
      particles_[j]->update (this);
      if (particles_[i]->position_.dist (particles_[j]->position_) < ENGINE_PGE_114_DEFAULT_DISTANCE)
        for (size_t k = j + 1; k < particles_.size (); k++)
        {
          if (particles_[k]->position_.dist (particles_[j]->position_) < ENGINE_PGE_114_DEFAULT_DISTANCE)
          {
            olc::Pixel color_s;
            if (flag_)
            {
              color_s = particles_[k]->color_;
              color_s.a = 95;
            } // end IF
            else
            {
              color_s = olc::BLACK;
              color_s.a = 20;
            } // end ELSE

            olc::PixelGameEngine::FillTriangle (particles_[i]->position_, particles_[j]->position_, particles_[k]->position_, color_s);
          } // end IF
 
          particles_[k]->update (this);
        } // end FOR
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
