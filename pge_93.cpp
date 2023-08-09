#include "stdafx.h"

#include "pge_93.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_93::PGE_93 ()
 : olc::PixelGameEngine ()
 , mouse_ ()
 , rad_ (ENGINE_PGE_93_DEFAULT_RAD)
{
  sAppName = "Example 93";
}

bool
PGE_93::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_93::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_93_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_93_DEFAULT_ALPHA_DECAY : 0);

  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static int32_t prev_x, prev_y;
  bool is_first_b = true;

  mouse_.x =
    Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), -1000.0f, 1000.0f);
  mouse_.y =
    Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseY ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), -50.0f, 50.0f);

  for (float i = 0.0f; i <= 360.0f; i += 0.1f)
  {
    float angle = i * static_cast<float> (M_PI) / 180.0f;
    float xOffsetRad = std::tan (angle) + 1.0f;
    float yOffsetRad = std::tan (angle) + 1.0f;
    int32_t x =
      half_width_i + static_cast<int32_t> (((rad_ * std::cos (xOffsetRad * mouse_.x)) * (std::cos (xOffsetRad * mouse_.x))) * std::cos (angle));
    int32_t y =
      half_height_i + static_cast<int32_t> (((rad_ * std::cos (yOffsetRad * mouse_.y)) * (std::cos (yOffsetRad * mouse_.y))) * std::sin (angle));
    olc::PixelGameEngine::DrawCircle (x, y, 3, {255, 255, 255, 100});
    if (is_first_b)
    {
      prev_x = x;
      prev_y = y;
      is_first_b = false;
    } // end IF
    //olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, {255, 255, 255, 100}, 0xFFFFFFFF);
    //olc::PixelGameEngine::Draw (x, y, { 255, 255, 255, 100 });
    prev_x = x;
    prev_y = y;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
