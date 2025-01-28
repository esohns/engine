#include "stdafx.h"

#include "pge_463.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_21.h"

PGE_463::PGE_463 ()
 : olc::PixelGameEngine ()
 , rectangles_ ()
{
  sAppName = "Example 463";
}

bool
PGE_463::OnUserCreate ()
{
  initialize ();

  return true;
}

bool
PGE_463::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<rectangle>::iterator iterator = rectangles_.begin ();
       iterator != rectangles_.end ();
       ++iterator)
  {
    (*iterator).show (this);
    (*iterator).update ();
  } // end FOR

  if (frame_count_i % 70 == 0)
    initialize ();

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_463::divideRect (float x, float y, float w, float h, int32_t n)
{
  float ww = Common_Tools::getRandomNumber (0.2f, 0.8f) * w;
  float hh = Common_Tools::getRandomNumber (0.2f, 0.8f) * h;
  float xx = x - (w / 2.0f) + ww;
  float yy = y - (h / 2.0f) + hh;
  n -= Common_Tools::getRandomNumber (1, 3);
  if (n >= 0)
  {
    if (h < w)
    {
      divideRect (xx - ww / 2.0f, y, ww, h, n);
      divideRect (xx + (w - ww) / 2.0f, y, (w - ww), h, n);
    } // end IF
    else
    {
      divideRect(x, yy - hh / 2, w, hh, n);
      divideRect(x, yy + (h - hh) / 2, w, h - hh, n);
    } // end ELSE
  } // end IF
  else
    rectangles_.push_back (rectangle (x, y, static_cast<int32_t> (w), static_cast<int32_t> (h), this));
}

void
PGE_463::initialize ()
{
  rectangles_.clear ();
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f);
  divideRect (center_s.x, center_s.y, olc::PixelGameEngine::ScreenWidth () * 0.75f, olc::PixelGameEngine::ScreenHeight () * 0.75f,
              Common_Tools::getRandomNumber (5, 9));
}
