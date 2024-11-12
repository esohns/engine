#include "stdafx.h"

#include "pge_404.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_404::PGE_404 ()
 : olc::PixelGameEngine ()
 , fireflies_ ()
 , noise_ ()
{
  sAppName = "Example 404";
}

bool
PGE_404::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_404_DEFAULT_NUMBER_OF_FIREFLIES; i++)
    fireflies_.push_back (firefly (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                                   Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));

  return true;
}

bool
PGE_404::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_404_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_404_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<firefly>::iterator iterator = fireflies_.begin ();
       iterator != fireflies_.end ();
       ++iterator)
  {
    (*iterator).update (this);
    (*iterator).relate (fireflies_);
    (*iterator).mutate (&noise_);
    (*iterator).show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
