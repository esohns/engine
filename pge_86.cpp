#include "stdafx.h"

#include "pge_86.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_86::PGE_86 ()
 : olc::PixelGameEngine ()
 , val_ (static_cast<float> (M_PI) / 5.0f)
{
  sAppName = "Example 86";
}

bool
PGE_86::OnUserCreate ()
{
  return true;
}

bool
PGE_86::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_86_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_86_DEFAULT_ALPHA_DECAY : 0);

  static int frame_count_i = 1;

  for (int k = 0; k < ENGINE_PGE_86_DEFAULT_R; k++)
    draw (k, frame_count_i);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_86::draw (int32_t k_in, int frameCount_in)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::Pixel color_s;
  Common_GL_Color_t color1, color2, lerped_color;
  color1.r = 0xD7;
  color1.g = 0xDA;
  color1.b = 0xE5;
  color2.r = 0x49;
  color2.g = 0x49;
  color2.b = 0x47;
  lerped_color =
    Common_GL_Tools::lerpRGB (color1, color2, k_in / static_cast<float> (ENGINE_PGE_86_DEFAULT_R));
  color_s.r = lerped_color.r;
  color_s.g = lerped_color.g;
  color_s.b = lerped_color.b;

  float outerR = (val_ * k_in + 1) * ENGINE_PGE_86_DEFAULT_R;
  int32_t n =
    static_cast<int32_t> (std::floor (static_cast<float> (M_PI) / std::asin (ENGINE_PGE_86_DEFAULT_R / outerR)));
  for (int i = 0; i < n; i++)
  {
    float x = outerR * std::cos ((i * 2.0f * static_cast<float> (M_PI)) / n - frameCount_in / 120.0f + k_in / 12.0f);
    float y = outerR * std::sin ((i * 2.0f * static_cast<float> (M_PI)) / n - std::sin (frameCount_in / 120.0f));
    olc::PixelGameEngine::FillCircle (half_width_i + static_cast<int32_t> (x), half_height_i + static_cast<int32_t> (y),
                                      static_cast<int32_t> ((ENGINE_PGE_86_DEFAULT_R * k_in) / (15.0f - 0.0f)),
                                      color_s);
  } // end FOR
}
