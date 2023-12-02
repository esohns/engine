#include "stdafx.h"

#include "pge_28.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_28::PGE_28 ()
 : olc::PixelGameEngine()
 , particles_ ()
 , attractors_ ()
{
  sAppName = "Example 28";
}

PGE_28::~PGE_28 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_28::OnUserCreate ()
{
  return true;
}

bool
PGE_28::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bReleased)
    attractors_.push_back ({ olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY () });

  particles_.push_back (new particle (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1),
                                      Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1)));
  if (particles_.size () > 100)
  {
    std::vector<particle*>::iterator iterator = particles_.begin ();
    delete *iterator;
    particles_.erase (iterator);
  } // end IF

  for (std::vector<olc::vi2d>::iterator iterator = attractors_.begin ();
       iterator != attractors_.end ();
       ++iterator)
    olc::PixelGameEngine::Draw (*iterator, olc::GREEN);

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    for (std::vector<olc::vi2d>::iterator iterator_2 = attractors_.begin ();
         iterator_2 != attractors_.end ();
         ++iterator_2)
      (*iterator)->attracted (*iterator_2);
    (*iterator)->update ();
    (*iterator)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
