#include "stdafx.h"

#include "pge_5.h"

#include <algorithm>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_5::PGE_5 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 5";
}

PGE_5::~PGE_5 ()
{
}

void
PGE_5::spawn (int numberOfFlakes_in)
{
  int color = 0;
  int x = 0;
  int y = 0;
  for (int i = 0; i < numberOfFlakes_in; i++)
  {
    color = 126 + rand () % 130;
    x = rand () % ScreenWidth ();
    y = rand () % 2;
    if (flakes[y][x] == 0)
      flakes[y][x] = color;
  } // end FOR
}

void
PGE_5::spawn (int x_in, int y_in)
{
  flakes[y_in][x_in] = 126 + rand () % 130;
}

void
PGE_5::move ()
{
  int direction = 0;
  for (int y = ScreenHeight () - 3; y >= 0; y--)
    for (int x = 0; x < ScreenWidth (); x++)
    {
      direction = (rand () % 3) - 1;
      if (flakes[y + 1][x + direction] == 0 && flakes[y][x] != 0)
      {
        flakes[y + 1][x + direction] = flakes[y][x];
        flakes[y][x] = 0;
      } // end IF
    } // end FOR
}

void
PGE_5::remove (int x_in, int y_in)
{
  int eraser_size = 5;
  int maxX = (x_in + eraser_size < 0 ? 0 : (x_in + eraser_size > ScreenWidth () ? ScreenWidth () : x_in + eraser_size));
  int maxY = (y_in + eraser_size < 0 ? 0 : (y_in + eraser_size > ScreenHeight () ? ScreenHeight () : y_in + eraser_size));
  int minX = (x_in - eraser_size < 0 ? 0 : (x_in - eraser_size > ScreenWidth () ? ScreenWidth () : x_in - eraser_size));
  int minY = (y_in - eraser_size < 0 ? 0 : (y_in - eraser_size > ScreenHeight () ? ScreenHeight () : y_in - eraser_size));

  for (int x = minX; x < maxX; x++)
    for (int y = minY; y < maxY; y++)
      flakes[y][x] = 0;
}

void
PGE_5::reset ()
{
  for (int x = 0; x < ScreenWidth (); x++)
    for (int y = 0; y < ScreenHeight (); y++)
      flakes[y][x] = 0;

  snow_fall_rate = 0;
}

bool
PGE_5::OnUserCreate ()
{
  spawn (25);
  return true;
}

bool
PGE_5::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    spawn (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());

  if (olc::PixelGameEngine::GetMouse (1).bHeld)
    remove (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());

  if (olc::PixelGameEngine::GetKey (olc::Key::UP).bPressed)
  {
    snow_fall_rate++;
    if (snow_fall_rate > max_snow_fall_rate)
      snow_fall_rate = max_snow_fall_rate;
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::DOWN).bPressed)
  {
    snow_fall_rate--;
    if (snow_fall_rate < 0)
      snow_fall_rate = 0;
  } // end IF

	if (olc::PixelGameEngine::GetKey (olc::Key::R).bPressed)
    reset ();

  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
      olc::PixelGameEngine::Draw (x, y, olc::Pixel (flakes[y][x], flakes[y][x], flakes[y][x]));

	// Move snow down, using elasped time for consitent behavior on faster hardware
	animate_elapsed += fElapsedTime;
	if (animate_elapsed > animate_rate)
  {
    animate_elapsed = animate_elapsed - animate_rate;
    spawn (snow_fall_rate); // Add some snowfall randomly
    move ();
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
