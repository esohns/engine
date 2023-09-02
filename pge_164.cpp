#include "stdafx.h"

#include "pge_164.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_164::PGE_164 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 164";
}

bool
PGE_164::OnUserCreate ()
{
  return true;
}

bool
PGE_164::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float r1 = 0.0f;
  float r2 = 0.0f;
  float r3 = 0.0f;
  float t1 = Common_GL_Tools::map (std::sin ((static_cast<float> (M_PI) / 180.0f) * frame_count_i), -1.0f, 1.0f, 0.03f, 1.0f);
  float t2 = 1.0f;
  float t3 = static_cast<float> (M_PI) / 100.0f;

  static int32_t prev_x, prev_y;
  bool is_first_b = true;
  while (r1 < 360.0f)
  {
    float a = std::sin ((static_cast<float> (M_PI) / 180.0f) * r2) * half_height_i * std::cos ((static_cast<float> (M_PI) / 180.0f) * r3);
    float x = std::cos ((static_cast<float> (M_PI) / 180.0f) * r1) * a;
    float y = std::sin ((static_cast<float> (M_PI) / 180.0f) * r1) * a;
    if (is_first_b)
    {
      is_first_b = false;
      prev_x = static_cast<int32_t> (x);
      prev_y = static_cast<int32_t> (y);
    } // end IF
    olc::PixelGameEngine::DrawLine (half_width_i + prev_x, half_height_i + prev_y,
                                    half_width_i + static_cast<int32_t> (x), half_height_i + static_cast<int32_t> (y),
                                    olc::WHITE, 0xFFFFFFFF);
    r1 += t1;
    r2 += t2;
    r3 += t3;

    prev_x = static_cast<int32_t> (x);
    prev_y = static_cast<int32_t> (y);
  } // end WHILE

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
