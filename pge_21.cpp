#include "stdafx.h"

#include "pge_21.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_21::PGE_21 ()
 : olc::PixelGameEngine ()
 , stars_ ()
 , speed_ (0)
{
  sAppName = "Example 21";
}

PGE_21::~PGE_21 ()
{
}

bool
PGE_21::OnUserCreate ()
{
  for (int i = 0; i < 500; i++)
    stars_.push_back (star ());

  return true;
}

bool
PGE_21::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  speed_ =
    static_cast<ACE_UINT32> (Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (ENGINE_PGE_21_DEFAULT_WIDTH - 1), 0.0f, 50.0f));

  for (std::vector<star>::iterator iterator = stars_.begin ();
       iterator != stars_.end ();
       ++iterator)
  {
    (*iterator).update (speed_);
    (*iterator).show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
