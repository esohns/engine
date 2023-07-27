#include "stdafx.h"

#include "pge_70.h"

#include "ace/Log_Msg.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_70::PGE_70 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , a1_ (0.0f)
 , a2_ (0.0f)
 , a3_ (0.0f)
 , a4_ (0.0f)
 , a5_ (0.0f)
 , amax_ (0.0f)
 , mobiles_ ()
{
  sAppName = "Example 70";
}

PGE_70::~PGE_70 ()
{
  for (std::vector<mobile*>::iterator iterator = mobiles_.begin ();
       iterator != mobiles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_70::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  reset ();

  return true;
}

bool
PGE_70::OnUserUpdate (float fElapsedTime)
{
  //int pixels = GetDrawTargetWidth () * GetDrawTargetHeight ();
  //olc::Pixel* p = GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_70_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_70_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    reset ();

  for (std::vector<mobile*>::iterator iterator = mobiles_.begin ();
       iterator != mobiles_.end ();
       ++iterator)
    (*iterator)->run (this, &noise_, a2_, a3_, a4_, a5_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_70::reset ()
{
  amax_ = Common_Tools::getRandomNumber (0.0f, 5.0f);
  a1_ = Common_Tools::getRandomNumber (1.0f, amax_);
  a2_ = Common_Tools::getRandomNumber (1.0f, amax_);
  a3_ = Common_Tools::getRandomNumber (1.0f, amax_);
  a4_ = Common_Tools::getRandomNumber (1.0f, amax_);
  //a5_ = Common_Tools::getRandomNumber (amax_, 10.0f);
  a5_ = 10.0f;

  for (std::vector<mobile*>::iterator iterator = mobiles_.begin ();
       iterator != mobiles_.end ();
       ++iterator)
    delete *iterator;
  mobiles_.clear ();

  for (int i = 0; i < ENGINE_PGE_70_DEFAULT_NUMBER_OF_MOBILES; i++)
    mobiles_.push_back (new mobile (this, &noise_, a1_, a2_, a3_));

  olc::PixelGameEngine::Clear (olc::BLACK);
}
