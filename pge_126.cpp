#include "stdafx.h"

#include "pge_126.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_126::PGE_126 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 126";
}

bool
PGE_126::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_126::OnUserUpdate (float fElapsedTime)
{
  static float elapsed_time_f = 0.0;
  elapsed_time_f += fElapsedTime * 1000.0f;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_126_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_126_DEFAULT_ALPHA_DECAY : 0);

  float t = elapsed_time_f / ENGINE_PGE_126_DEFAULT_F1;
  paintCircle (t, 0.0f);
  paintCircle (t, static_cast<float> (M_PI));

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_126::paintCircle (float t, float o)
{
  for (int i = 0; i < ENGINE_PGE_126_DEFAULT_N; i++)
  {
    float p = (i / static_cast<float> (ENGINE_PGE_126_DEFAULT_N)) * 2.0f * static_cast<float> (M_PI);

    int32_t x =
      static_cast<int32_t> ((olc::PixelGameEngine::ScreenWidth () / 2.0f) + std::sin (p + t) * (200.0f + std::sin ((t * ENGINE_PGE_126_DEFAULT_F2 + p) * ENGINE_PGE_126_DEFAULT_F3 + o) * ENGINE_PGE_126_DEFAULT_M * (1.0f - std::sin (p))));
    int32_t y =
      static_cast<int32_t> ((olc::PixelGameEngine::ScreenHeight () / 2.0f) + std::cos (p + t) * (200.0f + std::sin ((t * ENGINE_PGE_126_DEFAULT_F2 + p) * ENGINE_PGE_126_DEFAULT_F3 + o) * ENGINE_PGE_126_DEFAULT_M * (1.0f - std::sin (p))));

    olc::PixelGameEngine::DrawCircle (x, y, 3, olc::WHITE);
  } // end FOR
}
