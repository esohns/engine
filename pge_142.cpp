#include "stdafx.h"

#include "pge_142.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_142::PGE_142 ()
 : olc::PixelGameEngine ()
 , timer_ (0)
{
  sAppName = "Example 142";
}

bool
PGE_142::OnUserCreate ()
{
  return true;
}

bool
PGE_142::OnUserUpdate (float fElapsedTime)
{
  timer_ += 2;

  olc::Pixel color;
  for (int j = 0; j < ENGINE_PGE_142_DEFAULT_W; j++)
  {
    float y = Common_GL_Tools::map (static_cast<float> (j), 0.0f, static_cast<float> (ENGINE_PGE_142_DEFAULT_W - 1), -1.0f, 1.0f);
    for (int i = 0; i < ENGINE_PGE_142_DEFAULT_W; i++)
    {
      float x = Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_142_DEFAULT_W - 1), -1.0f, 1.0f);
      float d = ENGINE_PGE_142_DEFAULT_W / std::sqrt (x * x + y * y); // d=w/(x+y) : Tunnel-style
      float a = std::atan2 (y, x); // a=atan2(j,i) : Another tunnel-style
      int z = int (d * std::cos (a)) + timer_ ^ int (d * std::sin (a)) + timer_; // Instead of ^, & or | are also good
      color.r = z % 255;
      color.g = z * 2 % 255;
      color.b = z * 4 % 255;
      olc::PixelGameEngine::Draw (i, j, color);
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
