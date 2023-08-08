#include "stdafx.h"

#include "pge_92.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_92::PGE_92 ()
 : olc::PixelGameEngine ()
 , noiseOffsetX_ (0.0)
 , noiseOffsetY_ (0.0)
 , startColor_ ()
 , endColor_ ()
 , noise_ ()
{
  sAppName = "Example 92";
}

bool
PGE_92::OnUserCreate ()
{
  startColor_.r = 0x6D;
  startColor_.g = 0xB4;
  startColor_.b = 0xC8;
  endColor_.r = 0xD6;
  endColor_.g = 0xE2;
  endColor_.b = 0xE4;

  return true;
}

bool
PGE_92::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  static int frame_count_i = 1;
  static int32_t prev_x, prev_y;

  Common_GL_Color_t lerped_color;
  olc::Pixel color_s;
  int32_t pointsInLine =
    static_cast<int32_t> (5.0f * (olc::PixelGameEngine::ScreenWidth () / 100.0f) * olc::PixelGameEngine::ScreenWidth () / 1000.0f);
  bool is_first_b;

  for (int i = 0; i < ENGINE_PGE_92_DEFAULT_NUMBER_OF_LINES; i++)
  {
    float yStep = i / static_cast<float> (ENGINE_PGE_92_DEFAULT_NUMBER_OF_LINES);
    lerped_color = Common_GL_Tools::lerpRGB (startColor_, endColor_, 1.0f - yStep);
    color_s.r = lerped_color.r;
    color_s.g = lerped_color.g;
    color_s.b = lerped_color.b;
    is_first_b = true;
    for (int j = 0; j <= pointsInLine; j++)
    {
      float xStep = j / static_cast<float> (pointsInLine);
      double noiseX = xStep * ENGINE_PGE_92_DEFAULT_NOISE_SCALE + frame_count_i * 0.001f;
      double noiseY = yStep * ENGINE_PGE_92_DEFAULT_NOISE_SCALE;
      float offset_f = static_cast<float> (noise_.GetValue (noiseX, noiseY, 0.0));
      float animationOffset = std::sin ((frame_count_i + offset_f * 1000.0f) * 0.01f) * 10.0f;
      float sdfVal =
        1.0f - sdfValue (xStep * olc::PixelGameEngine::ScreenWidth (), yStep * olc::PixelGameEngine::ScreenHeight ());
      int32_t x = static_cast<int32_t> (xStep * olc::PixelGameEngine::ScreenWidth ());
      int32_t y = static_cast<int32_t> (yStep * olc::PixelGameEngine::ScreenHeight () + animationOffset * sdfVal);
      if (is_first_b)
      {
        prev_x = x;
        prev_y = y;
        is_first_b = false;
      } // end IF
      olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, color_s, 0xFFFFFFFF);
      prev_x = x; prev_y = y;
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_92::sdfValue (float x, float y)
{
  float dX = x - olc::PixelGameEngine::GetMouseX ();
  float dY = y - olc::PixelGameEngine::GetMouseY ();
  float sqrDist = dX * dX + dY * dY;
  return std::max (std::min (100 * 100 / sqrDist, 1.0f), 0.0f);
}
