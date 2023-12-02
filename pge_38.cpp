#include "stdafx.h"

#include "pge_38.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "engine_common.h"

PGE_38::PGE_38 ()
 : olc::PixelGameEngine ()
 , barrier_ ()
 , frameCounter_ (0)
 , population_ (NULL)
 , target_ ()
{
  sAppName = "Example 38";
}

PGE_38::~PGE_38 ()
{
  delete population_;
}

bool
PGE_38::OnUserCreate ()
{
  barrier_.rw = olc::PixelGameEngine::ScreenWidth () / 2;
  barrier_.rh = 10;
  barrier_.rx = (olc::PixelGameEngine::ScreenWidth () - barrier_.rw) / 2;
  barrier_.ry = (olc::PixelGameEngine::ScreenHeight () - barrier_.rh) / 2;

  population_ = new population (this);

  target_ = {olc::PixelGameEngine::ScreenWidth () / 2 , 50};

  return true;
}

bool
PGE_38::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  population_->update (target_,
                       barrier_,
                       this,
                       frameCounter_);

  ++frameCounter_;
  if (frameCounter_ == ENGINE_PGE_38_DEFAULT_LIFESPAN)
  {
    frameCounter_ = 0;
    population_->evaluate (this,
                           target_);
    population_->selection (this);
  } // end IF
  else
    population_->display (this);

  olc::PixelGameEngine::FillRect (barrier_.rx, barrier_.ry, barrier_.rw, barrier_.rh, olc::WHITE);
  olc::PixelGameEngine::FillCircle (target_, ENGINE_PGE_38_DEFAULT_TARGET_RADIUS, olc::GREEN);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
