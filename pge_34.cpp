#include "stdafx.h"

#include "pge_34.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_gl_tools.h"

#include "defines.h"
#include "engine_common.h"

PGE_34::PGE_34 ()
 : olc::PixelGameEngine ()
 , factor_ (0.0f)
 , r_ (0.0f)
{
  sAppName = "Example 34";
}

bool
PGE_34::OnUserCreate ()
{
  r_ = (olc::PixelGameEngine::ScreenHeight () / 2.0f) - 16.0f;

  return true;
}

bool
PGE_34::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  const int total = 200;
  factor_ += 0.015f;

  olc::PixelGameEngine::DrawCircle ({olc::PixelGameEngine::ScreenWidth () / 2, olc::PixelGameEngine::ScreenHeight () / 2}, static_cast<int32_t> (r_), olc::WHITE, 0xFF);
  olc::vd2d a, b;
  for (int i = 0; i < total; i++)
  {
    a = getVector (i, total);
    a += {static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2.0f), static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2.0f)};
    b = getVector (static_cast<int> (i * factor_), total);
    b += {static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2.0f), static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2.0f)};
    olc::PixelGameEngine::DrawLine (a, b, olc::WHITE, 0xFFFFFFFF);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::vf2d
PGE_34::getVector (int index_in, int total_in)
{
  float angle_f =
    Common_GL_Tools::map (static_cast<float> (index_in % total_in), 0.0f, static_cast<float> (total_in), 0.0f, static_cast<float> (M_PI) * 2.0f);
  angle_f += static_cast<float> (M_PI);
  olc::vf2d v = {cos (angle_f), sin (angle_f)};
  v *= r_;
  return v;
}