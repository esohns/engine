#include "stdafx.h"

#include "pge_50.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_50::PGE_50 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 50";
}

bool
PGE_50::OnUserCreate ()
{
  return true;
}

bool
PGE_50::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static int32_t frame_count_i = 1;

  float magnitude_f = 250.0f;
  int32_t size_i = 3;
  for (int i = 0; i < 361; ++i)
  {
    float w =
      Common_GL_Tools::map (static_cast<float> (std::sin (frame_count_i * M_PI / 180.0)), -1.0f, 1.0f, -100.0f, 100.0f);
    float wave1 =
      Common_GL_Tools::map (static_cast<float> (std::sin ((frame_count_i * 0.8 + i + w) * M_PI / 180.0)), -1.0f, 1.0f, -magnitude_f, magnitude_f);
    float wave2 =
      Common_GL_Tools::map (static_cast<float> (std::cos ((frame_count_i * 2.0 + i) * M_PI / 180.0)), -1.0f, 1.0f, -magnitude_f, magnitude_f);
    float x = (olc::PixelGameEngine::ScreenWidth () / 2.0f) + wave1;
    float y = (olc::PixelGameEngine::ScreenHeight () / 2.0f) + wave2;

    uint8_t c =
      static_cast<uint8_t> (Common_GL_Tools::map (static_cast<float> (std::sin ((frame_count_i * 5.0 + i) * M_PI / 180.0)), -1.0f, 1.0f, 0.0f, 255.0f));

    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y), size_i, {c, c, c, 255});
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
