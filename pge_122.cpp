#include "stdafx.h"

#include "pge_122.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_122::PGE_122 ()
 : olc::PixelGameEngine ()
 , points_ ()
{
  sAppName = "Example 122";
}

bool
PGE_122::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int32_t r = 12; r < 200; r += 12)
    for (int32_t i = 0; i < r/2; i++)
    {
      struct point point_s;
      point_s.r_ = r;
      point_s.theta_ = Common_GL_Tools::map (static_cast<float> (i), 0.0f, r / 2.0f, 0.0f, 2.0f * static_cast<float> (M_PI));
      points_.push_back (point_s);
    } // end FOR

  return true;
}

bool
PGE_122::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  float f = frame_count_i / 25.0f;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_122_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_122_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<point>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
  {
    int32_t disp = Common_GL_Tools::map ((*iterator).r_, 100, 192, 70, 0);
    if ((*iterator).r_ < 100)
      disp = Common_GL_Tools::map ((*iterator).r_, 12, 100, 0, 70);
    float cost = std::cos ((*iterator).theta_);
    float sint = std::sin ((*iterator).theta_);
    float t = f + (*iterator).r_ / 70.0f;

    float val = std::cos (t) / 2.0f + 0.5f;
    float r = (*iterator).r_ + val * disp;
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (half_width_i + r * cost), static_cast<int32_t> (half_height_i + r * sint), 3, {255, 0, 0, 255});
    val = std::cos (t + 0.05f) / 2.0f + 0.5f;
    r = (*iterator).r_ + val * disp;
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (half_width_i + r * cost), static_cast<int32_t> (half_height_i + r * sint), 3, {0, 255, 0, 255});
    val = std::cos (t + 0.1f) / 2.0f + 0.5f;
    r = (*iterator).r_ + val * disp;
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (half_width_i + r * cost), static_cast<int32_t> (half_height_i + r * sint), 3, {0, 0, 255, 255});
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
