#include "stdafx.h"

#include "pge_309.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_309::PGE_309 ()
 : olc::PixelGameEngine ()
 , angle_ (0.0f)
 , noise_ ()
 , nx_ (ENGINE_PGE_309_DEFAULT_NOISE_OFFSET_X)
 , ny_ (ENGINE_PGE_309_DEFAULT_NOISE_OFFSET_Y)
 , nz_ (ENGINE_PGE_309_DEFAULT_NOISE_OFFSET_Z)
 , c_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 309");
}

bool
PGE_309::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  angle_ = 1.0f / ENGINE_PGE_309_DEFAULT_NUMBER_OF_ANGLES * 2.0f * static_cast<float> (M_PI);

  return true;
}

bool
PGE_309::OnUserUpdate (float fElapsedTime)
{
  static olc::vi2d center_s (olc::PixelGameEngine::ScreenWidth () / 2,
                             olc::PixelGameEngine::ScreenHeight () / 2);

  for (int j = 0; j < ENGINE_PGE_309_DEFAULT_DRAWS_PER_FRAME; j++)
  {
    float x =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (nx_, 0.0, 0.0)), -1.0f, 1.0f, -static_cast<float> (olc::PixelGameEngine::ScreenWidth ()), static_cast<float> (olc::PixelGameEngine::ScreenWidth ()));
    float y =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (0.0, ny_, 0.0)), -1.0f, 1.0f, -static_cast<float> (olc::PixelGameEngine::ScreenHeight ()), static_cast<float> (olc::PixelGameEngine::ScreenHeight ()));
    float s =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (0.0, 0.0, nz_)), -1.0f, 1.0f, static_cast<float> (ENGINE_PGE_309_DEFAULT_MIN_SIZE), static_cast<float> (ENGINE_PGE_309_DEFAULT_MAX_SIZE));
    nx_ += ENGINE_PGE_309_DEFAULT_NOISE_INCREMENT;
    ny_ += ENGINE_PGE_309_DEFAULT_NOISE_INCREMENT;
    nz_ += ENGINE_PGE_309_DEFAULT_NOISE_INCREMENT * 4.0;

    olc::vf2d center (0.0f, 0.0f);
    olc::vf2d drawPoint (x, y);
    float cmAngle = std::atan2 (drawPoint.y - center.y, drawPoint.x - center.x);
    float magnitude = center.dist (drawPoint);

    for (int i = 0; i < ENGINE_PGE_309_DEFAULT_NUMBER_OF_ANGLES; i++)
    {
      float a = i * angle_ + cmAngle;
      olc::vf2d v (std::cos (a), std::sin (a));
      v *= magnitude;
      //v += center;

      olc::Pixel color = olc::BLACK;
      color.a = static_cast<uint8_t> (0.05f * 255.0f);
      olc::PixelGameEngine::FillCircle (center_s.x + static_cast<int32_t> (v.x) + 3,
                                        center_s.y + static_cast<int32_t> (v.y) + 3,
                                        static_cast<int32_t> (s / 2.0f),
                                        color);

      float r, g, b;
      Common_Image_Tools::HSVToRGB (static_cast<float> (c_), 64 / 100.0f, 1.0f, r, g, b);
      color.r = static_cast<uint8_t> (r * 255.0f);
      color.g = static_cast<uint8_t> (g * 255.0f);
      color.b = static_cast<uint8_t> (b * 255.0f);
      color.a = static_cast<uint8_t> (0.1f * 255.0f);
      olc::PixelGameEngine::FillCircle (center_s.x + static_cast<int32_t> (v.x),
                                        center_s.y + static_cast<int32_t> (v.y),
                                        static_cast<int32_t> (s / 2.0f),
                                        color);
    } // end FOR

    c_ = (c_ + 1) % 360;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
