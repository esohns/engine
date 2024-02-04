#include "stdafx.h"

#include "pge_342.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_342::PGE_342 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , t_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 342");
}

bool
PGE_342::OnUserCreate ()
{
  return true;
}

bool
PGE_342::OnUserUpdate (float fElapsedTime)
{
  static olc::vi2d center_s (olc::PixelGameEngine::ScreenWidth () / 2,
                             olc::PixelGameEngine::ScreenHeight () / 2);

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a >= ENGINE_PGE_342_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_342_DEFAULT_ALPHA_DECAY : 0);

  int F = 1;
  for (float r = 0.0f; r < 2.0f * static_cast<float> (M_PI) * 70.0f; r += static_cast<float> (M_PI) / 48.0f)
    if (F++ % 19 < 11)
    {
      float S = std::sin (r - t_ / 64.0f);
      float r_2, g, b;
      Common_Image_Tools::HSVToRGB ((std::fmod (std::abs (r - static_cast<float> (t_)), 255.0f) / 255.0f) * 360.0f, 1.0f, 1.0f, r_2, g, b);
      olc::Pixel color (static_cast<uint8_t> (r_2 * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));

      float T = r - t_ / 64.0f;
      float A = r;
      float n =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue (r * 9.0f, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      float N = static_cast<float> (M_PI) * (n > 0.5f ? -1.0f : 1.0f);
      float R = T + static_cast<float> (M_PI) / 48.0f;
      float A_2 = A - N;
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (std::cos (T) * 2.0f * (A + N)) + center_s.x, static_cast<int32_t> (S * (A + N))        + center_s.y,
                                      static_cast<int32_t> (std::cos (R) * 2.0f * A_2)     + center_s.x, static_cast<int32_t> (std::sin (R) * A_2) + center_s.y,
                                      color, 0xFFFFFFFF);
    } // end IF

  ++t_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
