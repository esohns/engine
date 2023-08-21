#include "stdafx.h"

#include "pge_133.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_133::PGE_133 ()
 : olc::PixelGameEngine ()
 , strands_ ()
{
  sAppName = "Example 133";
}

PGE_133::~PGE_133 ()
{
  for (std::vector<strand*>::iterator iterator = strands_.begin ();
       iterator != strands_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_133::OnUserCreate ()
{
  for (float angle = 0.0f; angle < 2.0f * static_cast<float> (M_PI); angle += 2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_133_DEFAULT_NUMBER_OF_HAIRS))
  {
    float x = olc::PixelGameEngine::ScreenWidth () / 2.0f + std::cos (angle) * 340.0f;
    float y = olc::PixelGameEngine::ScreenHeight () / 2.0f + std::sin (angle) * 340.0f;
    float hue = (angle / (2.0f * static_cast<float> (M_PI))) * 360.0f;
    strands_.push_back (new strand (x, y, hue));
  } // end FOR

  return true;
}

bool
PGE_133::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<strand*>::iterator iterator = strands_.begin ();
       iterator != strands_.end ();
       ++iterator)
  {
    (*iterator)->calcAngles (this);
    (*iterator)->calcPositions ();
    (*iterator)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
