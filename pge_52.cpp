#include "stdafx.h"

#include "pge_52.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_52::PGE_52 ()
 : olc::PixelGameEngine ()
 , attractors_ ()
 , particles_ ()
{
  sAppName = "Example 52";
}

PGE_52::~PGE_52 ()
{
  for (std::vector<attractor*>::iterator iterator = attractors_.begin ();
       iterator != attractors_.end ();
       ++iterator)
    delete *iterator;
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_52::OnUserCreate ()
{
  ACE_OS::srand (static_cast<u_int> (time (0)));

  int number_of_attractors_i = Common_Tools::getRandomNumber (1, 10);
  for (int i = 0; i < number_of_attractors_i; i++)
    attractors_.push_back (new attractor (this));
  for (int i = 0; i < ENGINE_PGE_52_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (new particle (this));

  return true;
}

bool
PGE_52::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < particles_.size (); i++)
  {
    particles_[i]->update (attractors_, this);
    particles_[i]->show (this);
  } // end FOR

  for (int i = 0; i < attractors_.size (); i++)
  {
    attractors_[i]->update (this);
    attractors_[i]->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
