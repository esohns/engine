#include "stdafx.h"

#include "pge_29.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_29::PGE_29 ()
 : olc::PixelGameEngine()
 , particle_ (NULL)
 , walls_ ()
{
  sAppName = "Example 29";
}

PGE_29::~PGE_29 ()
{
  delete particle_;

  for (std::vector<wall*>::iterator iterator = walls_.begin ();
       iterator != walls_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_29::OnUserCreate ()
{
  srand (static_cast<unsigned int> (time (0)));

  float x1, x2, y1, y2;
  for (int i = 0; i < 5; i++)
  {
    x1 = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    x2 = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    y1 = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    y2 = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    walls_.push_back (new wall ({x1, y1}, {x2, y2}));
  } // end FOR

  x1 = -1.0f;
  y1 = -1.0f;
  x2 = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  y2 = -1.0f;
  walls_.push_back (new wall ({x1, y1}, {x2, y2}));
  x1 = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  y1 = -1.0f;
  x2 = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  y2 = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
  walls_.push_back (new wall ({x1, y1}, {x2, y2}));
  x1 = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  y1 = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
  x2 = -1.0f;
  y2 = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
  walls_.push_back (new wall ({x1, y1}, {x2, y2}));
  x1 = -1.0f;
  y1 = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
  x2 = -1.0f;
  y2 = -1.0f;
  walls_.push_back (new wall ({x1, y1}, {x2, y2}));

  particle_ = new particle ((olc::PixelGameEngine::ScreenWidth () - 1) / 2,
                            (olc::PixelGameEngine::ScreenHeight () - 1) / 2);

  return true;
}

bool
PGE_29::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<wall*>::iterator iterator = walls_.begin ();
      iterator != walls_.end ();
      ++iterator)
    (*iterator)->show (this);

  particle_->update (olc::PixelGameEngine::GetMouseX (),
                     olc::PixelGameEngine::GetMouseY ());

  particle_->show (this);
  particle_->look (walls_,
                   this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
