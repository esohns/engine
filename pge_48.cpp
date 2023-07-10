#include "stdafx.h"

#include "pge_48.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_48::PGE_48 ()
 : olc::PixelGameEngine ()
 , sun_ (NULL)
 , end_ (NULL)
{
  sAppName = "Example 48";
}

PGE_48::~PGE_48 ()
{
  orbit* next = sun_;
  while (next->child_)
    next = next->child_;
  while (next)
  {
    orbit* parent = next->parent_;
    delete next;
    next = parent;
  } // end WHILE
}

bool
PGE_48::OnUserCreate ()
{
  sun_ = new orbit (olc::PixelGameEngine::ScreenWidth () / 2,
                    olc::PixelGameEngine::ScreenHeight () / 2 + 30,
                    olc::PixelGameEngine::ScreenWidth () / 5,
                    0,
                    NULL);
  orbit* next = sun_;
  for (int i = 0; i < ENGINE_PGE_48_DEFAULT_NUMBER_OF_ORBITS; i++)
    next = next->addChild ();
  end_ = next;
  ACE_ASSERT (end_);

  return true;
}

bool
PGE_48::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);

  std::vector<olc::vi2d> path_a;
  orbit* next;
  for (int i = 0; i < ENGINE_PGE_48_DEFAULT_RESOLUTION; i++)
  {
    next = sun_;
    while (next)
    {
      next->update ();
      next = next->child_;
    } // end WHILE
    path_a.push_back (end_->position_);
  } // end FOR

  //next = sun_;
  //while (next)
  //{
  //  next->show (this);
  //  next = next->child_;
  //} // end WHILE

  //static int32_t prev_x = 0;
  //static int32_t prev_y = 0;
  for (std::vector<olc::vi2d>::iterator iterator = path_a.begin ();
       iterator != path_a.end ();
       ++iterator)
  {
    olc::PixelGameEngine::Draw (*iterator, olc::MAGENTA);
    //prev_x = (iterator == path_a.begin ()) ? (*iterator).x : prev_x;
    //prev_y = (iterator == path_a.begin ()) ? (*iterator).y : prev_y;
    //olc::PixelGameEngine::DrawLine ({prev_x, prev_y}, *iterator, olc::MAGENTA, 0xFFFFFFFF);
    //prev_x = (*iterator).x;
    //prev_y = (*iterator).y;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
