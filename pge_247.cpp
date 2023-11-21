#include "stdafx.h"

#include "pge_247.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_247::PGE_247 ()
 : olc::PixelGameEngine ()
 , pad_ (0.0f)
 , spacing_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 247");
}

bool
PGE_247::OnUserCreate ()
{
  pad_ = ENGINE_PGE_247_DEFAULT_S / 12.0f;
  spacing_ =
    (ENGINE_PGE_247_DEFAULT_S - pad_ * 2.0f) / static_cast<float> (ENGINE_PGE_247_DEFAULT_N);

  return true;
}

bool
PGE_247::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float t =
    (frame_count_i % ENGINE_PGE_247_DEFAULT_LOOP_LENGTH) / static_cast<float> (ENGINE_PGE_247_DEFAULT_LOOP_LENGTH);
  float C =
    Common_GL_Tools::map (t, 0.0f, 1.0f, 0.0f, 2.0f * static_cast<float> (M_PI));

  for (int x = 0; x <= ENGINE_PGE_247_DEFAULT_N; x++)
    for (int y = 0; y <= ENGINE_PGE_247_DEFAULT_N; y++)
    {
      float x1 = half_width_i + ENGINE_PGE_247_DEFAULT_R * std::cos (C);
      float y1 = half_height_i + ENGINE_PGE_247_DEFAULT_R * std::sin (C);
      float x2 = half_width_i + ENGINE_PGE_247_DEFAULT_R * std::cos (C + static_cast<float> (M_PI));
      float y2 = half_height_i + ENGINE_PGE_247_DEFAULT_R * std::sin (C + static_cast<float> (M_PI));

      olc::vf2d position_s (x * spacing_ + pad_, y * spacing_ + pad_);
      float d = pDistance (position_s.x, position_s.y,
                           x1, y1, x2, y2);
      d /= ENGINE_PGE_247_DEFAULT_FACTOR;

      float radius_f = 15.0f + 15.0f * std::sin (d + C);
      radius_f /= 3.0f;
      olc::Pixel color;
      color.r = static_cast<uint8_t> (127.5f + 127.5f * std::sin (d + C));
      color.g = static_cast<uint8_t> (80.0f - 127.5f * std::cos (d * 0.75f + C));
      color.b = static_cast<uint8_t> (127.5f + 127.5f * std::cos (d * 0.15f + C));
      olc::PixelGameEngine::FillCircle (position_s,
                                        static_cast<int32_t> (radius_f), color);

      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x1), static_cast<int32_t> (y1),
                                      static_cast<int32_t> (x2), static_cast<int32_t> (y2),
                                      olc::RED, 0xFFFFFFFF);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_247::pDistance (float x, float y, float x1, float y1, float x2, float y2)
{
  float A = x - x1;
  float B = y - y1;
  float C = x2 - x1;
  float D = y2 - y1;

  float dot = A * C + B * D;
  float len_sq = C * C + D * D;
  float param = -1.0f;
  if (len_sq != 0.0f) //in case of 0 length line
    param = dot / len_sq;

  float xx, yy;

  if (param < 0.0f)
  {
    xx = x1;
    yy = y1;
  } // end IF
  else if (param > 1.0f)
  {
    xx = x2;
    yy = y2;
  } // end ELSE IF
  else
  {
    xx = x1 + param * C;
    yy = y1 + param * D;
  } // end ELSE

  float dx = x - xx;
  float dy = y - yy;

  return std::sqrt (dx * dx + dy * dy);
}
