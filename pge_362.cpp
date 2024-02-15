#include "stdafx.h"

#include "pge_362.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_362::PGE_362 ()
 : olc::PixelGameEngine ()
 , dots_ ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 362");
}

bool
PGE_362::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  dots_.clear ();

  int countLines = 0;
  float xSep = ENGINE_PGE_362_DEFAULT_DOT_SIZE * 2.0f;
  float ySep = xSep * std::sqrt (3.0f) / 2.0f;
  float x = 0.0f;
  float y = -ySep / 4.0f;
  while (y < static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) + ENGINE_PGE_362_DEFAULT_DOT_SIZE)
  {
    x = (countLines % 2 == 0) ? xSep / 4.0f : -xSep / 4.0f;

    while (x < static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) + ENGINE_PGE_362_DEFAULT_DOT_SIZE)
    {
      dots_.push_back (dot (x, y, this));
      x += xSep;
    } // end WHILE

    countLines++;
    y += ySep;
  } // end WHILE

  return true;
}

bool
PGE_362::OnUserUpdate (float fElapsedTime)
{
  //float r, g, b;
  //Common_Image_Tools::HSVToRGB (0.6f * 360, 0.75f, 0.15f, r, g, b);
  //olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 255);
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_362_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_362_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<dot>::iterator iterator = dots_.begin ();
       iterator != dots_.end ();
       ++iterator)
    (*iterator).show (t_, this);

  t_ += 1.5f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
