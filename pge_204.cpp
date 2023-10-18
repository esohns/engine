#include "stdafx.h"

#include "pge_204.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_204::PGE_204 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , strings_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 204");
}

bool
PGE_204::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  float rooty = olc::PixelGameEngine::ScreenHeight () * 0.025f;
  for (int32_t x = 20; x < olc::PixelGameEngine::ScreenWidth (); x += 30)
    strings_.push_back (string (static_cast<float> (x), rooty));

  return true;
}

bool
PGE_204::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::WHITE);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    for (std::vector<string>::iterator iterator = strings_.begin ();
         iterator != strings_.end ();
         ++iterator)
      (*iterator).applyRandomForce ();  
  } // end IF

  for (std::vector<string>::iterator iterator = strings_.begin ();
       iterator != strings_.end ();
       ++iterator)
  {
    (*iterator).update (this);
    if (Common_Tools::testRandomProbability (0.333333f))
      (*iterator).applyWind (&noise_, frame_count_i);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
