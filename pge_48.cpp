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
  sun_ = new orbit (olc::PixelGameEngine::ScreenWidth () / 2.0,
                    olc::PixelGameEngine::ScreenHeight () / 2.0 + 30.0,
                    olc::PixelGameEngine::ScreenWidth () / 5.0,
                    0,
                    NULL);

  orbit* next = sun_;
  for (int i = 0; i < ENGINE_PGE_48_DEFAULT_NUMBER_OF_ORBITS; i++)
    next = next->addChild ();
  end_ = next;

  return true;
}

bool
PGE_48::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);

  std::vector<olc::vd2d> path_a;
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

  //static olc::vd2d prev = {0.0, 0.0};
  for (std::vector<olc::vd2d>::iterator iterator = path_a.begin ();
       iterator != path_a.end ();
       ++iterator)
  {
    olc::PixelGameEngine::Draw (*iterator, olc::MAGENTA);
    //prev.x = (iterator == path_a.begin ()) ? (*iterator).x : prev.x;
    //prev.y = (iterator == path_a.begin ()) ? (*iterator).y : prev.y;
    //olc::PixelGameEngine::DrawLine (prev, *iterator, olc::MAGENTA, 0xFFFFFFFF);
    //prev = *iterator;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
