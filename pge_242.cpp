#include "stdafx.h"

#include "pge_242.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_242::PGE_242 ()
 : olc::PixelGameEngine ()
 , w_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 242");
}

bool
PGE_242::OnUserCreate ()
{
  w_ = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_242_DEFAULT_C;

  for (int i = 0; i < ENGINE_PGE_242_DEFAULT_C; i++)
    for (int j = 0; j < ENGINE_PGE_242_DEFAULT_C; j++)
    {
      float x = i * w_ + w_ / 2.0f;
      float y = j * w_ + w_ / 2.0f;
      objects_.push_back (object (x, y, w_));
    } // end FOR

  return true;
}

bool
PGE_242::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<object>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator).show (this);
    (*iterator).update ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
