#include "stdafx.h"

#include "pge_343.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_343::PGE_343 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , objects_ ()
 , max_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 343");
}

bool
PGE_343::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  max_ =
    std::max (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * 0.8f;

  for (int i = 0; i < ENGINE_PGE_343_DEFAULT_NUMBER_OF_OBJECTS; i++)
    objects_.push_back (object ());

  return true;
}

bool
PGE_343::OnUserUpdate (float fElapsedTime)
{
//  olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a >= ENGINE_PGE_343_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_343_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<object>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator).update ();
    (*iterator).show (&noise_,
                      this,
                      max_);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
