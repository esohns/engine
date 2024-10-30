#include "stdafx.h"

#include "pge_376.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_376::PGE_376 ()
 : olc::PixelGameEngine ()
 , i_ (0)
 , j_ (0)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 376");
}

bool
PGE_376::OnUserCreate ()
{
  i_ =
    static_cast<int> (Common_Tools::getRandomNumber (ENGINE_PGE_376_DEFAULT_N / 5.0f, 4.0f * ENGINE_PGE_376_DEFAULT_N / 5.0f));
  j_ =
    static_cast<int> (Common_Tools::getRandomNumber (ENGINE_PGE_376_DEFAULT_N / 5.0f, 4.0f * ENGINE_PGE_376_DEFAULT_N / 5.0f));

  return true;
}

bool
PGE_376::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::WHITE);

  float t =
    2.0f * static_cast<float> (M_PI) * (frame_count_i % ENGINE_PGE_376_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_376_DEFAULT_NUMBER_OF_FRAMES);

  static int s = (olc::PixelGameEngine::ScreenWidth () - 2 * ENGINE_PGE_376_DEFAULT_MARGIN) / ENGINE_PGE_376_DEFAULT_N;

  for (int i = 0; i < ENGINE_PGE_376_DEFAULT_N; i++)
  {
    int32_t x = ENGINE_PGE_376_DEFAULT_MARGIN + i * s;

    for (int j = 0; j < ENGINE_PGE_376_DEFAULT_N; j++)
    {
      int32_t y = ENGINE_PGE_376_DEFAULT_MARGIN + j * s;

      olc::vf2d a (static_cast<float> (x) + s / 2.0f, static_cast<float> (y) + s / 2.0f);
      static float half_width_f = olc::PixelGameEngine::ScreenWidth() / 2.0f;
      static float half_height_f = olc::PixelGameEngine::ScreenHeight() / 2.0f;
      static olc::vf2d size_s (half_width_f, half_height_f);
      float d = a.dist (size_s);
      float theta =
        std::atan2 (y + s / 2.0f - half_height_f, x + s / 2.0f - half_width_f) + static_cast<float> (M_PI) + d / 30.0f;
      olc::Pixel color = rainbow (theta / (2.0f * static_cast<float> (M_PI)));
      float z = (std::sin (d / 20.0f - t) + 1.0f) / 2.0f;
      if (i == i_ && j == j_)
      {
        color = rainbow (t / (2.0f * static_cast<float> (M_PI)));
        z =
          Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (1.0 + std::cos (t), 1.0 + std::sin (t))), -1.0f, 1.0f, 0.0f, 1.0f);
      } // end IF

      olc::PixelGameEngine::FillRect (x, y, static_cast<int32_t> (z * s), s, color);
      olc::PixelGameEngine::DrawRect (x, y, s, s, olc::BLACK);
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::Pixel
PGE_376::rainbow (float t)
{
#define PALETTE_SIZE_I 8
  static olc::Pixel palette_a[PALETTE_SIZE_I] = {
    olc::Pixel (0xab, 0xcd, 0x5e, 255), olc::Pixel (0x14, 0x97, 0x6b, 255), olc::Pixel (0x2b, 0x67, 0xaf, 255),
    olc::Pixel (0x62, 0xb6, 0xde, 255), olc::Pixel (0xf5, 0x89, 0xa3, 255), olc::Pixel (0xef, 0x56, 0x2f, 255),
    olc::Pixel (0xfc, 0x84, 0x05, 255), olc::Pixel (0xf9, 0xd5, 0x31, 255)
  };
  int i = static_cast<int> (std::floor (PALETTE_SIZE_I * t));
  float num_f;
  float amt = std::modf (PALETTE_SIZE_I * t, &num_f);
  return olc::PixelLerp (palette_a[i % PALETTE_SIZE_I], palette_a[(i + 1) % PALETTE_SIZE_I], amt);
}
