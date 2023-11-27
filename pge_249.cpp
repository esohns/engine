#include "stdafx.h"

#include "pge_249.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_math_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_249::PGE_249 ()
 : olc::PixelGameEngine ()
 , u_ (0.0f)
 , tiles_ ()
 , gapsPossibilities_ ()
 , prevHorizGaps_ ()
 , prevVertGaps_ ()
 , horizGaps_ ()
 , vertGaps_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 249");
}

bool
PGE_249::OnUserCreate ()
{
  std::vector<int> temp_a;
  getGapsPossibilities (temp_a);

  u_ =
    (olc::PixelGameEngine::ScreenWidth () - 2 * ENGINE_PGE_249_DEFAULT_MARGIN) / static_cast<float> (ENGINE_PGE_249_DEFAULT_N + ENGINE_PGE_249_DEFAULT_GAP_SUM);

  for (int i = 0; i < ENGINE_PGE_249_DEFAULT_N; i++)
  {
    std::vector<int> new_line_a;
    for (int j = 0; j < ENGINE_PGE_249_DEFAULT_N; j++)
      new_line_a.push_back (Common_Tools::testRandomProbability (0.5f) ? 0 : 1);
    tiles_.push_back (new_line_a);
  } // end FOR

  prevHorizGaps_ = generateGaps ();
  prevVertGaps_ = generateGaps ();
  horizGaps_ = generateGaps ();
  vertGaps_ = generateGaps ();

  return true;
}

