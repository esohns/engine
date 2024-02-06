#include "stdafx.h"

#include "pge_349.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_349::PGE_349 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 349");
}

bool
PGE_349::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_349::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static float elapsed_time_f = 0.0f;

  float r, g, b;
  olc::Pixel color;
  Common_Image_Tools::HSVToRGB (((frame_count_i % 256) / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
  color =
    {static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 90};
  olc::PixelGameEngine::Clear (color);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a >= ENGINE_PGE_349_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_349_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < ENGINE_PGE_349_DEFAULT_C_SIZE; i += ENGINE_PGE_349_DEFAULT_D / 2)
  {
    Common_Image_Tools::HSVToRGB ((std::fmod (frame_count_i + ((i / static_cast<float> (ENGINE_PGE_349_DEFAULT_C_SIZE)) * 255.0f), 255.0f) / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
    color =
      {static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 255};
    crown (static_cast<float> (i), elapsed_time_f * 1000.0f, color);
  } // end FOR

  ++frame_count_i;
  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_349::crown (float r, float offset, olc::Pixel& color)
{
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth ()  / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  for (int i = 0; i < ENGINE_PGE_349_DEFAULT_K; i++)
  {
    float theta =
      static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_349_DEFAULT_K) + (r / static_cast<float> (ENGINE_PGE_349_DEFAULT_K_2 * 5) + i +
        std::cos (std::sin (std::cos ((offset / 5.0f + r) / static_cast<float> (ENGINE_PGE_349_DEFAULT_K_2) / 20.0f) * static_cast<float> (i + ENGINE_PGE_349_DEFAULT_K) + r / static_cast<float> (ENGINE_PGE_349_DEFAULT_K_2))));
    float x = center_s.x + r * std::cos (theta);
    float y = center_s.y + r * std::sin (theta);
    float R =
      (r == 0.0f) ? -1.0f
                  : static_cast<float> (ENGINE_PGE_349_DEFAULT_D) * r * std::log (r / static_cast<float> (ENGINE_PGE_349_DEFAULT_K_2)) / static_cast<float> (ENGINE_PGE_349_DEFAULT_K);
    olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (x), static_cast<int32_t> (y), R < 0.0f ? -1 : static_cast<int32_t> (R / 2.0f), color, 0xFF);
  } // end FOR
}
