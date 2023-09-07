#include "stdafx.h"

#include "pge_172.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_172::PGE_172 ()
 : olc::PixelGameEngine ()
 , C_ (0.0f)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 172");
}

bool
PGE_172::OnUserCreate ()
{
  return true;
}

bool
PGE_172::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  float f = Common_GL_Tools::map (std::cos (C_ * 0.05f), -1.0f, 1.0f, 1.0f, 40.0f);
  C_ += 0.03f;
  for (float j = C_; j < C_ + 2.0f * static_cast<float> (M_PI); j += 2.0f * static_cast<float> (M_PI) / 9.0f)
    for (float i = ENGINE_PGE_172_DEFAULT_W / 2.0f; i > 0.0f; i -= 3.0f)
    {
      float r = j + i / 5.0f;
      olc::vf2d pos (std::cos (r) * i, std::sin (r) * i);
      for (int32_t k = 0; k++ < 3;)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (noise_.GetValue (pos.x / static_cast<double> (ENGINE_PGE_172_DEFAULT_W),
          //Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (pos.x / static_cast<double> (ENGINE_PGE_172_DEFAULT_W),
                                                                     pos.y / static_cast<double> (ENGINE_PGE_172_DEFAULT_W),
                                                                     pos.mag () / static_cast<double> (ENGINE_PGE_172_DEFAULT_W) - C_ * 0.1)),
                                -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI) * f;
        float offset_x = std::cos (n) * 10.0f;
        float offset_y = std::sin (n) * 10.0f;
        olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (pos.x), half_height_i + static_cast<int32_t> (pos.y),
                                        half_width_i + static_cast<int32_t> (pos.x + offset_x), half_height_i + static_cast<int32_t> (pos.y + offset_y),
                                        olc::BLACK, 0xFFFFFFFF);
        pos.x += offset_x;
        pos.y += offset_y;
      } // end FOR
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
