#include "stdafx.h"

#include "pge_95.h"

#include "ace/Log_Msg.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "engine_common.h"

PGE_95::PGE_95 ()
 : olc::PixelGameEngine ()
 , cellWidth_ (static_cast<int32_t> (ENGINE_PGE_95_DEFAULT_CELL_SIZE * static_cast<float> (std::sqrt (2.0f))))
 , cellHeight_ (ENGINE_PGE_95_DEFAULT_CELL_SIZE)
 , thetaRate_ (1.0f / 240.0f)
 , moveRate_ (ENGINE_PGE_95_DEFAULT_CELL_SIZE * 0.04f)
 , origin_ (ENGINE_PGE_95_DEFAULT_WIDTH / 2.0f, ENGINE_PGE_95_DEFAULT_HEIGHT / 2.0f)
 , cells_ ()
{
  sAppName = "Example 95";
}

PGE_95::~PGE_95 ()
{
  for (std::vector<cell*>::iterator iterator = cells_.begin ();
      iterator != cells_.end ();
      ++iterator)
    delete *iterator;
}

bool
PGE_95::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  int num =
    static_cast<int> (std::floor (olc::PixelGameEngine::ScreenHeight () / (ENGINE_PGE_95_DEFAULT_CELL_SIZE * 4.0f)));
  for (int j = 0; j <= num; j++)
    for (int i = 0; i <= num; i++)
    {
      cell* cell_p = new cell (i, j, cellWidth_, cellHeight_, origin_);
      cell_p->show (this);
      cells_.push_back (cell_p);
    } // end FOR

  return true;
}

bool
PGE_95::OnUserUpdate (float fElapsedTime)
{
  for (std::vector<cell*>::iterator iterator = cells_.begin ();
      iterator != cells_.end ();
      ++iterator)
  {
    if (!(*iterator)->isDone ())
    {
      (*iterator)->update (cellWidth_, cellHeight_, thetaRate_, moveRate_);
      (*iterator)->show (this);
    } // end IF
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
