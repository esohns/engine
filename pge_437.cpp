#include "stdafx.h"

#include "pge_437.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_20.h"

PGE_437::PGE_437 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 437");
}

bool
PGE_437::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_437::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::Pixel (250, 250, 250, 255U));

  // space between rotations
  float v =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (frame_count_i / 60.0 / 30.0, 2.5, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 15.0f + 15.0f;
  // space scaling when further from center
  float e =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (frame_count_i / 60.0 / 5.0, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 0.8f + 1.0f;
  float ne =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (frame_count_i / 60.0 / 7.0, 1.5, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 0.8f + 1.0f;
  // steps per rotation
  float steps =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (frame_count_i / 60.0 / 120.0, 3.5, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 10.0f + 20.0f;
  // kind of spiral phase
  float gth = frame_count_i / 60.0f * (2.0f * static_cast<float> (M_PI) / 120.0f);
  
  static olc::Pixel color (0, 0, 0, 50);
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth ()  / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  for (float i = 0.0f; i < 2.0f * static_cast<float> (M_PI) * 50.0f; i += 2.0f * static_cast<float> (M_PI) / steps)
  {
    float r = v * std::pow (i, 1.0f / e);
    float x = r * std::cos (i);
    float y = r * std::sin (i);

    float ni = i + (2.0f * static_cast<float> (M_PI) / steps);
    float rr = v * std::pow (ni, 1.0f / ne);
    float nx = rr * std::cos (ni);
    float ny = rr * std::sin (ni);
    
    // rotate points around 0 by gth
    float rx = std::cos (gth) * x - std::sin (gth) * y;
    float ry = std::sin (gth) * x + std::cos (gth) * y;
    
    float rnx = std::cos (gth) * nx - std::sin (gth) * ny;
    float rny = std::sin (gth) * nx + std::cos (gth) * ny;

    olc::PixelGameEngine::FillRect (static_cast<int32_t> (rx + center_s.x), static_cast<int32_t> (ry + center_s.y),
                                    static_cast<int32_t> (rnx - rx), static_cast<int32_t> (rny - ry),
                                    color);
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (-rx + center_s.x), static_cast<int32_t> (-ry + center_s.y),
                                    static_cast<int32_t> (-rnx + rx), static_cast<int32_t> (-rny + ry),
                                    color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
