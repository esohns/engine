#include "stdafx.h"

#include "pge_131.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_131::PGE_131 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , w_ (0)
{
  sAppName = "Example 131";
}

bool
PGE_131::OnUserCreate ()
{
  w_ = std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ());

  return true;
}

bool
PGE_131::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static olc::Pixel color = {0, 255, 64, 255};

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_131_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_131_DEFAULT_ALPHA_DECAY : 0);

  float f = frame_count_i * 0.5f;
  for (int i = 1; i < w_ * 1.2f; i++)
  {
    float y = std::sqrt (i * 200.0f);
    float r = static_cast<float> (std::pow (i / static_cast<float> (w_), 4)) * i / 2.0f + 150.0f;
    float N =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (i * 0.1 - f, 0.0, 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 99.0f + f * 0.02f;
    //strokeWeight(i * 0.01);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (half_width_i + std::cos (N) * r),
                                    static_cast<int32_t> (           y + std::sin (N) * r),
                                    static_cast<int32_t> (half_width_i + std::cos (N + 0.2f) * r),
                                    static_cast<int32_t> (           y + std::sin (N + 0.2f) * r),
                                    color, 0xFFFFFFFF);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
