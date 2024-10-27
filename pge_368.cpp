#include "stdafx.h"

#include "pge_368.h"

#include "ace/Log_Msg.h"

#include "common_math_tools.h"

#include "engine_common.h"

PGE_368::PGE_368 ()
 : olc::PixelGameEngine ()
 , colors_ ()
 , weights1_ ()
 , weights2_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 368");
}

bool
PGE_368::OnUserCreate ()
{
  static olc::Pixel palette_a[5] = { {0x20, 0x30, 0x51, 255}, {0x44, 0x64, 0xa1, 255}, {0x62, 0xb6, 0xde, 255}, {0xb3, 0xdc, 0xe0, 255}, {0xe2, 0xf0, 0xf3, 255} };
  int pl = 5; // size of palette
  for (int i = 0; i < ENGINE_PGE_368_DEFAULT_N; i++)
  {
    std::vector<olc::Pixel> colors_a (ENGINE_PGE_368_DEFAULT_N);
    colors_.push_back (colors_a);
    for (int j = 0; j < ENGINE_PGE_368_DEFAULT_N; j++)
    {
      colors_[i][j] = palette_a[(i + j) % pl];

      if (i > ENGINE_PGE_368_DEFAULT_N / 2.0f && j < ENGINE_PGE_368_DEFAULT_N / 2.0f)
        colors_[i][j] = palette_a[(pl - 1 + (ENGINE_PGE_368_DEFAULT_N * ENGINE_PGE_368_DEFAULT_N) - i + j) % pl];
      if (i < ENGINE_PGE_368_DEFAULT_N / 2.0f && j > ENGINE_PGE_368_DEFAULT_N / 2.0f)
        colors_[i][j] = palette_a[(pl - 1 + (ENGINE_PGE_368_DEFAULT_N * ENGINE_PGE_368_DEFAULT_N) + i - j) % pl];
      if (i > ENGINE_PGE_368_DEFAULT_N / 2.0f && j > ENGINE_PGE_368_DEFAULT_N / 2.0f)
        colors_[i][j] = palette_a[(pl - 2 + (ENGINE_PGE_368_DEFAULT_N * ENGINE_PGE_368_DEFAULT_N) - i - j) % pl];
    } // end FOR
  } // end FOR

  for (int i = 0; i < ENGINE_PGE_368_DEFAULT_N; i++)
  {
    float w1 = (ENGINE_PGE_368_DEFAULT_N / 2.0f) - i + 1;
    if (i > ENGINE_PGE_368_DEFAULT_N / 2.0f)
      w1 = i - static_cast<float> (ENGINE_PGE_368_DEFAULT_N / 2.0f) + 1;
    w1 = (w1 * w1) + 5.0f;
    weights1_.push_back (w1);

    float w2 = static_cast<float> (i + 1);
    if (i > ENGINE_PGE_368_DEFAULT_N / 2.0f)
      w2 = static_cast<float> (ENGINE_PGE_368_DEFAULT_N - i);
    w2 = (w2 * w2) + 5.0f;
    weights2_.push_back (w2);
  } // end FOR

  return true;
}

bool
PGE_368::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  bool firstHalf =
    frame_count_i % (2 * ENGINE_PGE_368_DEFAULT_N_ANIM + 2 * ENGINE_PGE_368_DEFAULT_N_WAIT) < ENGINE_PGE_368_DEFAULT_N_ANIM + ENGINE_PGE_368_DEFAULT_N_WAIT;
  std::vector<std::vector<std::pair<float, float> > > lerpedWeights;
  float wSum = 0.0f;
  std::vector<float> hSums;
  for (int i = 0; i < ENGINE_PGE_368_DEFAULT_N; i++)
  {
    std::vector<std::pair<float, float> > weightsLine;

    int offset = 5 * static_cast<int> (std::abs (i - ENGINE_PGE_368_DEFAULT_N / 2 + 1.0f / 2.0f));
    int f = (frame_count_i + offset) % (2 * ENGINE_PGE_368_DEFAULT_N_ANIM + 2 * ENGINE_PGE_368_DEFAULT_N_WAIT);
    float t;
    if (f < ENGINE_PGE_368_DEFAULT_N_WAIT)
      t = 0.0f;
    else if (f < ENGINE_PGE_368_DEFAULT_N_ANIM + ENGINE_PGE_368_DEFAULT_N_WAIT)
      t = easeInOutQuint (((f - ENGINE_PGE_368_DEFAULT_N_WAIT) % ENGINE_PGE_368_DEFAULT_N_ANIM) / static_cast<float> (ENGINE_PGE_368_DEFAULT_N_ANIM));
    else if (f < ENGINE_PGE_368_DEFAULT_N_ANIM + 2 * ENGINE_PGE_368_DEFAULT_N_WAIT)
      t = 1.0f;
    else
      t = 1.0f - easeInOutQuint (((f - ENGINE_PGE_368_DEFAULT_N_ANIM - 2 * ENGINE_PGE_368_DEFAULT_N_WAIT) % ENGINE_PGE_368_DEFAULT_N_ANIM) / static_cast<float> (ENGINE_PGE_368_DEFAULT_N_ANIM));

    float w = Common_Math_Tools::lerp (weights1_[i], weights2_[i], t);
    wSum += w;
    float hSum = 0.0f;
    for (int j = 0; j < ENGINE_PGE_368_DEFAULT_N; j++)
    {
      float h = Common_Math_Tools::lerp (weights1_[j], weights2_[j], t);
      hSum += h;
      weightsLine.push_back (std::make_pair (w, h));
    } // end FOR
    hSums.push_back (hSum);
    lerpedWeights.push_back (weightsLine);
  } // end FOR

  int32_t x = ENGINE_PGE_368_DEFAULT_MARGIN;
  float uw = (olc::PixelGameEngine::ScreenWidth () - 2 * ENGINE_PGE_368_DEFAULT_MARGIN) / wSum;
  for (int i = 0; i < ENGINE_PGE_368_DEFAULT_N; i++)
  {
    int32_t y = ENGINE_PGE_368_DEFAULT_MARGIN;
    float uh = (olc::PixelGameEngine::ScreenHeight () - 2 * ENGINE_PGE_368_DEFAULT_MARGIN) / hSums[i];
    int32_t w = static_cast<int32_t> (lerpedWeights[i][0].first * uw);

    for (int j = 0; j < ENGINE_PGE_368_DEFAULT_N; j++)
    {
      int32_t h = static_cast<int32_t> (lerpedWeights[i][j].second * uh);
      if (firstHalf)
        olc::PixelGameEngine::FillRect (y, x, h, w, colors_[i][j]);
      else
        olc::PixelGameEngine::FillRect (x, y, w, h, colors_[i][j]);
  
      y += h;
    } // end FOR

    x += w;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
