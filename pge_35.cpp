#include "stdafx.h"

#include "pge_35.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_35::PGE_35 ()
 : olc::PixelGameEngine ()
 , columns_ (0)
 , rows_ (0)
 , grid_ (NULL)
 , next_ (NULL)
{
  sAppName = "Example 35";
}

PGE_35::~PGE_35 ()
{
  delete [] grid_;
  delete [] next_;
}

bool
PGE_35::OnUserCreate ()
{
  columns_ = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_35_DEFAULT_RESOLUTION;
  rows_ = olc::PixelGameEngine::ScreenHeight () / ENGINE_PGE_35_DEFAULT_RESOLUTION;
  grid_ = new int[columns_ * rows_];
  next_ = new int[columns_ * rows_];

  reset ();

  return true;
}

bool
PGE_35::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (int i = 0; i < columns_; i++)
    for (int j = 0; j < rows_; j++)
      if (grid_[j * columns_ + i])
      {
        int x = i * ENGINE_PGE_35_DEFAULT_RESOLUTION;
        int y = j * ENGINE_PGE_35_DEFAULT_RESOLUTION;
        olc::PixelGameEngine::FillRect (x, y, ENGINE_PGE_35_DEFAULT_RESOLUTION - 1, ENGINE_PGE_35_DEFAULT_RESOLUTION - 1, olc::BLACK);
      } // end IF

  for (int i = 0; i < columns_; i++)
    for (int j = 0; j < rows_; j++)
    {
      int state = grid_[j * columns_ + i];
      int neighbors = countNeighbors (i, j);

      if (!state && (neighbors == 3))
        next_[j * columns_ + i] = 1;
      else if (state && (neighbors < 2 || neighbors > 3))
        next_[j * columns_ + i] = 0;
      else
        next_[j * columns_ + i] = state;
    } // end FOR

  int* temp_p = grid_;
  grid_ = next_;
  next_ = temp_p;

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    int i =
      static_cast<int32_t> (olc::PixelGameEngine::GetMouseX () / static_cast<float> (ENGINE_PGE_35_DEFAULT_RESOLUTION));
    int j =
      static_cast<int32_t> (olc::PixelGameEngine::GetMouseY () / static_cast<float> (ENGINE_PGE_35_DEFAULT_RESOLUTION));
    grid_[j * columns_ + i] += 1;
  } // end IF
  if (olc::PixelGameEngine::GetMouse (1).bPressed)
    reset ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

int
PGE_35::countNeighbors (int x, int y)
{
  int sum = 0;

  for (int i = -1; i < 2; i++)
    for (int j = -1; j < 2; j++)
    {
      int col = (x + i + columns_) % columns_;
      int row = (y + j + rows_) % rows_;
      sum += grid_[row * columns_ + col];
    } // end FOR
  sum -= grid_[y * columns_ + x];

  return sum;
}

void
PGE_35::reset ()
{
  for (int i = 0; i < columns_; i++)
    for (int j = 0; j < rows_; j++)
      grid_[j * columns_ + i] = static_cast<int> (std::floor (Common_Tools::getRandomNumber (0.0f, 2.0f)));
  ACE_OS::memset (next_, 0, sizeof(int) * columns_ * rows_);
}
