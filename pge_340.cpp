#include "stdafx.h"

#include "pge_340.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_340::PGE_340 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , t_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 340");
}

bool
PGE_340::OnUserCreate ()
{
  return true;
}

bool
PGE_340::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a >= ENGINE_PGE_340_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_340_DEFAULT_ALPHA_DECAY : 0);

  float A = 5.0f;
  int P = 5;
  static int V = 460;
  static int H = 360;
  for (float d = 0.0f; d < static_cast<float> (ENGINE_PGE_340_DEFAULT_W + P); d += A)
  {
    for (float r = 0; r < 2.0f * static_cast<float> (M_PI); r += static_cast<float> (M_PI) / 38.0f)
    {
      float r_2, g, b;
      Common_Image_Tools::HSVToRGB ((std::fmod (std::abs (t_) + r * 40.0f, 255.0f) / 255.0f) * 360.0f, 1.0f, 200.0f / 255.0f, r_2, g, b);
      olc::Pixel color (static_cast<uint8_t> (r_2 * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));

      float E = A * (-t_ % 24) / 24.0f + d;
      float n =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue (r * 99.0f, P + int (t_ / 24.0f), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      float N = (n > 0.5f ? A / 2.0f : -A / 2.0f);
      float D = E + N;
      float R = r + static_cast<float> (M_PI) / 38.0f;
      float D_2 = E - N;
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (std::cos (r) * D)   + V, static_cast<int32_t> (std::sin (r) * D)   + H,
                                      static_cast<int32_t> (std::cos (R) * D_2) + V, static_cast<int32_t> (std::sin (R) * D_2) + H,
                                      color, 0xFFFFFFFF);
    } // end FOR

    P++;
    A *= 1.05f;
  } // end FOR

  --t_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
