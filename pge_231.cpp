#include "stdafx.h"

#include "pge_231.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_10.h"

PGE_231::PGE_231 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 231");
}

bool
PGE_231::OnUserCreate ()
{
  return true;
}

bool
PGE_231::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  static olc::Pixel colors_a[9] = {olc::Pixel (0x20, 0x30, 0x51), olc::Pixel (0x44, 0x64, 0xa1), olc::Pixel (0x62, 0xb6, 0xde),
                                   olc::Pixel (0xb3, 0xdc, 0xe0), olc::Pixel (0xe2, 0xf0, 0xf3), olc::Pixel (0xb3, 0xdc, 0xe0),
                                   olc::Pixel (0x62, 0xb6, 0xde), olc::Pixel (0x44, 0x64, 0xa1), olc::Pixel (0x20, 0x30, 0x51)};

  int nLayers = 9;
  float t = frame_count_i / 200.0f;

  static float r1 = 200.0f, r2 = 50.0f;
  std::list<olc::vf2d> outer_points_a = makePolygon (r1, ENGINE_PGE_231_DEFAULT_NUMBER_OF_SIDES, t); // outer border polygon
  std::list<olc::vf2d> blob_points_a = makeBlob ((r1 + r2) / 2.0f, t); // central blob
  std::list<olc::vf2d> inner_points_a = makePolygon (r2, ENGINE_PGE_231_DEFAULT_NUMBER_OF_SIDES, -t); // inner border polygon

  // draw the interpolated shapes between the outer border and the blob
  for (int i = 0; i < nLayers; i++)
  {
    std::list<olc::vf2d>::iterator iterator = outer_points_a.begin ();
    std::list<olc::vf2d>::iterator iterator_2 = blob_points_a.begin ();
    olc::vf2d first_s, prev_s;
    bool is_first_b = true;
    for (int j = 0; j < ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS; j++)
    {
      olc::vf2d p1 = *iterator;
      olc::vf2d p2 = *iterator_2;
      olc::vf2d position_s (Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (nLayers - 1), p1.x, p2.x),
                            Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (nLayers - 1), p1.y, p2.y));
      if (is_first_b)
      {
        is_first_b = false;
        first_s = position_s;
        prev_s = position_s;
      }
      olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
                                      half_width_i + static_cast<int32_t> (position_s.x), half_height_i + static_cast<int32_t> (position_s.y),
                                      colors_a[i], 0xFFFFFFFF);
      prev_s = position_s;

      ++iterator;
      ++iterator_2;
    } // end FOR
    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
                                    half_width_i + static_cast<int32_t> (first_s.x), half_height_i + static_cast<int32_t> (first_s.y),
                                    colors_a[i], 0xFFFFFFFF);
  } // end FOR

  //nLayers++;
  //colors.push(backCol);

  // draw the interpolated shapes between the blob and the inner border
  for (int i = 0; i < nLayers; i++)
  {
    std::list<olc::vf2d>::iterator iterator = blob_points_a.begin ();
    std::list<olc::vf2d>::iterator iterator_2 = inner_points_a.begin ();
    olc::vf2d first_s, prev_s;
    bool is_first_b = true;
    for (int j = 0; j < ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS; j++)
    {
      olc::vf2d p1 = *iterator;
      olc::vf2d p2 = *iterator_2;
      olc::vf2d position_s (Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (nLayers - 1), p1.x, p2.x),
                            Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (nLayers - 1), p1.y, p2.y));
      if (is_first_b)
      {
        is_first_b = false;
        first_s = position_s;
        prev_s = position_s;
      }
      olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
                                      half_width_i + static_cast<int32_t> (position_s.x), half_height_i + static_cast<int32_t> (position_s.y),
                                      colors_a[i], 0xFFFFFFFF);
      prev_s = position_s;

      ++iterator;
      ++iterator_2;
    } // end FOR
    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
                                    half_width_i + static_cast<int32_t> (first_s.x), half_height_i + static_cast<int32_t> (first_s.y),
                                    colors_a[i], 0xFFFFFFFF);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::list<olc::vf2d>
PGE_231::makePolygon (float r, int n, float theta0)
{
  std::list<olc::vf2d> points_a;

  for (int i = 0; i < n; i++)
  {
    float theta = theta0 + i * 2.0f * static_cast<float> (M_PI) / static_cast<float> (n);
    float x1 = r * std::cos (theta);
    float y1 = r * std::sin (theta);
    float x2 = r * std::cos (theta + 2.0f * static_cast<float> (M_PI) / static_cast<float> (n));
    float y2 = r * std::sin (theta + 2.0f * static_cast<float> (M_PI) / static_cast<float> (n));
    for (int j = 0; j < ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS / n; j++)
    {
      float t = j / static_cast<float> (ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS) * n;
      float x = x1 * (1.0f - t) + x2 * t;
      float y = y1 * (1.0f - t) + y2 * t;
      points_a.push_back (olc::vf2d (x, y));
    } // end FOR
  } // end FOR

  // reorder the points so that every polygon will be aligned
  int index_i = static_cast<int> (points_a.size ()) - 1, i = 0;
  for (std::list<olc::vf2d>::iterator iterator = points_a.begin ();
       iterator != points_a.end ();
       ++iterator, ++i)
  {
    if (std::abs (std::atan2 ((*iterator).y, (*iterator).x)) < 2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS))
    {
      index_i = i;
      break;
    } // end IF
  } // end FOR
  int nShifts = ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS - index_i;
  for (int i = 0; i < nShifts; i++)
  {
    olc::vf2d temp_s = points_a.back ();
    points_a.pop_back ();
    points_a.push_front (temp_s);
  } // end FOR

  return points_a;
}

std::list<olc::vf2d>
PGE_231::makeBlob (float r, float t)
{
  std::list<olc::vf2d> points_a;

  float maxOffset = r / 10.0f;
  for (int i = 0; i < ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS; i++)
  {
    float theta =
      2.0f * static_cast<float> (M_PI) * i / static_cast<float> (ENGINE_PGE_231_DEFAULT_NUMBER_OF_POINTS);
    float rOffset =
      std::sin (theta * ENGINE_PGE_231_DEFAULT_NUMBER_OF_BUMPS) * std::cos (t * 10.0f) * maxOffset;
    points_a.push_back (olc::vf2d ((r + rOffset) * std::cos (theta),
                                   (r + rOffset) * std::sin (theta)));
  } // end FOR

  return points_a;
}
