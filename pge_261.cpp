#include "stdafx.h"

#include "pge_261.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_261::PGE_261 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 261");
}

bool
PGE_261::OnUserCreate ()
{
  return true;
}

bool
PGE_261::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float t = frame_count_i * ENGINE_PGE_261_DEFAULT_SPEED;
  olc::vf2d mouse_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                     static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  float noise_weight_f =
    Common_GL_Tools::map (mouse_s.x, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth ()), -100.0f, 100.0f);
  //float angle_f =
  //  Common_GL_Tools::map (mouse_s.y, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight ()), static_cast<float> (-M_PI), static_cast<float> (M_PI));
  //p.rotate(angle)
  drawNobo (ENGINE_PGE_261_DEFAULT_RADIUS, noise_weight_f, ENGINE_PGE_261_DEFAULT_AMOUNT, ENGINE_PGE_261_DEFAULT_CIRCLE_RADIUS, t);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_261::drawNobo (float radius, float noiseWeight, int32_t amount, int32_t circleSize, float t)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  bool is_first_b = true;
  olc::vf2d prev_s;
  for (float i = 0.0f; i < (2.0f * static_cast<float> (M_PI) / static_cast<float> (amount)) * (amount + 3); i += 2.0f * static_cast<float> (M_PI) / static_cast<float> (amount))
  {
    float x = std::cos (i) * radius;
    float y = std::sin (i) * radius;
    float nx =
      static_cast<float> (noise_.Evaluate (x + 100.0, y + 200.0, std::cos (t), std::sin (t)) * noiseWeight);
    float ny =
      static_cast<float> (noise_.Evaluate (x + 300.0, y + 400.0, std::cos (t), std::sin (t)) * noiseWeight);
    olc::vf2d position (x + nx, y + ny);

    if (is_first_b)
    {
      is_first_b = false;
      prev_s = position;
    } // end IF

    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
                                    half_width_i + static_cast<int32_t> (position.x), half_height_i + static_cast<int32_t> (position.y),
                                    olc::WHITE, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (position.x), half_height_i + static_cast<int32_t> (position.y),
                                      circleSize, olc::WHITE, 0xFF);

    prev_s = position;
  } // end FOR
}
