#include "stdafx.h"

#include "pge_145.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_145::PGE_145 ()
 : olc::PixelGameEngine ()
 , c_ (0.0f)
 , t_ (0.0f)
{
  sAppName = "Example 145";
}

bool
PGE_145::OnUserCreate ()
{
  c_ = ENGINE_PGE_145_DEFAULT_W / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_145::OnUserUpdate (float fElapsedTime)
{
  t_ += 0.05f;

  int i = 0;
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int Y = ENGINE_PGE_145_DEFAULT_W; Y--;)
    for (int X = ENGINE_PGE_145_DEFAULT_W; X--;)
    {
      float x = (X - c_) / 33.0f;
      float y = (Y - c_) / 33.0f;
      p[i].a = sin (x * x - y * y) > std::sin (2.0f * x * y + t_) ? 0 : 255;
      // Have a try to change the fomula. Here is an example.
      // sin(x*x+y*y)>sin(2*x*y+t)?0:255
      i++;
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
