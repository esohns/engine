#include "stdafx.h"

#include "pge_105.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_105::PGE_105 ()
 : olc::PixelGameEngine ()
 , curveSize_ (0.0f)
 , shapes_ ()
{
  sAppName = "Example 105";
}

PGE_105::~PGE_105 ()
{
  for (std::vector<shape*>::iterator iterator = shapes_.begin ();
       iterator != shapes_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_105::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  curveSize_ =
    std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * 0.375f;

  for (int i = 0; i < ENGINE_PGE_105_DEFAULT_NUMBER_OF_SHAPES; i++)
    shapes_.push_back (new shape (curveSize_));

  return true;
}

bool
PGE_105::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_105_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_105_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<shape*>::iterator iterator = shapes_.begin ();
       iterator != shapes_.end ();
       ++iterator)
  {
    (*iterator)->update (this);
    (*iterator)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
