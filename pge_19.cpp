#include "stdafx.h"

#include "pge_19.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_19::PGE_19 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 19";
}

PGE_19::~PGE_19 ()
{
  for (int i = 0; i < arms_.size (); ++i)
    delete arms_[i];
}

bool
PGE_19::OnUserCreate ()
{
  float da = static_cast<float> (M_PI);
  for (float a = 0.0F; a < 2.0F * M_PI; a += da)
  {
    float x = ENGINE_PGE_19_DEFAULT_WIDTH  / 2.0F + std::cos (a) * 300;
    float y = ENGINE_PGE_19_DEFAULT_HEIGHT / 2.0F + std::sin (a) * 300;
    arms_.push_back (new arm ({ x, y }));
  } // end FOR

  return true;
}

bool
PGE_19::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < arms_.size (); ++i)
  {
    arms_[i]->update({ static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                       static_cast<float> (olc::PixelGameEngine::GetMouseY ()) });
    arms_[i]->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
