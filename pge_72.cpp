#include "stdafx.h"

#include "pge_72.h"

#include "ace/Log_Msg.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_72::PGE_72 ()
 : olc::PixelGameEngine ()
 , f_ (0.0f)
{
  sAppName = "Example 72";
}

bool
PGE_72::OnUserCreate ()
{
  return true;
}

bool
PGE_72::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_72_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_72_DEFAULT_ALPHA_DECAY : 0);

  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight() / 2;

  int32_t g = 8;
  olc::Pixel color = olc::BLACK;
  while (g-- > 1)
  {
    color.g = static_cast<uint8_t> (36 * g);
    color.b = static_cast<uint8_t> (std::min (olc::PixelGameEngine::ScreenWidth (), 255));
    for (int32_t r = -half_height_i; r <= half_height_i; r++)
      vertex (r, g, color);
  } // end WHILE

  f_ += 0.5f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_72::vertex (int32_t t_in, int32_t k_in, const olc::Pixel& color_in)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Draw (half_width_i + t_in,
                              static_cast<int32_t> (half_height_i + 100.0f * (1.0f + std::cos (static_cast<float> (M_PI) * t_in / static_cast<float> (half_height_i))) * std::sin (static_cast<float> (M_PI) * (t_in - f_ * k_in) / static_cast<float> (half_height_i))),
                              color_in);
}
