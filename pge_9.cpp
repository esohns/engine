#include "stdafx.h"

#include "pge_9.h"

#include <algorithm>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_9::PGE_9 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 9";
}

PGE_9::~PGE_9 ()
{
}

void
PGE_9::dotPattern (int pattern_repetitions)
{
  // create the LEFT pattern and draw it also at the RIGHT side
  for (int x = 0; x < ScreenWidth() / pattern_repetitions; x++)
    for (int y = 0; y < ScreenHeight(); y++)
    {
      int color = rand() % 255; // (rand() % 2) * 255;
      Draw (x, y, olc::Pixel(color, color, color));
    } // end FOR
}

void
PGE_9::stampPattern (int pattern_repetitions)
{
  int skip_x = backgroundPattern->width;
  int skip_y = backgroundPattern->height;
  for (int y = 0; y < ScreenHeight(); y = y + skip_y)
    for (int x = 0; x < ScreenWidth() / pattern_repetitions; x = x + skip_x)
      DrawSprite (olc::vi2d (x, y), backgroundPattern);
}

void
PGE_9::addSprite (int pattern_repetitions, int inverse)
{
  int offset = 0;
  int pattern_width = ScreenWidth () / pattern_repetitions;
  olc::Pixel pix;
  olc::Pixel imagePix;

  for (int y = 0; y < ScreenHeight (); y++)
    for (int x = pattern_width; x < ScreenWidth (); x++)
    {
      imagePix = threeDImage->GetPixel (x, y);
      int greyscale = (imagePix.r + imagePix.g + imagePix.b) / 3;
      offset = greyscale / (255 / pattern_repetitions);

      int samplePixelPosition = (x - pattern_width);
      int shiftedSamplePosition = samplePixelPosition + (offset * inverse);
      pix = olc::PixelGameEngine::GetDrawTarget ()->GetPixel (shiftedSamplePosition, y);

      Draw (x, y, pix);
    } // end FOR
}

bool
PGE_9::OnUserCreate ()
{
  // Load the sprite
  threeDImage = new olc::Sprite ("./800x500.png");
  backgroundPattern = new olc::Sprite ("./background_pattern_tenth.png");

  dotPattern (10);
  addSprite (10, invert_display);

  return true;
}

bool
PGE_9::OnUserUpdate (float fElapsedTime)
{
  int updateNeeded = 0;

  if (GetMouse(1).bReleased)
  {
    updateNeeded = 1;
    invert_display *= -1;
  }

  if (GetMouse(0).bReleased)
  {
    updateNeeded = 1;
    if (state == DISPLAY_DOTS)
      state = DISPLAY_PATTERN;
    else
      state = DISPLAY_DOTS;
  }

  if (updateNeeded)
  {
    if (state == DISPLAY_DOTS)
    {
      dotPattern(10);
      addSprite (10, invert_display);
    } // end IF
    else
    {
      stampPattern (10);
      addSprite (10, invert_display);
    } // end ELSE
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
