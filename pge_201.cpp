#include "stdafx.h"

#include "pge_201.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_201::PGE_201 ()
 : olc::PixelGameEngine ()
 , arcs_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 201");
}

bool
PGE_201::OnUserCreate ()
{

  return true;
}

bool
PGE_201::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  if (frame_count_i % 100 == 0)
    createArc ();

  olc::PixelGameEngine::Clear (olc::BLACK);

  std::vector<arc> deadArcs;
  for (std::vector<arc>::iterator iterator = arcs_.begin ();
       iterator != arcs_.end ();
       ++iterator)
  {
    (*iterator).act (this);

    if ((*iterator).length_ > olc::PixelGameEngine::ScreenWidth () * std::sqrt (2.0f))
      deadArcs.push_back (*iterator);
  } // end FOR

  for (std::vector<arc>::iterator iterator = deadArcs.begin ();
       iterator != deadArcs.end ();
       ++iterator)
  {
    std::vector<arc>::iterator iterator_2 = arcs_.begin ();
    for (;
         iterator_2 != arcs_.end ();
         ++iterator_2)
      if (*iterator_2 == *iterator)
        break;
    ACE_ASSERT (iterator_2 != arcs_.end ());
    arcs_.erase (iterator_2);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_201::createArc ()
{
  int start = 0;
  int end = 0;
  while (start < 360)
  {
    end = start + (Common_Tools::getRandomNumber (0, 45) + 5);
    arc a (static_cast<float> (start), static_cast<float> (end));
    arcs_.push_back (a);

    start = end + Common_Tools::getRandomNumber (0, 45);
  } // end WHILE
}
