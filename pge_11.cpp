#include "stdafx.h"

#include "pge_11.h"

#include <algorithm>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_11::PGE_11 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 11";
}

PGE_11::~PGE_11 ()
{
}

void
PGE_11::add (const olc::vf2d& pos, float r)
{
  struct ball b;
  b.pos = pos;
  olc::vf2d vel;
  vel.x = 1 + rand () % 10;
  vel.y = 1 + rand () % 10;
  vel.norm ();
  b.vel = vel * 10;
  b.radius = r;
  balls_.emplace_back (b);
}

//float
//PGE_11::map (float s, float a1, float a2, float b1, float b2)
//{
//  return b1 + (s-a1)*(b2-b1)/(a2-a1);
//}

bool
PGE_11::OnUserCreate ()
{
  srand (time (NULL));

  for (int i = 0; i < 7; i++)
  {
    olc::vf2d position_s;
    position_s.x = rand () % ScreenWidth ();
    position_s.y = rand () % ScreenHeight ();
    add (position_s, (float)(rand () % 30 + 2));
  } // end FOR
 
  return true;
}

bool
PGE_11::OnUserUpdate (float fElapsedTime)
{
  for (auto& ball : balls_)
  {
    ball.pos += ball.vel * fElapsedTime;

    if (ball.pos.x < 0 || ball.pos.x >= ScreenWidth ()) ball.vel.x *= -1;
    if (ball.pos.y < 0 || ball.pos.y >= ScreenHeight ()) ball.vel.y *= -1;
  } // end FOR

  for (int x = 0; x < ScreenWidth (); x++)
    for (int y = 0; y < ScreenHeight (); y++)
    {
      float sum = 0.0f;
      for (auto ball : balls_)
      {
        olc::vf2d point_s { (float)x, (float)y };
        sum += 200 * ball.radius / point_s.dist (ball.pos);
      } // end FOR
      Draw (x, y, olc::Colours::FromHsv (sum, 255, 255));
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
