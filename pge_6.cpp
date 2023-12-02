#include "stdafx.h"

#include "pge_6.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_6::PGE_6 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 6";
}

PGE_6::~PGE_6 ()
{
}

void
PGE_6::initializePalette (struct fire_palette palette_in[])
{
  for (int a = 0; a < 64; a++)
  {
    palette_in[a].red = a * sizeof (ACE_UINT32);
    palette_in[a].green = 0;
    palette_in[a].blue = 0;

    palette_in[a + 64].red = 63 * sizeof (ACE_UINT32);
    palette_in[a + 64].green = a * sizeof (ACE_UINT32);
    palette_in[a + 64].blue = 0;

    palette_in[a + 128].red = 63 * sizeof (ACE_UINT32);
    palette_in[a + 128].green = 63 * sizeof (ACE_UINT32);
    palette_in[a + 128].blue = a * sizeof (ACE_UINT32);

    palette_in[a + 192].red = 63 * sizeof (ACE_UINT32);
    palette_in[a + 192].green = 63 * sizeof (ACE_UINT32);
    palette_in[a + 192].blue = 63 * sizeof (ACE_UINT32);
  } // end FOR
}

void
PGE_6::moveFire ()
{
  int n = 0;
  static int yPos = ((ScreenHeight() - 5) * ScreenWidth());

  for (int n2 = 0; n2 < numberOfFires; ++n2)
  {
    n = rand() % 100;
    if (n > 50)
      fire[n2] = fire[n2] + rand() % 7;
    else
      fire[n2] = fire[n2] - rand() % 7;
    if (fire[n2] > static_cast<ACE_UINT32> (ScreenWidth () - 11))
      fire[n2] = (2 * ScreenWidth () / 3) - rand () % (ScreenWidth () / 3);
    if (fire[n2] < 21)
      fire[n2] = (2 * ScreenWidth () / 3) + rand () % (ScreenWidth () / 3);

    // Left column
    fireBuffer[yPos + fire[n2] - 1 - ScreenWidth()] = 255;
    fireBuffer[yPos + fire[n2] - 1] = 255;
    fireBuffer[yPos + fire[n2] - 1 + ScreenWidth()] = 255;

    // Middle column
    fireBuffer[yPos + fire[n2] - ScreenWidth()] = 255;
    fireBuffer[yPos + fire[n2]] = 255;
    fireBuffer[yPos + fire[n2] + ScreenWidth()] = 255;

    // Right column
    fireBuffer[yPos + fire[n2] + 1 - ScreenWidth()] = 255;
    fireBuffer[yPos + fire[n2] + 1] = 255;
    fireBuffer[yPos + fire[n2] + 1 + ScreenWidth()] = 255;
  } // end FOR
}

void
PGE_6::moveUp ()
{
  ACE_OS::memcpy (fireBuffer, fireBuffer + WIDTH, HEIGHT_MINUS_BOTTOM_LINE);
}

void
PGE_6::blurScreen ()
{
  int yOffset = 0;
  int arrayOffset = 0;
  int startY = (HEIGHT / 2) * WIDTH;

  for (int x = 0; x < WIDTH; x++)
  {
    yOffset = startY;
    for (int y = HEIGHT / 2; y < HEIGHT; y++)
    {
      arrayOffset = yOffset + x;
      blurPixel (arrayOffset);
      yOffset = yOffset + ScreenWidth ();
    }
  }
}

void
PGE_6::blurPixel (int offset_in)
{
  int rgbCombined = 0;

  int offsetTop = offset_in - ScreenWidth ();
  int offsetBottom = offset_in + ScreenWidth ();

  // [L] [M] [R]
  // [L] [M] [R]
  // [L] [M] [R]

  // Middle column
  rgbCombined += getColor (offsetTop);    // x, y - 1;
  rgbCombined += getColor (offsetBottom); // x, y + 1;

  // Left column
  rgbCombined += getColor (offsetTop - 1);    // x - 1, y - 1;
  rgbCombined += getColor (offset_in - 1);    // x - 1, y;
  rgbCombined += getColor (offsetBottom - 1); // x - 1, y + 1;

  // Right column
  rgbCombined += getColor (offsetTop + 1);    // x + 1, y - 1;
  rgbCombined += getColor (offset_in + 1);    // x + 1, y;
  rgbCombined += getColor (offsetBottom + 1); // x + 1, y + 1);

  // average the colors
  rgbCombined = rgbCombined >> 3; // divide by 8

  fireBuffer[offset_in] = rgbCombined;
}

int
PGE_6::getColor (int offset_in)
{
  if ((offset_in < 0) || (offset_in >= SCREEN_SIZE))
    return 0;

  return fireBuffer[offset_in];
}

void
PGE_6::draw ()
{
  int place = 0;
  int yOffset = 0;

  for (int x = 0; x < ScreenWidth(); x++)
  {
    yOffset = 0;
    for (int y = 0; y < ScreenHeight(); y++)
    {
      place = yOffset + x;

      Draw (x, y, olc::Pixel (palette[fireBuffer[place]].red, palette[fireBuffer[place]].green, palette[fireBuffer[place]].blue));
      yOffset = yOffset + ScreenWidth ();
    } // end FOR
  } // end FOR
}

bool
PGE_6::OnUserCreate ()
{
  srand (static_cast<unsigned int> (time (NULL)));

  fireBuffer = new ACE_UINT32[SCREEN_SIZE];
  ACE_OS::memset (fireBuffer, 0, sizeof (ACE_UINT32) * SCREEN_SIZE);

  for (int i = 0; i < numberOfFires; ++i)
    fire[i] = (ScreenWidth() / 2) - numberOfFires + i;

  initializePalette (palette);

  return true;
}

bool
PGE_6::OnUserUpdate (float fElapsedTime)
{
  moveFire ();
  blurScreen ();
  moveUp ();

  draw ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
