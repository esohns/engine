#include "stdafx.h"

#include "pge_205.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_205::PGE_205 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 205");
}

PGE_205::~PGE_205 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_205::OnUserCreate ()
{
  for (int i = 0; i < 600; i++)
  {
    float x = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
    float y = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
    particles_.push_back (new particle (x, y, ENGINE_PGE_205_DEFAULT_RADIUS));
  } // end FOR

  return true;
}

bool
PGE_205::OnUserUpdate (float fElapsedTime)
{
  static olc::vf2d center_s (static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2),
                             static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2));

  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    for (std::vector<particle*>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      (*iterator)->updateMouse (this);
  } // end IF

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    for (std::vector<particle*>::iterator iterator_2 = particles_.begin ();
         iterator_2 != particles_.end ();
         ++iterator_2)
    {
      if (*iterator != *iterator_2)
        (*iterator)->updateForce (**iterator_2);
    } // end FOR

    olc::vf2d temp = (*iterator)->position_;
    temp -= center_s;
    temp *= -ENGINE_PGE_205_DEFAULT_CENTER_STRENGTH;
    (*iterator)->acceleration_ += temp;
    (*iterator)->update (center_s, this);
    (*iterator)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
