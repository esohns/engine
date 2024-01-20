#include "stdafx.h"

#include "pge_320.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_320::PGE_320 ()
 : olc::PixelGameEngine ()
 , n_ (10)
 , rev_ (1)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 320");
}

bool
PGE_320::OnUserCreate ()
{
  return true;
}

bool
PGE_320::OnUserUpdate (float fElapsedTime)
{
  static float w = 16.0f;
  static float h = 16.0f;
  static float dx = w / static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  static float dy = h / static_cast<float> (olc::PixelGameEngine::ScreenHeight ());

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  float x = -w / 2.0f;
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i++)
  {
    float y = -h / 2.0f;
    for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); j++)
    {
      float r = std::sqrt ((x * x) + (y * y));
      float theta = std::atan2 (y, x);
      float val = std::sin (n_ * std::cos (r) + 2.0f * n_ * theta);

      float r_2, g, b;
      float hue_f = Common_GL_Tools::map (val, -1.0f, 1.0f, 0.0f, 360.0f);
      Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r_2, g, b);
      olc::Pixel color (static_cast<uint8_t> (r_2 * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
      p[i + j * olc::PixelGameEngine::ScreenWidth ()] = color;

      y += dy;
    } // end FOR

    x += dx;
  } // end FOR

  if (n_ >= 20 || n_ <= -20)
    rev_ = rev_ * -1;
  n_ += rev_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
