#include "stdafx.h"

#include "pge_69.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_69::PGE_69 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , w_ (0.0f)
{
  sAppName = "Example 69";
}

bool
PGE_69::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  w_ = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());

  olc::PixelGameEngine::Clear (olc::BLACK);

  return true;
}

bool
PGE_69::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  int pixels = GetDrawTargetWidth () * GetDrawTargetHeight ();
  olc::Pixel* p = GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_69_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_69_DEFAULT_ALPHA_DECAY : 0);

  float angle_f = (static_cast<float> (M_PI) + std::sin (frame_count_i * 0.02f)) / 7.0f;

  for (float j = 0.0f; j < static_cast<float> (M_PI) * 2.0f; j += static_cast<float> (M_PI) * 2.0f / 8.0f)
    tree (7, w_ / 2.0f, w_ / 2.0f, j, 45.0f, angle_f, frame_count_i);

  frame_count_i++;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_69::tree (int32_t step_in, float x_in, float y_in, float rad_in, float lengs_in, float angle_in, int frameCounter_in)
{
  if (step_in <= 0) return;

  int32_t inf = 20 - step_in; // influence of noise
  float n =
    static_cast<float> (Common_GL_Tools::map (noise_.GetValue ((x_in + frameCounter_in) / static_cast<double> (w_), (y_in - frameCounter_in) / static_cast<double> (w_), 0.0), -1.0, 1.0, 0.0, 1.0) * inf);
  float x = x_in + std::cos (rad_in) * lengs_in + std::cos (n) * inf;
  float y = y_in + std::sin (rad_in) * lengs_in + std::sin (n) * inf;
  olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x_in), static_cast<int32_t> (y_in),
                                  static_cast<int32_t> (x), static_cast<int32_t> (y),
                                  {255, 255, 255, 50}, 0xFFFFFFFF);
  step_in--;
  lengs_in *= 0.9f;
  tree (step_in, x, y, rad_in + angle_in, lengs_in, angle_in, frameCounter_in);
  tree (step_in, x, y, rad_in - angle_in, lengs_in, angle_in, frameCounter_in);
}
