#include "stdafx.h"

#include "pge_217.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_217::PGE_217 ()
 : olc::PixelGameEngine ()
 , h_ (0.0f)
 , delta_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 217");
}

bool
PGE_217::OnUserCreate ()
{
  h_ = ENGINE_PGE_217_DEFAULT_R * std::sqrt (3.0f) / 2.0f;
  delta_ = 1.0f - std::sqrt (3.0f) / 2.0f;

  return true;
}

bool
PGE_217::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  
  static olc::Pixel a (0x62, 0xb6, 0xde, 0xff);
  static olc::Pixel b (0xf9, 0xd5, 0x31, 0xff);
  static olc::Pixel c (0x2b, 0x67, 0xaf, 0xff);
  static olc::Pixel d (0xab, 0xcd, 0x5e, 0xff);

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  float t0 =
    Common_GL_Tools::map ((frame_count_i % ENGINE_PGE_217_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_217_DEFAULT_NUMBER_OF_FRAMES),
                          0.0f,
                          1.0f,
                          ( std::sqrt (32.0f * (delta_ * delta_) + 1.0f) + 4.0f * delta_ - 1.0f) / (8.0f * delta_),
                          (-std::sqrt (32.0f * (delta_ * delta_) + 1.0f) + 4.0f * delta_ + 1.0f) / (8.0f * delta_));

  olc::vf2d offset_s (static_cast<float> (half_width_i),
                      static_cast<float> (half_height_i));
  makeLine (offset_s, t0,        0.0f,                                           c, b);
  makeLine (offset_s, 1.0f - t0, 2.0f * static_cast<float> (M_PI) / 6.0f,        b, a);
  makeLine (offset_s, t0,        2.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, a, d);
  makeLine (offset_s, 1.0f - t0, 3.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, d, b);
  makeLine (offset_s, t0,        4.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, b, a);
  makeLine (offset_s, 1.0f - t0, 5.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, a, c);

  {
    olc::vf2d offset_2 = offset_s;
    offset_2.x += ENGINE_PGE_217_DEFAULT_R * 3.0f / 2.0f;
    offset_2.y += h_;
    makeLine (offset_2, t0,        0.0f,                                           a, c);
    makeLine (offset_2, 1.0f - t0, 2.0f * static_cast<float> (M_PI) / 6.0f,        c, d);
    makeLine (offset_2, t0,        2.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, d, b);
    makeLine (offset_2, t0,        4.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, d);
    makeLine (offset_2, 1.0f - t0, 5.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, d, a);
  };

  {
    olc::vf2d offset_2 = offset_s;
    offset_2.y += 2.0f * h_;
    makeLine (offset_2, t0,        0.0f,                                           d, a);
    makeLine (offset_2, 1.0f - t0, 2.0f * static_cast<float> (M_PI) / 6.0f,        a, b);
    makeLine (offset_2, t0,        2.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, b, c);
    makeLine (offset_2, 1.0f - t0, 3.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, a);
  };

  {
    olc::vf2d offset_2 = offset_s;
    offset_2.x += -ENGINE_PGE_217_DEFAULT_R * 3.0f / 2.0f;
    offset_2.y += h_;
    makeLine (offset_2, 1.0f - t0, 2.0f * static_cast<float> (M_PI) / 6.0f,        c, d);
    makeLine (offset_2, t0,        2.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, d, b);
    makeLine (offset_2, 1.0f - t0, 3.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, b, c);
    makeLine (offset_2, t0,        4.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, d);
  };

  {
    olc::vf2d offset_2 = offset_s;
    offset_2.x += -ENGINE_PGE_217_DEFAULT_R * 3.0f / 2.0f;
    offset_2.y += -h_;
    makeLine (offset_2, t0,        2.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, a);
    makeLine (offset_2, 1.0f - t0, 3.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, a, d);
    makeLine (offset_2, t0,        4.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, d, c);
    makeLine (offset_2, 1.0f - t0, 5.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, b);
  };

  {
    olc::vf2d offset_2 = offset_s;
    offset_2.y += -2.0f * h_;
    makeLine (offset_2, t0,        0.0f,                                           d, a);
    makeLine (offset_2, 1.0f - t0, 3.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, a);
    makeLine (offset_2, t0,        4.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, a, b);
    makeLine (offset_2, 1.0f - t0, 5.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, b, d);
  };

  {
    olc::vf2d offset_2 = offset_s;
    offset_2.x += ENGINE_PGE_217_DEFAULT_R * 3.0f / 2.0f;
    offset_2.y += -h_;
    makeLine (offset_2, t0,        0.0f,                                           b, d);
    makeLine (offset_2, t0,        4.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, d, c);
    makeLine (offset_2, 1.0f - t0, 5.0f * 2.0f * static_cast<float> (M_PI) / 6.0f, c, b);
  };

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_217::makeLine (olc::vf2d& offset, float t0, float theta, olc::Pixel& color1, olc::Pixel& color2)
{
  olc::Pixel black (olc::BLACK);
  draw_circle_arc (offset, static_cast<float> (ENGINE_PGE_217_DEFAULT_R), theta, theta + 2.0f * static_cast<float> (M_PI) / 6.0f, black);
  olc::vf2d offset_2 = offset;
  offset_2.x += 2.0f * h_ * std::cos (theta + 2.0f * static_cast<float> (M_PI) / 12.0f);
  offset_2.y += 2.0f * h_ * std::sin (theta + 2.0f * static_cast<float> (M_PI) / 12.0f);
  draw_circle_arc (offset_2, static_cast<float> (ENGINE_PGE_217_DEFAULT_R), theta + static_cast<float> (M_PI), theta + static_cast<float> (M_PI) + 2.0f * static_cast<float> (M_PI) / 6.0f, black);

  float t = t0;
  int idx = 0;
  while (t < 1.0f - ENGINE_PGE_217_DEFAULT_EPSILON)
  {
    // draw circle
    olc::vf2d pos =
      lerpPoints (offset.x + ENGINE_PGE_217_DEFAULT_R * std::cos (theta),
                  offset.y + ENGINE_PGE_217_DEFAULT_R * std::sin (theta),
                  offset.x + ENGINE_PGE_217_DEFAULT_R * std::cos (theta + 2.0f * static_cast<float> (M_PI) / 6.0f),
                  offset.y + ENGINE_PGE_217_DEFAULT_R * std::sin (theta + 2.0f * static_cast<float> (M_PI) / 6.0f),
                  t);
    olc::Pixel color = idx % 2 == 0 ? color1 : color2;
    idx++;
    olc::PixelGameEngine::FillCircle (pos, static_cast<int32_t> (delta_ * (1.0f - (2.0f * t - 1.0f) * (2.0f * t - 1.0f)) * ENGINE_PGE_217_DEFAULT_R), color);

    // compute new t
    float a = t + delta_ * (1.0f - (2.0f * t - 1.0f) * (2.0f * t - 1.0f));
    float t1 = (std::sqrt (16.0f * a * delta_ + 16.0f * (delta_ * delta_) - 8.0f * delta_ + 1.0f) + 4.0f * delta_ - 1.0f) / (8.0f * delta_);
    t = t1;
  } // end WHILE

  t = t0;
  idx = 0;
  while (t > ENGINE_PGE_217_DEFAULT_EPSILON)
  {
    // compute new t
    float a = t - delta_ * (1.0f - (2.0f * t - 1.0f) * (2.0f * t - 1.0f));
    float t2 = (-std::sqrt (-16.0f * a * delta_ + 16.0f * (delta_ * delta_) + 8.0f * delta_ + 1.0f) + 4.0f * delta_ + 1.0f) / (8.0f * delta_);
    t = t2;

    // draw circle
    olc::vf2d pos =
      lerpPoints (offset.x + ENGINE_PGE_217_DEFAULT_R * std::cos (theta),
                  offset.y + ENGINE_PGE_217_DEFAULT_R * std::sin (theta),
                  offset.x + ENGINE_PGE_217_DEFAULT_R * std::cos (theta + 2.0f * static_cast<float> (M_PI) / 6.0f),
                  offset.y + ENGINE_PGE_217_DEFAULT_R * std::sin (theta + 2.0f * static_cast<float> (M_PI) / 6.0f),
                  t);
    idx++;
    olc::Pixel color = idx % 2 == 0 ? color1 : color2;
    olc::PixelGameEngine::FillCircle (pos, static_cast<int32_t> (delta_ * (1.0f - (2.0f * t - 1.0f) * (2.0f * t - 1.0f)) * ENGINE_PGE_217_DEFAULT_R), color);
  } // end WHILE
}

void
PGE_217::draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color)
{
  std::vector<olc::vf2d> points_arc;

  for (int i = 0; i <= ENGINE_PGE_217_DEFAULT_ARC_RESOLUTION + 1; ++i)
  {
    float angle_point =
      (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_217_DEFAULT_ARC_RESOLUTION));
    olc::vf2d point_s(static_cast<float> (std::cos(angle_point)), static_cast<float> (std::sin(angle_point)));
    point_s *= radius;
    point_s += center;
    points_arc.push_back(point_s);
  } // end FOR

  for (int i = 0; i <= ENGINE_PGE_217_DEFAULT_ARC_RESOLUTION; i++)
    olc::PixelGameEngine::DrawLine(points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
}

olc::vf2d
PGE_217::lerpPoints (float x1, float y1, float x2, float y2, float t)
{
  float x3 = (1.0f - t) * x1 + t * x2;
  float y3 = (1.0f - t) * y1 + t * y2;
  return olc::vf2d (x3, y3);
}