bool
PGE_249::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  static olc::Pixel colors_a[2] = {olc::Pixel (0x1a, 0x1c, 0x37),
                                   olc::Pixel (0xff, 0xcc, 0xd1)};
  olc::PixelGameEngine::Clear (colors_a[0]);

  float t =
    (frame_count_i % ENGINE_PGE_249_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_249_DEFAULT_NUMBER_OF_FRAMES);
  t = easeInOutExpo (t);

  if (frame_count_i % ENGINE_PGE_249_DEFAULT_NUMBER_OF_FRAMES == 0)
  {
    prevHorizGaps_ = horizGaps_;
    prevVertGaps_ = vertGaps_;
    horizGaps_ = generateGaps ();
    vertGaps_ = generateGaps ();
  } // end IF

  olc::Pixel temp;
  float y = static_cast<float> (ENGINE_PGE_249_DEFAULT_MARGIN);
  for (int j = 0; j < ENGINE_PGE_249_DEFAULT_N; j++)
  {
    float x = static_cast<float> (ENGINE_PGE_249_DEFAULT_MARGIN);
    float h =
      j < ENGINE_PGE_249_DEFAULT_N - 1 ? u_ * Common_Math_Tools::lerp (static_cast<float> (prevVertGaps_[j]), static_cast<float> (vertGaps_[j]), t)
                                       : 0.0f;
    for (int i = 0; i < ENGINE_PGE_249_DEFAULT_N; i++)
    {
      // truchet tile
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y), static_cast<int32_t> (u_), static_cast<int32_t> (u_), colors_a[tiles_[i][j]]);

      olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x + u_), static_cast<int32_t> (y),      static_cast<int32_t> (u_ / 2.0f), colors_a[0]);
      olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x),      static_cast<int32_t> (y + u_), static_cast<int32_t> (u_ / 2.0f), colors_a[0]);

      olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x),      static_cast<int32_t> (y),      static_cast<int32_t> (u_ / 2.0f), colors_a[1]);
      olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x + u_), static_cast<int32_t> (y + u_), static_cast<int32_t> (u_ / 2.0f), colors_a[1]);

      // rectangles beneath
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y + u_), static_cast<int32_t> (u_), static_cast<int32_t> (h), colors_a[0]);
      if (i == 0)
        olc::PixelGameEngine::FillRect (static_cast<int32_t> (x - u_ / 2.0f), static_cast<int32_t> (y + u_), static_cast<int32_t> (u_), static_cast<int32_t> (h), colors_a[0]);
      //if (i == 0 && j < N-1) rect(0, y+u/2, margin, h+u);

      olc::PixelGameEngine::FillRect (static_cast<int32_t> (x + u_ / 2.0f), static_cast<int32_t> (y + u_), static_cast<int32_t> (u_ / 2.0f), static_cast<int32_t> (h), colors_a[1]);
      if (i == ENGINE_PGE_249_DEFAULT_N - 1)
        olc::PixelGameEngine::FillRect (static_cast<int32_t> (x + u_), static_cast<int32_t> (y + u_), static_cast<int32_t> (u_ / 2.0f), static_cast<int32_t> (h), colors_a[1]);
      //if (i == N-1 && j < N-1) rect(x+u, y+u/2, margin, h+u);

      x += u_;

      if (i < ENGINE_PGE_249_DEFAULT_N - 1)
      {
        // rectangles to the right
        float w = u_ * Common_Math_Tools::lerp (static_cast<float> (prevHorizGaps_[i]), static_cast<float> (horizGaps_[i]), t);

        olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y), static_cast<int32_t> (w), static_cast<int32_t> (u_ / 2.0f), colors_a[0]);
        if (j == 0)
          olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y - u_ / 2.0f), static_cast<int32_t> (w), static_cast<int32_t> (u_ / 2.0f), colors_a[0]);
        //if (j == 0) rect(x-u/2, 0, w+u, margin);

        olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y + u_ / 2.0f), static_cast<int32_t> (w), static_cast<int32_t> (u_ / 2.0f + h), colors_a[1]);
        if (j == ENGINE_PGE_249_DEFAULT_N - 1)
          olc::PixelGameEngine::FillRect (static_cast<int32_t> (x), static_cast<int32_t> (y + u_), static_cast<int32_t> (w), static_cast<int32_t> (u_ / 2.0f + h), colors_a[1]);
        //if (j == N-1) rect(x-u/2, y+u, w+h+u, margin);

        x += w;
      } // end IF

      temp = colors_a[0]; colors_a[0] = colors_a[1]; colors_a[1] = temp;
    }
    y += u_ + h;
    if (ENGINE_PGE_249_DEFAULT_N % 2 == 0)
    {
      temp = colors_a[0]; colors_a[0] = colors_a[1]; colors_a[1] = temp;
    } // end IF
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_249::getGapsPossibilities (std::vector<int>& arr)
{
  if (arr.size () == ENGINE_PGE_249_DEFAULT_N - 1)
  {
    int sum = 0;
    for (std::vector<int>::iterator iterator = arr.begin ();
         iterator != arr.end ();
         ++iterator)
      sum += *iterator;
    if (sum == ENGINE_PGE_249_DEFAULT_GAP_SUM)
      gapsPossibilities_.push_back (arr);
    return;
  } // end IF

  for (int i = 0; i <= ENGINE_PGE_249_DEFAULT_N; i++)
  {
    std::vector<int> arr_2 = arr;
    arr_2.push_back (i);
    getGapsPossibilities (arr_2);
  } // end FOR
}

std::vector<int>
PGE_249::generateGaps ()
{
  std::vector<int> result;

  int index_i = Common_Tools::getRandomNumber (0, static_cast<int> (gapsPossibilities_.size ()) - 1);
  result = gapsPossibilities_[index_i];
  static std::random_device rd;
  static std::default_random_engine rng (rd ());
  std::shuffle (std::begin (result), std::end (result), rng);

  return result;
}

float
PGE_249::easeInOutExpo (float x)
{
  return x == 0.0f ? 0.0f
                   : x == 1.0f ? 1.0f
                               : x < 0.5f ? static_cast<float> (std::pow (2, 20.0f * x - 10.0f)) / 2.0f
                                          : (2.0f - static_cast<float> (std::pow (2, -20.0f * x + 10.0f))) / 2.0f;
}
