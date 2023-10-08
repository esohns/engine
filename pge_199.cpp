#include "stdafx.h"

#include "pge_199.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_199::PGE_199 ()
 : olc::PixelGameEngine ()
 , rope_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 199");
}

bool
PGE_199::OnUserCreate ()
{
  node A (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2));
  node B (static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2), static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2 - 30));
  node C (static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2), static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2 + 30));
  node D (static_cast<float> (olc::PixelGameEngine::ScreenWidth ()), static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2));
  rope_.addNode (A);
  rope_.addNode (B);
  rope_.addNode (C);
  rope_.addNode (D);

  return true;
}

bool
PGE_199::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  rope_.resampleFB ();
  for (int i = 0; i < 2; i++)
  {
    rope_.relax ();
    rope_.limitLength ();
  } // end FOR
  rope_.show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
