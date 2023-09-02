#include "stdafx.h"

#include "pge_166.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_166::PGE_166 ()
 : olc::PixelGameEngine ()
 , A_ (0.0f)
 , noise_ ()
 , t_ (0.0f)
{
  sAppName = "Example 166";
}

bool
PGE_166::OnUserCreate ()
{
  return true;
}

bool
PGE_166::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  t_ += 0.01f;
  A_ = (std::sin (t_ / 5.0f) * std::sin (t_ / 5.0f)) / 3.0f;
  for (int i = 12; i--;)
    T ((static_cast<float> (M_PI) / 6.0f) * i);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_166::T (float d, int32_t s, int32_t x, int32_t y, float l)
{
  if (s == 0) return;

  int32_t f = 20 - s;
  float n =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / static_cast<double> (ENGINE_PGE_166_DEFAULT_W) - t_,
                                                                y / static_cast<double> (ENGINE_PGE_166_DEFAULT_W) + t_,
                                                                0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * f;
  float offset_x = l * std::cos (d) + f * std::cos (n);
  float offset_y = l * std::sin (d) + f * std::sin (n);
  //strokeWeight(s * 2)
  olc::PixelGameEngine::DrawLine (x, y, x + static_cast<int32_t> (offset_x), y + static_cast<int32_t> (offset_y),
                                  olc::WHITE, 0xFFFFFFFF);
  x += static_cast<int32_t> (offset_x);
  y += static_cast<int32_t> (offset_y);

  T (d + A_, s - 1, x, y, l * 0.8f);
  T (d + -A_, s - 1, x, y, l * 0.8f);
}
