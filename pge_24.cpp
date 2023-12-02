#include "stdafx.h"

#include "pge_24.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "engine_common.h"

//////////////////////////////////////////

PGE_24::PGE_24 ()
 : olc::PixelGameEngine ()
 , width_ (ENGINE_PGE_24_DEFAULT_CELL_SIZE)
 , columns_ (0)
 , rows_ (0)
 , grid_ ()
 , current_ (NULL)
 , stack_ ()
{
  sAppName = "Example 24";
}

bool
PGE_24::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  columns_ = olc::PixelGameEngine::ScreenWidth () / width_;
  rows_ = olc::PixelGameEngine::ScreenHeight () / width_;

  for (int j = 0; j < rows_; j++)
    for (int i = 0; i < columns_; i++)
      grid_.push_back (cell (i, j));

  current_ = &grid_[0];

  return true;
}

bool
PGE_24::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<cell>::iterator iterator = grid_.begin ();
       iterator != grid_.end ();
       ++iterator)
    (*iterator).show (this, width_);

  current_->visited_ = true;
  current_->highlight (this, width_);

  // STEP 1
  cell* next = current_->checkNeighbors (columns_,
                                         rows_,
                                         grid_);
  if (next)
  {
    next->visited_ = true;

    // STEP 2
    stack_.push (current_);

    // STEP 3
    removeWalls (*current_, *next);

    // STEP 4
    current_ = next;
  } // end IF
  else if (stack_.size () > 0)
  {
    current_ = stack_.top ();
    stack_.pop ();
  } // end ELSE IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_24::removeWalls (cell& a, cell& b)
{
  int x = a.i_ - b.i_;
  if (x == 1)
  {
    a.walls_[3] = false;
    b.walls_[1] = false;
  }
  else if (x == -1)
  {
    a.walls_[1] = false;
    b.walls_[3] = false;
  }

  int y = a.j_ - b.j_;
  if (y == 1)
  {
    a.walls_[0] = false;
    b.walls_[2] = false;
  }
  else if (y == -1)
  {
    a.walls_[2] = false;
    b.walls_[0] = false;
  }
}
