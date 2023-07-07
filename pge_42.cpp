#include "stdafx.h"

#include "pge_42.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_42::PGE_42 ()
 : olc::PixelGameEngine ()
 , grid_ (NULL)
 , x_ (0)
 , y_ (0)
 , direction_ (UP)
{
  sAppName = "Example 42";
}

PGE_42::~PGE_42 ()
{
  delete [] grid_;
}

bool
PGE_42::OnUserCreate ()
{
  grid_ = new int[olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ()];
  ACE_OS::memset (grid_, 0, sizeof (int) * olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ());

  x_ = olc::PixelGameEngine::ScreenWidth () / 2;
  y_ = olc::PixelGameEngine::ScreenHeight () / 2;

  return true;
}

bool
PGE_42::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    olc::PixelGameEngine::Clear (olc::BLACK);

  olc::Pixel color;
  for (int n = 0; n < ENGINE_PGE_42_DEFAULT_ITERATIONS_PER_CYCLE; n++)
  {
    color = olc::WHITE;
    if (grid_[y_ * olc::PixelGameEngine::ScreenWidth () + x_] == 0)
    {
      right ();
      grid_[y_ * olc::PixelGameEngine::ScreenWidth () + x_] = 1;
      color = olc::BLACK;
    } // end IF
    else if (grid_[y_ * olc::PixelGameEngine::ScreenWidth () + x_] == 1)
    {
      left ();
      grid_[y_ * olc::PixelGameEngine::ScreenWidth () + x_] = 0;
    } // end ELSE IF

    olc::PixelGameEngine::Draw (x_, y_, color);
    forwardAndEdges ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_42::right ()
{
  direction_++;
  if (direction_ > LEFT)
    direction_ = UP;
}

void
PGE_42::left ()
{
  direction_--;
  if (direction_ < UP)
    direction_ = LEFT;
}

void
PGE_42::forwardAndEdges ()
{
  if (direction_ == UP)
    y_--;
  else if (direction_ == RIGHT)
    x_++;
  else if (direction_ == DOWN)
    y_++;
  else if (direction_ == LEFT)
    x_--;

  if (x_ > olc::PixelGameEngine::ScreenWidth () - 1)
    x_ = 0;
  else if (x_ < 0)
    x_ = olc::PixelGameEngine::ScreenWidth () - 1;

  if (y_ > olc::PixelGameEngine::ScreenHeight () - 1)
    y_ = 0;
  else if (y_ < 0)
    y_ = olc::PixelGameEngine::ScreenHeight () - 1;
}
