#include "stdafx.h"

#include "pge_88.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_88::PGE_88 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = "Example 88";
}

bool
PGE_88::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  //olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_88::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_88_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_88_DEFAULT_ALPHA_DECAY : 0);
  //olc::PixelGameEngine::Clear (olc::WHITE);

  static int frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::Pixel color = olc::WHITE;
  int32_t offset = (frame_count_i / 80) % 1;
  for (int i = 1 + offset; i <= 42 + offset; i++)
  {
    if (i == 1)
      color.r = (255 - offset * 255);
    else if (i == 42)
      color.r = offset * 255;
    else
      color.r = 255;
    color.g = color.b = color.r;
    color.a = 255;

    int32_t size = 5 * i;
    float noisiness = 15.0f * static_cast<float> (std::sqrt (i));
    float k = static_cast<float> (std::sqrt (i)) / 10.0f;
    float t = frame_count_i / 300.0f - i / 50.0f;
    blob (size, half_width_i, half_height_i, k, t, noisiness, color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_88::blob (int32_t size, int32_t xCenter, int32_t yCenter, float k, float t, float noisiness, const olc::Pixel& color)
{
  static float angleStep = static_cast<float> (M_PI) / 8.0f;
  bool is_first_b = true;
  static int32_t prev_x, prev_y;
  for (float theta = 0.0f; theta <= static_cast<float> (M_PI) * 2.0f + 3.0f * angleStep; theta += angleStep)
  {
    float r1 = std::cos (theta) + 1.0f;
    float r2 = std::sin (theta) + 1.0f;
    float r =
      size + Common_GL_Tools::map (static_cast<float> (noise_.GetValue (k * r1, k * r2, t)), -1.0f, 1.0f, 0.0f, 1.0f) * noisiness;
    int32_t x = xCenter + static_cast<int32_t> (r * std::cos (theta));
    int32_t y = yCenter + static_cast<int32_t> (r * std::sin (theta));
    if (is_first_b)
    {
      prev_x = x;
      prev_y = y;
      is_first_b = false;
    } // end IF
    //olc::PixelGameEngine::Draw (x, y, color);
    olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, color, 0xFFFFFFFF);
    prev_x = x;
    prev_y = y;
  } // end FOR
}
