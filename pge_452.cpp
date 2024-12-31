#include "stdafx.h"

#include "pge_452.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_21.h"

PGE_452::PGE_452 ()
 : olc::PixelGameEngine ()
 , objects_ ()
 , w_ (0.0f)
{
  sAppName = "Example 452";
}

bool
PGE_452::OnUserCreate ()
{
  w_ = olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_452_DEFAULT_OBJECTS_PER_COL_ROW);
  for (int i = 0; i < ENGINE_PGE_452_DEFAULT_OBJECTS_PER_COL_ROW; i++)
    for (int j = 0; j < ENGINE_PGE_452_DEFAULT_OBJECTS_PER_COL_ROW; j++)
      objects_.push_back (object (i * w_, j * w_, w_ / 2.0f));

  return true;
}

bool
PGE_452::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_452_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_452_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<object>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator).show (this);
    (*iterator).update ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
