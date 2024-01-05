#include "stdafx.h"

#include "pge_301.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_301::PGE_301 ()
 : olc::PixelGameEngine ()
 , spots_ ()
 , rectangles_ ()
 , objects_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 301");
}

bool
PGE_301::OnUserCreate ()
{
  int w = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_301_DEFAULT_N;
  for (int j = 0; j < ENGINE_PGE_301_DEFAULT_N; j++)
    for (int i = 0; i < ENGINE_PGE_301_DEFAULT_N; i++)
    {
      int32_t x = i * w + w / 2;
      int32_t y = j * w + w / 2;
      int off = 0;
      if ((i + j) % 2 == 0)
        off = 0;

      struct spot spot_s;
      spot_s.position.x = static_cast<float> (x + off);
      spot_s.position.y = static_cast<float> (y + off);
      spot_s.d = w * 0.4f;
      spots_.push_back (spot_s);
    } // end FOR

  recRect (olc::PixelGameEngine::ScreenWidth () / 2, olc::PixelGameEngine::ScreenHeight () / 2,
           olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight (),
           ENGINE_PGE_301_DEFAULT_LEVEL, rectangles_);

  for (int i = 0; i < ENGINE_PGE_301_DEFAULT_N * ENGINE_PGE_301_DEFAULT_N; i++)
  {
    struct spot s = spots_[i];
    struct rectangle r = rectangles_[i];
    objects_.push_back (object (s.position, static_cast<int> (s.d), static_cast<int> (s.d), r.position, r.w, r.h, -int((s.position.x + s.position.y) / 14.0f)));
  } // end FOR

  return true;
}

bool
PGE_301::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<object>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator).show (this);
    (*iterator).move ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_301::recRect (int x, int y, int w, int h, int n, std::vector<struct rectangle>& arr)
{
  int ww = static_cast<int> (Common_Tools::getRandomNumber (0.0f, 1.0f) * w);
  int hh = static_cast<int> (Common_Tools::getRandomNumber (0.0f, 1.0f) * h);
  n--;

  if (n >= 0)
  {
    if (w < h)
    {
      recRect (x, y - (h / 2) + (hh / 2), w, hh, n, arr);
      recRect (x, y + (h / 2) - (h - hh) / 2, w, (h - hh), n, arr);
    } // end IF
    else
    {
      recRect (x - (w / 2) + (ww / 2), y, ww, h, n, arr);
      recRect (x + (w / 2) - (w - ww) / 2, y, (w - ww), h, n, arr);
    } // end ELSE
  } // end IF
  else
  {
    struct rectangle rectangle_s;
    rectangle_s.position.x = static_cast<float> (x);
    rectangle_s.position.y = static_cast<float> (y);
    rectangle_s.w = w;
    rectangle_s.h = h;
    arr.push_back (rectangle_s);
  } // end ELSE
}
