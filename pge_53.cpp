#include "stdafx.h"

#include "pge_53.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_53::PGE_53 ()
 : olc::PixelGameEngine ()
 , agents_ ()
{
  sAppName = "Example 53";
}

PGE_53::~PGE_53 ()
{
  for (std::vector<agent*>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_53::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_53_DEFAULT_NUMBER_OF_AGENTS; i++)
    agents_.push_back (new agent (this, {255, 255, 255, 255}));

  return true;
}

bool
PGE_53::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    for (std::vector<agent*>::iterator iterator = agents_.begin ();
         iterator != agents_.end ();
         ++iterator)
      delete *iterator;
    agents_.clear ();

    for (int i = 0; i < ENGINE_PGE_53_DEFAULT_NUMBER_OF_AGENTS; i++)
      agents_.push_back (new agent (this, {255, 255, 255, 255}));

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  int pixels = GetDrawTargetWidth () * GetDrawTargetHeight ();
  olc::Pixel* m = GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    m[i].a = (m[i].a > ENGINE_PGE_53_DEFAULT_ALPHA_DECAY ? m[i].a - ENGINE_PGE_53_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < 2; i++)
    for (std::vector<agent*>::iterator iterator = agents_.begin ();
         iterator != agents_.end ();
         ++iterator)
    {
      (*iterator)->updateDirection ((2.0 * M_PI) / 11.0, // ~30°
                                    this);
      (*iterator)->updatePosition (this);
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
