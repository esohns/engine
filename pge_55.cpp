#include "stdafx.h"

#include "pge_55.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_55::PGE_55 ()
 : olc::PixelGameEngine ()
 , objects_ ()
{
  sAppName = "Example 55";
}

PGE_55::~PGE_55 ()
{
  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_55::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_55_DEFAULT_NUMBER_OF_OBJECTS; i++)
    objects_.push_back (new object (i));

  return true;
}

bool
PGE_55::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    noise_.SetSeed (static_cast<int> (time (NULL)));

  static unsigned int frame_count_i = 0;
  //static double x_offset_d = Common_Tools::getRandomNumber (0.0, 1000.0);
  static double x_offset_d = 1000.0;

  for (int i = 0; i < objects_.size (); i++)
    objects_[i]->draw (frame_count_i,
                       this,
                       &noise_,
                       x_offset_d);

  frame_count_i++;
  //x_offset_d += ENGINE_PGE_39_DEFAULT_X_INCREMENT;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
