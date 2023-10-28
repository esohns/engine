#include "stdafx.h"

#include "pge_215.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_215::PGE_215 ()
 : olc::PixelGameEngine ()
 , points1_ ()
 , points2_ ()
 , color1_ (200, 150, 0, 255) // yellow
 , color2_ (30, 80, 0, 255)   // green
 , white_ (olc::WHITE)
 , diameter_ (0.0f)
 , idx_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 215");
}

bool
PGE_215::OnUserCreate ()
{
  float refDim =
    std::min (static_cast<float> (olc::PixelGameEngine::ScreenWidth ()), static_cast<float> (olc::PixelGameEngine::ScreenHeight ()));
  float smallR = refDim / 8.0f;
  float bigR = refDim / 4.0f;

  points1_ = makeStar (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f,
                       bigR * (1.0f + ENGINE_PGE_215_DEFAULT_DELTA), smallR * (1.0f + ENGINE_PGE_215_DEFAULT_DELTA));
  points2_ = makeStar (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f,
                       bigR * (1.0f - ENGINE_PGE_215_DEFAULT_DELTA), smallR * (1.0f - ENGINE_PGE_215_DEFAULT_DELTA));

  diameter_ = ENGINE_PGE_215_DEFAULT_DELTA * bigR / 3.0f;

  return true;
}

bool
PGE_215::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear ({100, 0, 0, 255});

  float r, g, b;
  olc::Pixel color1, color2;
  Common_GL_Color_t rgb_color1, rgb_color2, lerped_color;
  for (int i = 0; i < ENGINE_PGE_215_DEFAULT_NUMBER_OF_POINTS; i++)
  {
    float pos = (i + 1) / static_cast<float> (ENGINE_PGE_215_DEFAULT_NUMBER_OF_POINTS);

    float hue = pos * 360.0f;
    Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
    rgb_color1.r = color1_.r;
    rgb_color1.g = color1_.g;
    rgb_color1.b = color1_.b;
    rgb_color2.r = static_cast<uint8_t> (r * 255.0f);
    rgb_color2.g = static_cast<uint8_t> (g * 255.0f);
    rgb_color2.b = static_cast<uint8_t> (b * 255.0f);
    lerped_color = Common_GL_Tools::lerpRGB (rgb_color1, rgb_color2, pos);
    color1.r = lerped_color.r;
    color1.g = lerped_color.g;
    color1.b = lerped_color.b;
    drawStarStep (points1_, static_cast<float> (std::fmod (idx_ + 0.05f * i, points1_.size ())), color1, white_, pos);
    hue = (idx_ / static_cast<float> (points1_.size ())) * 360.0f;
    Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
    rgb_color1.r = color2_.r;
    rgb_color1.g = color2_.g;
    rgb_color1.b = color2_.b;
    rgb_color2.r = static_cast<uint8_t> (r * 255.0f);
    rgb_color2.g = static_cast<uint8_t> (g * 255.0f);
    rgb_color2.b = static_cast<uint8_t> (b * 255.0f);
    lerped_color = Common_GL_Tools::lerpRGB (rgb_color1, rgb_color2, pos);
    color2.r = lerped_color.r;
    color2.g = lerped_color.g;
    color2.b = lerped_color.b;
    drawStarStep (points2_, static_cast<float> (std::fmod (idx_ + 0.05f * i, points2_.size ())), color2, white_, pos);
  } // end FOR

  idx_ = static_cast<float> (std::fmod (idx_ + 0.05f, points1_.size ()));

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<olc::vf2d>
PGE_215::makeStar (float x, float y, float bigR, float smallR)
{
  std::vector<olc::vf2d> result_a;

  for (int i = 0; i < 360; i += 36)
  {
    float r = i % (36 * 2) == 0 ? bigR : smallR;
    result_a.push_back (olc::vf2d (x + r * std::cos (-static_cast<float> (M_PI) / 10.0f + (static_cast<float> (M_PI) / 180.0f) * i),
                                   y + r * std::sin (-static_cast<float> (M_PI) / 10.0f + (static_cast<float> (M_PI) / 180.0f) * i)));
  } // end FOR

  return result_a;
}

void
PGE_215::drawStarStep (std::vector<olc::vf2d>& points, float idx, olc::Pixel& color1, olc::Pixel& color2, float pos)
{
  int i = static_cast<int> (std::floor (idx));
  float j = idx - i;

  int32_t x =
    static_cast<int32_t> (Common_Math_Tools::lerp (points[i].x, points[(i + 1) % points.size ()].x, j));
  int32_t y =
    static_cast<int32_t> (Common_Math_Tools::lerp (points[i].y, points[(i + 1) % points.size ()].y, j));

  Common_GL_Color_t rgb_color1_s, rgb_color2_s;
  rgb_color1_s.r = color1.r;
  rgb_color1_s.g = color1.g;
  rgb_color1_s.b = color1.b;
  rgb_color2_s.r = color2.r;
  rgb_color2_s.g = color2.g;
  rgb_color2_s.b = color2.b;
  Common_GL_Color_t lerped_color_s =
    Common_GL_Tools::lerpRGB (rgb_color1_s, rgb_color2_s, pos * pos * pos);
  olc::Pixel color (lerped_color_s.r, lerped_color_s.g, lerped_color_s.b, 255);
  olc::PixelGameEngine::FillCircle (x, y, static_cast<int32_t> (diameter_ * pos), color);
}
