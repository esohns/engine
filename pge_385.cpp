#include "stdafx.h"

#include "pge_385.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_385::PGE_385 ()
 : olc::PixelGameEngine ()
 , numberOfTypes_ (0)
 , colorStep_ (0.0f)
 , particles_ ()
 , forces_ ()
 , minDistances_ ()
 , radii_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 385");
}

bool
PGE_385::OnUserCreate ()
{
  numberOfTypes_ = Common_Tools::getRandomNumber (2, 6);
  colorStep_ = 360.0f / static_cast<float> (numberOfTypes_);

  particles_.clear ();
  for (int i = 0; i < ENGINE_PGE_385_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (particle (numberOfTypes_, this));

  forces_.clear ();
  minDistances_.clear ();
  radii_.clear ();
  for (int i = 0; i < numberOfTypes_; i++)
  {
    std::vector<float> column_a (numberOfTypes_, 0.0f);
    forces_.push_back (column_a);
    minDistances_.push_back (column_a);
    radii_.push_back (column_a);
  } // end FOR

  setParameters ();

  return true;
}

bool
PGE_385::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_385_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_385_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    setParameters ();
  if (olc::PixelGameEngine::GetMouse (1).bPressed)
    OnUserCreate ();

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator).update (particles_,
                        forces_,
                        minDistances_,
                        radii_,
                        this);
    (*iterator).draw (colorStep_,
                      this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_385::setParameters ()
{
  for (int i = 0; i < numberOfTypes_; i++)
    for (int j = 0; j < numberOfTypes_; j++)
    {
      forces_[i][j] = Common_Tools::getRandomNumber (0.3f, 1.0f);
      if (Common_Tools::testRandomProbability (0.5f))
        forces_[i][j] *= -1.0f;
      minDistances_[i][j] = Common_Tools::getRandomNumber (30.0f, 50.0f);
      radii_[i][j] = Common_Tools::getRandomNumber (70.0f, 250.0f);
    } // end FOR
}
