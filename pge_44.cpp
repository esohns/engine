#include "stdafx.h"

#include "pge_44.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_44::PGE_44 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , vehicles_ ()
{
  sAppName = "Example 44";
}

PGE_44::~PGE_44 ()
{
  for (std::vector<vehicle*>::iterator iterator = vehicles_.begin ();
       iterator != vehicles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_44::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_44_DEFAULT_NUMBER_OF_VEHICLES; i++)
    vehicles_.push_back (new vehicle (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth ()),
                                      Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight ())));

  return true;
}

bool
PGE_44::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<vehicle*>::iterator iterator = vehicles_.begin ();
       iterator != vehicles_.end ();
       ++iterator)
  {
    (*iterator)->wander (&noise_);

    (*iterator)->update ();
    (*iterator)->show (this);
    (*iterator)->edges (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
