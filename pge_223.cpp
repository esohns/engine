#include "stdafx.h"

#include "pge_223.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_10.h"
#include "engine_common.h"

PGE_223::PGE_223 ()
 : olc::PixelGameEngine ()
 , s_ (0.0f)
 , u_ (0.0f)
 , dDiff_ (0.0f)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 223");
}

bool
PGE_223::OnUserCreate ()
{
  s_ =
    (olc::PixelGameEngine::ScreenWidth () - 2 * ENGINE_PGE_223_DEFAULT_MARGIN) / static_cast<float> (ENGINE_PGE_223_DEFAULT_N);
  u_ = s_ / 4.0f;
  dDiff_ = static_cast<float> (ENGINE_PGE_223_DEFAULT_D_MAX - ENGINE_PGE_223_DEFAULT_D_MIN);

  return true;
}

bool
PGE_223::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float t =
    (frame_count_i % ENGINE_PGE_223_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_223_DEFAULT_NUMBER_OF_FRAMES);
  float dummy_f;
  float tCol = std::modf (t * dDiff_ * 2.0f, &dummy_f);
  tCol = std::min (std::max (tCol, 0.0f), 0.5f);
  tCol += std::floor (t * dDiff_ * 2.0f) / 2.0f;
  tCol /= dDiff_;
  std::pair<olc::Pixel, olc::Pixel> palette = {olc::Pixel (0xff, 0xfb, 0xe6, 0xff), rainbow (tCol) };

  for (int i = 0; i < ENGINE_PGE_223_DEFAULT_N; i++)
  {
    int x = static_cast<int> (i * s_ + ENGINE_PGE_223_DEFAULT_MARGIN);
    for (int j = 0; j < ENGINE_PGE_223_DEFAULT_N; j++)
    {
      int y = static_cast<int> (j * s_ + ENGINE_PGE_223_DEFAULT_MARGIN);
      float tij =
        static_cast<float> (std::fmod (t + std::floor (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (static_cast<double> (x), static_cast<double> (y), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * dDiff_) / dDiff_, 1));
      makeTile (x, y, tij, palette);
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_223::draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color)
{
  std::vector<olc::vf2d> points_arc;

  for (int i = 0; i <= ENGINE_PGE_223_DEFAULT_ARC_RESOLUTION + 1; ++i)
  {
    float angle_point =
      (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_223_DEFAULT_ARC_RESOLUTION));
    olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
    point_s *= radius;
    point_s += center;
    points_arc.push_back (point_s);
  } // end FOR

  for (int i = 0; i <= ENGINE_PGE_223_DEFAULT_ARC_RESOLUTION; i++)
    olc::PixelGameEngine::DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
}

void
PGE_223::makeTile (int x, int y, float t, std::pair<olc::Pixel, olc::Pixel>& palette)
{
  float dt = d (t);

  int d1 = ENGINE_PGE_223_DEFAULT_D_MIN + static_cast<int> (std::floor (dt)); // amount of arcs on top-left and bottom-right corners
  int d2 = 7 - d1; // amount of arcs on top-right and bottom-left corners

  float dummy_f;
  float f = std::modf (dt, &dummy_f);
  float f1 = std::min (std::max (f, ENGINE_PGE_223_DEFAULT_EPSILON), 1.0f - ENGINE_PGE_223_DEFAULT_EPSILON);
  float f2 = std::min (std::max (1.0f - f, ENGINE_PGE_223_DEFAULT_EPSILON), 1.0f - ENGINE_PGE_223_DEFAULT_EPSILON);

  // top-left and bottom-right corners
  int k = 0;
  olc::Pixel color;
  olc::vf2d position;
  for (int i = 1; i < d1; i++)
  {
    color = k % 2 == 0 ? palette.first : palette.second;
    position.x = static_cast<float> (x); position.y = static_cast<float> (y);
    draw_circle_arc (position, i * u_, 0.0f, static_cast<float> (M_PI_2), color);
    position.x += s_; position.y += s_;
    draw_circle_arc (position, i * u_, static_cast<float> (M_PI), (3.0f * static_cast<float> (M_PI)) / 2.0f, color);
    k++;
  } // end FOR
  color = k % 2 == 0 ? palette.first : palette.second;
  position.x = static_cast<float> (x); position.y = static_cast<float> (y);
  draw_circle_arc (position, d1 * u_, 0.0f, f1 * static_cast<float> (M_PI_2), color);
  //arc(x, y, d1 * u, d1 * u, PI/2 - f1*PI/2, PI/2);
  position.x += s_; position.y += s_;
  draw_circle_arc (position, d1 * u_, static_cast<float> (M_PI), static_cast<float> (M_PI) + f1 * static_cast<float> (M_PI_2), color);
  //arc(x + s, y + s, d1 * u, d1 * u, 3*PI/2 - f1*PI/2, 3*PI/2);
  //k++;

  // top-right and bottom-left corners
  color = k % 2 == 0 ? palette.first : palette.second;
  position.x = static_cast<float> (x); position.y = static_cast<float> (y);
  position.x += s_;
  draw_circle_arc (position, (d2 + 1) * u_, static_cast<float> (M_PI_2), static_cast<float> (M_PI_2) + f2 * static_cast<float> (M_PI_2), color);
  //arc(x + s, y, (d2+1) * u, (d2+1) * u, PI - f2*PI/2, PI);
  position.x = static_cast<float> (x);
  position.y += s_;
  draw_circle_arc (position, (d2 + 1) * u_, 3.0f * static_cast<float> (M_PI_2), 3.0f * static_cast<float> (M_PI_2) + f2 * static_cast<float> (M_PI_2), color);
  //arc(x, y + s, (d2+1) * u, (d2+1) * u, TAU - f2*PI/2, TAU);
  k++;
  for (int i = d2; i > 0; i--)
  {
    color = k % 2 == 0 ? palette.first : palette.second;
    position.x = static_cast<float> (x); position.y = static_cast<float> (y);
    position.x += s_;
    draw_circle_arc (position, i * u_, static_cast<float> (M_PI_2), static_cast<float> (M_PI), color);
    position.x = static_cast<float> (x);
    position.y += s_;
    draw_circle_arc (position, i * u_, (3.0f * static_cast<float> (M_PI)) / 2.0f, 2.0f * static_cast<float> (M_PI), color);
    k++;
  } // end FOR
}

float
PGE_223::d (float x)
{
  float dummy_f;
  float dx = std::modf (x * dDiff_ * 2.0f, &dummy_f);
  dx = std::min (std::max (dx, 0.0f), 0.5f);
  dx += std::floor (2.0f * x * dDiff_) / 2.0f;
  if (dx > dDiff_ / 2.0f)
    dx = dDiff_ - dx;
  return 2.0f * dx;
}

olc::Pixel
PGE_223::rainbow (float t)
{
  static olc::Pixel palette_a[6] = {olc::Pixel (0xf9, 0xd5, 0x31, 0xff),
                                    olc::Pixel (0xab, 0xcd, 0x5e, 0xff),
                                    olc::Pixel (0x62, 0xb6, 0xde, 0xff),
                                    olc::Pixel (0xf5, 0x89, 0xa3, 0xff),
                                    olc::Pixel (0xef, 0x56, 0x2f, 0xff),
                                    olc::Pixel (0xfc, 0x84, 0x05, 0xff)};
  int i = static_cast<int> (std::floor (6.0f * t));
  float dummy_f;
  float amt = std::modf (6.0f * t, &dummy_f);
  return olc::PixelLerp (palette_a[i % 6], palette_a[(i + 1) % 6], amt);
}
