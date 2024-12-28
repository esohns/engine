#include "stdafx.h"

#include "pge_443.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_20.h"

PGE_443::PGE_443 ()
 : olc::PixelGameEngine ()
 , columns_ (ENGINE_PGE_443_DEFAULT_SIZE)
 , rows_ (ENGINE_PGE_443_DEFAULT_SIZE)
 , t_ (0.0f)
{
  sAppName = "Example 443";
}

bool
PGE_443::OnUserCreate ()
{
  return true;
}

bool
PGE_443::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_443_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_443_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < columns_; j++)
    {
      int32_t x =
        static_cast<int32_t> (i * (olc::PixelGameEngine::ScreenWidth () / static_cast<float> (columns_)) + ENGINE_PGE_443_DEFAULT_DIAMETER / 2.0f);
      int32_t y =
        static_cast<int32_t> (j * (olc::PixelGameEngine::ScreenHeight () / static_cast<float> (rows_)) + ENGINE_PGE_443_DEFAULT_DIAMETER / 2.0f);
      float patternValue = funPattern (myLog (i, -j), myAtan2 (i, -j), t_);
      int32_t r = static_cast<int32_t> (funRadius (patternValue));

      olc::Pixel color (0x26, 0x26, 0x26);
      if (patternValue < 0.0f)
      {
        float hue = Common_GL_Tools::map (patternValue, -1.0f, 0.0f, 0.0f, 360.0f);
        float r, g, b;
        Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
        color = olc::PixelF (r, g, b);
      } // end IF

      olc::PixelGameEngine::FillCircle (x, y, r, color);
    } // end FOR

  t_ += 0.03f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_443::funPattern (float x, float y, float t)
{
  return std::cos (std::cos (ENGINE_PGE_443_DEFAULT_SCALE * x - t) + 2.0f * std::cos (ENGINE_PGE_443_DEFAULT_SCALE * y - std::abs (std::sin (ENGINE_PGE_443_DEFAULT_SCALE * x - t))));
}

float
PGE_443::funRadius (float t)
{
  return ENGINE_PGE_443_DEFAULT_DIAMETER * std::abs (2.0f / (1.0f + std::exp (-5.0f * t)) - 1.0f);
}

float
PGE_443::myLog (int32_t x, int32_t y)
{
  x = x - ENGINE_PGE_443_DEFAULT_U;
  y = y - ENGINE_PGE_443_DEFAULT_V;

  if (x == 0 && y == 0)
    return 0.0f;
  
  return 6.0f * std::log (static_cast<float> (std::sqrt (x * x + y * y)));
}

float
PGE_443::myAtan2 (int32_t x, int32_t y)
{
  x = x - ENGINE_PGE_443_DEFAULT_U;
  y = y - ENGINE_PGE_443_DEFAULT_V;

  if (x == 0 && y == 0)
    return 0.0f;
  
  return 6.0f * static_cast<float> (std::atan2 (y, x));
}
