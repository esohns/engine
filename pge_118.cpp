#include "stdafx.h"

#include "pge_118.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_118::PGE_118 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 118";
}

bool
PGE_118::OnUserCreate ()
{

  return true;
}

bool
PGE_118::OnUserUpdate (float fElapsedTime)
{
  static float milli_seconds_f = 0.0f;
  milli_seconds_f += fElapsedTime * 1000.0f;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_118_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_118_DEFAULT_ALPHA_DECAY : 0);

  harom (500.0f, 460.0f, 40.0f, 460.0f, 7, (std::sin (std::fmod (0.0005f * milli_seconds_f, 2.0f * static_cast<float> (M_PI))) + 1.0f) / 2.0f);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_118::harom (float ax, float ay, float bx, float by, int level, float ratio)
{
  if (level == 0) return;

  float vx, vy, nx, ny, cx, cy;
  vx = bx - ax;
  vy = by - ay;
  nx = std::cos (static_cast<float> (M_PI) / 3.0f) * vx - std::sin (static_cast<float> (M_PI) / 3.0f) * vy;
  ny = std::sin (static_cast<float> (M_PI) / 3.0f) * vx + std::cos (static_cast<float> (M_PI) / 3.0f) * vy;
  cx = ax + nx;
  cy = ay + ny;
  olc::PixelGameEngine::DrawLine (static_cast<int32_t> (ax), static_cast<int32_t> (ay), static_cast<int32_t> (bx), static_cast<int32_t> (by), olc::WHITE, 0xFFFFFFFF);
  olc::PixelGameEngine::DrawLine (static_cast<int32_t> (ax), static_cast<int32_t> (ay), static_cast<int32_t> (cx), static_cast<int32_t> (cy), olc::WHITE, 0xFFFFFFFF);
  olc::PixelGameEngine::DrawLine (static_cast<int32_t> (cx), static_cast<int32_t> (cy), static_cast<int32_t> (bx), static_cast<int32_t> (by), olc::WHITE, 0xFFFFFFFF);

  harom (ax * ratio + cx * (1 - ratio), ay * ratio + cy * (1 - ratio), ax * (1 - ratio) + bx * ratio, ay * (1 - ratio) + by * ratio, level - 1, ratio);
}
