#include "stdafx.h"

#include "pge_324.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_324::PGE_324 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 324");
}

bool
PGE_324::OnUserCreate ()
{
  return true;
}

bool
PGE_324::OnUserUpdate (float fElapsedTime)
{
  static float elapsed_time_f = 0.0f;
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  olc::PixelGameEngine::Clear (olc::BLACK);

  int count = 
    static_cast<int> (Common_GL_Tools::map (std::sin ((elapsed_time_f * 1000.0f) / 500.0f), -1.0f, 1.0f, 64.0f, 1024.0f));
  float r, r_2, g, b;
  for (int i = 0; i < count; i++)
  {
    r =
      ENGINE_PGE_324_DEFAULT_RADIUS * Common_GL_Tools::map (std::sin ((i / static_cast<float> (count)) * ENGINE_PGE_324_DEFAULT_WAVES * static_cast<float> (M_PI)), -1.0f, 1.0f, Common_GL_Tools::map (std::sin ((elapsed_time_f * 1000.0f) / 500.0f), -1.0f, 1.0f, 0.7f, 1.3f), 1.0f);
    Common_Image_Tools::HSVToRGB ((i / static_cast<float> (count)) * 360.0f, 1.0f, 1.0f, r_2, g, b);
    olc::Pixel color (static_cast<uint8_t> (r_2 * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (center_s.x + std::sin ((i / static_cast<float> (count)) * 2.0f * static_cast<float> (M_PI)) * r),            static_cast<int32_t> (center_s.y + std::cos ((i / static_cast<float> (count)) * 2.0f * static_cast<float> (M_PI)) * r),
                                    static_cast<int32_t> (center_s.x + std::sin ((i / static_cast<float> (count)) * 2.0f * static_cast<float> (M_PI)) * (r + 100.0f)), static_cast<int32_t> (center_s.y + std::cos ((i / static_cast<float> (count)) * 2.0f * static_cast<float> (M_PI)) * (r + 100.0f)),
                                    color, 0xFFFFFFFF);
  } // end FOR

  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
