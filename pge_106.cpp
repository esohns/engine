#include "stdafx.h"

#include "pge_106.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_106::PGE_106 ()
 : olc::PixelGameEngine ()
 , objects_ ()
{
  sAppName = "Example 105";
}

PGE_106::~PGE_106 ()
{
  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_106::OnUserCreate ()
{
  //olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  int32_t c = ENGINE_PGE_106_DEFAULT_C;
  float w = olc::PixelGameEngine::ScreenWidth () / static_cast<float> (c);
  for (int32_t i = 0; i < c; i++)
    for (int32_t j = 0; j < c; j++)
    {
      float x = i * w + w / 2.0f;
      float y = j * w + w / 2.0f;
      objects_.push_back (new object (x, y, w / 2.0f));
    } // end FOR

  return true;
}

bool
PGE_106::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_105_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_105_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
    (*iterator)->run (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
