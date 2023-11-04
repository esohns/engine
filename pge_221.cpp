#include "stdafx.h"

#include "pge_221.h"

#include "ace/Log_Msg.h"

#include "defines_10.h"
#include "engine_common.h"

PGE_221::PGE_221 ()
 : olc::PixelGameEngine ()
 , agents_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 221");
}

bool
PGE_221::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_221_DEFAULT_NUMBER_OF_AGENTS; i++)
    agents_.push_back (agent (this));

  return true;
}

bool
PGE_221::OnUserUpdate (float fElapsedTime)
{
  static float milli_seconds_f = 0.0f;
  milli_seconds_f += fElapsedTime * 1000.0f;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth() * olc::PixelGameEngine::GetDrawTargetHeight();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget()->GetData();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_221_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_221_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    (*iterator).plan (agents_, milli_seconds_f, this, &noise_);
  for (std::vector<agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    (*iterator).update ();

  for (std::vector<agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    (*iterator).draw (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
