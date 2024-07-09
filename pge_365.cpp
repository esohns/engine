#include "stdafx.h"

#include "pge_365.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_365::PGE_365 ()
 : olc::PixelGameEngine ()
 , whirls_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 365");
}

bool
PGE_365::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (float jter = olc::PixelGameEngine::ScreenHeight () * 0.2f; jter <= olc::PixelGameEngine::ScreenHeight (); jter += olc::PixelGameEngine::ScreenHeight () * 0.2f)
    for (float iter = olc::PixelGameEngine::ScreenWidth () * 0.15f; iter <= olc::PixelGameEngine::ScreenWidth (); iter += olc::PixelGameEngine::ScreenWidth () * 0.15f)
    {
      whirls_.push_back (whirl (iter, jter, 0.0f));
      whirls_.push_back (whirl (iter, jter, static_cast<float> (M_PI)));
    } // end FOR

  return true;
}

bool
PGE_365::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_365_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_365_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<whirl>::iterator iterator = whirls_.begin ();
       iterator != whirls_.end ();
       ++iterator)
    (*iterator).show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
