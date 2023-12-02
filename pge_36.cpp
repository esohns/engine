#include "stdafx.h"

#include "pge_36.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "engine_common.h"

PGE_36::PGE_36 ()
 : olc::PixelGameEngine ()
 , vehicles_ ()
 , food_ ()
 , poison_ ()
{
  sAppName = "Example 36";
}

PGE_36::~PGE_36 ()
{
  for (vehicles_t::iterator iterator = vehicles_.begin ();
       iterator != vehicles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_36::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_36_DEFAULT_NUM_VEHICLES; i++)
  {
    int x = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int y = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    vehicles_.push_back (new vehicle (x, y, NULL));
  } // end FOR

  for (int i = 0; i < ENGINE_PGE_36_DEFAULT_NUM_FOOD; i++)
  {
    int x = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int y = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    food_.push_back ({x, y});
  } // end FOR
  for (int i = 0; i < ENGINE_PGE_36_DEFAULT_NUM_POISON; i++)
  {
    int x = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int y = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    poison_.push_back ({x, y});
  } // end FOR

  return true;
}

bool
PGE_36::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_FOOD_PROBABILITY))
  {
    int x = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int y = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    food_.push_back ({x, y});
  } // end IF
  if (Common_Tools::testRandomProbability (ENGINE_PGE_36_DEFAULT_POISON_PROBABILITY))
  {
    int x = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int y = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    poison_.push_back ({x, y});
  } // end IF

  for (vehicle::positions_t::const_iterator iterator = food_.begin ();
       iterator != food_.end ();
       ++iterator)
    olc::PixelGameEngine::FillCircle (*iterator, ENGINE_PGE_36_DEFAULT_FOOD_RADIUS, olc::GREEN);
  for (vehicle::positions_t::const_iterator iterator = poison_.begin ();
       iterator != poison_.end ();
       ++iterator)
    olc::PixelGameEngine::FillCircle (*iterator, ENGINE_PGE_36_DEFAULT_FOOD_RADIUS, olc::RED);

  vehicles_t::iterator iterator = vehicles_.begin ();
  while (iterator != vehicles_.end ())
  {
    (*iterator)->boundaries (this);
    (*iterator)->behaviors (food_, poison_);
    (*iterator)->update ();
    (*iterator)->display (this, false);

    vehicle* vehicle_p = (*iterator)->clone ();
    if (vehicle_p)
      vehicles_.push_back (vehicle_p);
    if ((*iterator)->dead ())
    {
      food_.push_back ((*iterator)->position_);
      iterator = vehicles_.erase (iterator);
    } // end IF
    else
      ++iterator;
  } // end WHILE

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
