#include "stdafx.h"

#include "pge_7.h"

#include <algorithm>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_7::PGE_7 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 7";
}

PGE_7::~PGE_7 ()
{
}

void
PGE_7::spawn (int numberOfDrops_in)
{
  int color = 0;
  int x = 0;
  int y = 0;
  for (int i = 0; i < numberOfDrops_in; i++)
  {
    color = 126 + rand () % 130;
    x = rand () % ScreenWidth ();
    y = rand () % 2;
    if (drops[y][x] == 0)
      drops[y][x] = color;
  } // end FOR
}

void
PGE_7::spawn (int x_in, int y_in)
{
  drops[y_in][x_in] = 126 + rand () % 130;
}

void
PGE_7::moveDrop (int y, int x)
{
  if (drops[y][x] == 0 || drops[y][x] == 100)
    return;

  // If the spot directly below is empty, move there first.
  if (drops[y + 1][x] == 0)
  {
    drops[y + 1][x] = drops[y][x];
    drops[y][x] = 0;
  }
  else if (drops[y + 1][x + 1] == 0)
  {
    drops[y + 1][x + 1] = drops[y][x];
    drops[y][x] = 0;
  }
  else if (drops[y + 1][x - 1] == 0)
  {
    drops[y + 1][x - 1] = drops[y][x];
    drops[y][x] = 0;
  }
  else
  {
    // Lets see if we can move randomly out left/right from the spot up to 5 max places if there is nothing in the way.
    int spread = (rand() % 5) + 1;

    for (int i = 1; i < spread; i++)
    {
      // Edge of screen, can't move
      if (x + i < 1 || x + i > ScreenWidth() - 1)
        return;

      // "Wall" can't move
      if (drops[y][x + i] == 1)
        return;

      if (drops[y][x + i] == 0)
      {
        // found a spot, move me
        drops[y][x + i] = drops[y][x];
        drops[y][x] = 0;
        return;
      }
    } // end FOR

    for (int i = -1; i > -spread; i--)
    {
      // Edge of screen, can't move
      if (x + i < 1 || x + i > ScreenWidth() - 1)
        return;

      // "Wall" can't move
      if (drops[y][x + i] == 1)
        return;

      if (drops[y][x + i] == 0)
      {
        // found a spot, move me
        drops[y][x + i] = drops[y][x];
        drops[y][x] = 0;
        return;
      }
    } // end FOR
  } // end ELSE
}

void
PGE_7::leftToRight ()
{
  for (int y = ScreenHeight () - 3; y >= 0; y--)
    for (int x = 1; x < ScreenWidth () - 2; x++)
      moveDrop (y, x);
}

void
PGE_7::rightToLeft ()
{
  for (int y = ScreenHeight () - 3; y >= 0; y--)
    for (int x = ScreenWidth () - 2; x > 1; x--)
      moveDrop (y, x);
}

void
PGE_7::move ()
{
  int direction = rand() % 2;
  if (direction == 1)
    rightToLeft ();
  else
    leftToRight ();
}

void
PGE_7::remove (int x_in, int y_in)
{
  int eraser_size = 5;
  int maxX = (x_in + eraser_size < 0 ? 0 : (x_in + eraser_size > ScreenWidth () ? ScreenWidth () : x_in + eraser_size));
  int maxY = (y_in + eraser_size < 0 ? 0 : (y_in + eraser_size > ScreenHeight () ? ScreenHeight () : y_in + eraser_size));
  int minX = (x_in - eraser_size < 0 ? 0 : (x_in - eraser_size > ScreenWidth () ? ScreenWidth () : x_in - eraser_size));
  int minY = (y_in - eraser_size < 0 ? 0 : (y_in - eraser_size > ScreenHeight () ? ScreenHeight () : y_in - eraser_size));

  for (int x = minX; x < maxX; x++)
    for (int y = minY; y < maxY; y++)
      drops[y][x] = 0;
}

void
PGE_7::reset ()
{
  for (int x = 0; x < ScreenWidth (); x++)
    for (int y = 0; y < ScreenHeight (); y++)
      drops[y][x] = 0;

  water_fall_rate = 0;
}

bool
PGE_7::OnUserCreate ()
{
  spawn (25);
  return true;
}

bool
PGE_7::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse(0).bHeld)
    spawn (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());

  if (olc::PixelGameEngine::GetMouse(1).bHeld)
    remove (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());

  if (olc::PixelGameEngine::GetKey (olc::Key::UP).bPressed)
  {
    water_fall_rate++;
    if (water_fall_rate > max_water_fall_rate)
      water_fall_rate = max_water_fall_rate;
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::DOWN).bPressed)
  {
    water_fall_rate--;
    if (water_fall_rate < 0)
      water_fall_rate = 0;
  } // end IF

	if (olc::PixelGameEngine::GetKey (olc::Key::R).bPressed)
    reset ();

  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
      olc::PixelGameEngine::Draw (x, y, olc::Pixel (drops[y][x], drops[y][x], drops[y][x]));

	// Move snow down, using elasped time for consitent behavior on faster hardware
	animate_elapsed += fElapsedTime;
	if (animate_elapsed > animate_rate)
  {
    animate_elapsed = animate_elapsed - animate_rate;
    spawn (water_fall_rate); // Add some snowfall randomly
    move ();
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
