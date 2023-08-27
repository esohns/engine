#include "stdafx.h"

#include "pge_152.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_152::PGE_152 ()
 : olc::PixelGameEngine ()
 , blackHoles_ ()
 , stars_ ()
{
  sAppName = "Example 152";
}

bool
PGE_152::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = ENGINE_PGE_152_DEFAULT_NUMBER_OF_HOLES; i--;)
    blackHoles_.push_back ({0.0f, 0.0f});

  for (int i = ENGINE_PGE_152_DEFAULT_NUMBER_OF_STARS; i--;)
    stars_.push_back (star (i, this));

  return true;
}

bool
PGE_152::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_152_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_152_DEFAULT_ALPHA_DECAY : 0);

  float t = frame_count_i / 1000.0f;
  for (int i = ENGINE_PGE_152_DEFAULT_NUMBER_OF_HOLES; i--;)
  {
    blackHoles_[i] = {
      std::cos (t * i) * i * 50 + olc::PixelGameEngine::ScreenWidth () / 2.0f,
      std::sin (t * i) * i * 50 + olc::PixelGameEngine::ScreenHeight () / 2.0f
    };

    olc::PixelGameEngine::DrawCircle (blackHoles_[i], 3, olc::RED, 0xFF);
  } // end FOR

  for (std::vector<star>::iterator iterator = stars_.begin ();
       iterator != stars_.end ();
       ++iterator)
    (*iterator).update (blackHoles_, this);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
