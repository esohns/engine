#include "stdafx.h"

#include "pge_230.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_10.h"

PGE_230::PGE_230 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 230");
}

bool
PGE_230::OnUserCreate ()
{
  return true;
}

bool
PGE_230::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  static float transition_f = 1.0f / 20.0f;
  float t = static_cast<float> (frame_count_i % ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES);
  if (static_cast<float> (t) < ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES * (1.0f / 2.0f - transition_f))
    t = 0.0f;
  else if (t < ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES / 2) t = easeOutElastic ((t - ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES * (1.0f / 2.0f - transition_f)) / (transition_f * ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES));
  else if (t < ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES * (1.0f - transition_f)) t = 1.0f;
  else t = easeInElastic ((ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES - t) / (transition_f * ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES));

  static olc::Pixel paletteA[4] = {olc::Pixel (0xf5, 0x89, 0xa3), olc::Pixel (0xef, 0x56, 0x2f), olc::Pixel (0xfc, 0x84, 0x05), olc::Pixel (0xf9, 0xd5, 0x31)};
  static olc::Pixel paletteB[4] = {olc::Pixel (0xab, 0xcd, 0x5e), olc::Pixel (0x14, 0x97, 0x6b), olc::Pixel (0x2b, 0x67, 0xaf), olc::Pixel (0x62, 0xb6, 0xde)};

  float theta0 = 3.0f * static_cast<float> (M_PI) * (frame_count_i % ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES / static_cast<float> (ENGINE_PGE_230_DEFAULT_NUMBER_OF_FRAMES));
  if (t < 1.0f / 2.0f) theta0 = 3.0f * static_cast<float> (M_PI) - theta0;
  float thetaStep = 2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_230_DEFAULT_NUMBER_OF_POINTS);
  for (int i = 0; i < ENGINE_PGE_230_DEFAULT_NUMBER_OF_POINTS; i++)
  {
    float theta = i * thetaStep;

    float r1a = std::sqrt (2.0f) * ENGINE_PGE_230_DEFAULT_RADIUS * squareRadius (theta + theta0);
    float r1b = ENGINE_PGE_230_DEFAULT_RADIUS * (1.0f + std::sqrt (2.0f)) / 2.0f;
    float r1 = r1a * (1.0f - t) + r1b * t;
    float x1 = r1 * std::cos (theta);
    float y1 = r1 * std::sin (theta);

    float r2a = static_cast<float> (ENGINE_PGE_230_DEFAULT_RADIUS);
    float r2b = r1b * squareRadius (theta + theta0);
    float r2 = r2a * (1.0f - t) + r2b * t;
    float x2 = r2 * std::cos (theta);
    float y2 = r2 * std::sin (theta);

    float r3a = ENGINE_PGE_230_DEFAULT_RADIUS * squareRadius (theta + theta0);
    float r3b = r1b / std::sqrt (2.0f);
    float r3 = r3a * (1.0f - t) + r3b * t;
    float x3 = r3 * std::cos (theta);
    float y3 = r3 * std::sin (theta);

    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (x1), half_height_i + static_cast<int32_t> (y1),
                                    half_width_i + static_cast<int32_t> (x2), half_height_i + static_cast<int32_t> (y2),
                                    olc::BLACK, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (x2), half_height_i + static_cast<int32_t> (y2),
                                    half_width_i + static_cast<int32_t> (x3), half_height_i + static_cast<int32_t> (y3),
                                    olc::BLACK, 0xFFFFFFFF);

    olc::Pixel colA = paletteA[i % 4];
    olc::Pixel colB = paletteB[i % 4];
    olc::Pixel color = olc::PixelLerp (colA, colB, t);
    olc::PixelGameEngine::FillCircle (half_width_i + static_cast<int32_t> (x2), half_height_i + static_cast<int32_t> (y2),
                                      5,
                                      color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_230::squareRadius (float t)
{
  return std::cos (static_cast<float> (M_PI) / 4.0f) / std::cos (std::fmod (t, (2.0f * static_cast<float> (M_PI)) / 4.0f) - (static_cast<float> (M_PI) / 4.0f));
}

float
PGE_230::easeOutElastic (float x)
{
  static float c4 = (2.0f * static_cast<float> (M_PI)) / 3.0f;
  return x == 0.0f ? 0.0f 
                   : x == 1.0f ? 1.0f
                               : static_cast<float> (std::pow (2, -10.0f * x)) * std::sin ((x * 10.0f - 0.75f) * c4) + 1.0f;
}

float
PGE_230::easeInElastic (float x)
{
  static float c4 = (2.0f * static_cast<float> (M_PI)) / 3.0f;
  return x == 0.0f ? 0.0f 
                   : x == 1.0f ? 1.0f
                               : -static_cast<float> (std::pow (2, 10.0f * x - 10.0f)) * std::sin ((x * 10.0f - 10.75f) * c4);
}
