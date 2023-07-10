#include "stdafx.h"

#include "pge_47.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_47::PGE_47 ()
 : olc::PixelGameEngine ()
 , segments_ ()
{
  sAppName = "Example 47";
}

bool
PGE_47::OnUserCreate ()
{
  olc::vf2d a = {0, 200};
  olc::vf2d b = {600, 200};
  segment s1 (a, b);

  float len = a.dist (b);
  float h = len * std::sqrt (3.0f) / 2.0f;
  olc::vf2d c = {300, 200 + h};

  segment s2 (b, c);
  segment s3 (c, a);
  segments_.push_back (s1);
  segments_.push_back (s2);
  segments_.push_back (s3);

  return true;
}

bool
PGE_47::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    std::vector<segment> next;

    for (std::vector<segment>::iterator iterator = segments_.begin();
         iterator != segments_.end();
         ++iterator)
    {
      std::vector<segment> children = (*iterator).generate ();
      next.insert (next.end (), children.begin (), children.end ());
    } // end FOR

    segments_ = next;
  } // end IF

  for (std::vector<segment>::iterator iterator = segments_.begin ();
       iterator != segments_.end ();
       ++iterator)
    (*iterator).show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
