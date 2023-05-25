#include "stdafx.h"

#include "pge_26.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "engine_common.h"

//////////////////////////////////////////

PGE_26::PGE_26 ()
 : olc::PixelGameEngine ()
 , sandpile_ ()
 , next_ ()
 , defaultColor_ ({255, 0, 0, 255})
 , colors_ ()
{
  sAppName = "Example 26";

  colors_[0] = {255, 255, 0  , 255};
  colors_[1] = {0  , 185, 63 , 255};
  colors_[2] = {0  , 104, 255, 255};
  colors_[3] = {122, 0  , 229, 255};
}

void
PGE_26::topple ()
{
  next_ = sandpile_;

  for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); ++j)
    for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); ++i)
    {
      if (sandpile_[i][j] >= 4)
      {
        next_[i][j] -= 4;
        if (i + 1 < olc::PixelGameEngine::ScreenWidth ())
          next_[i + 1][j]++;
        if (i - 1 >= 0)
          next_[i - 1][j]++;
        if (j + 1 < olc::PixelGameEngine::ScreenHeight ())
          next_[i][j + 1]++;
        if (j - 1 >= 0)
          next_[i][j - 1]++;
      } // end IF
    } // end FOR

  sandpile_t tmp = sandpile_;
  sandpile_ = next_;
  next_ = tmp;
}

void
PGE_26::render ()
{
  for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); ++j)
    for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); ++i)
    {
      olc::Pixel col = defaultColor_;
      if (sandpile_[i][j] == 0)
        col = colors_[0];
      else if (sandpile_[i][j] == 1)
        col = colors_[1];
      else if (sandpile_[i][j] == 2)
        col = colors_[2];
      else if (sandpile_[i][j] == 3)
        col = colors_[3];

      olc::PixelGameEngine::Draw (i, j, col);
    } // end FOR
}

bool
PGE_26::OnUserCreate ()
{
  for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); ++j)
    for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); ++i)
    {
      sandpile_[i][j] = 0;
      next_[i][j] = 0;
    } // end FOR

  sandpile_[olc::PixelGameEngine::ScreenWidth () / 2][olc::PixelGameEngine::ScreenHeight () / 2] = 1000000000;

  return true;
}

bool
PGE_26::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);

  render ();

  for (int i = 0; i < 50; i++)
    topple ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
