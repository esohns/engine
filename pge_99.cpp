#include "stdafx.h"

#include "pge_99.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_99::PGE_99 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
{
  sAppName = "Example 99";
}

bool
PGE_99::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_99::OnUserUpdate (float fElapsedTime)
{
  //static int frame_count_i = 1;
  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  t_ += 0.01f;
  //float f_f = frame_count_i / 100.0f;
  float f_f = 0.01f;

  olc::PixelGameEngine::Clear (olc::WHITE);

  static olc::Pixel color_s = {0, 0, 0, 100};
  for (int i = 628; i--; )
  {
    float radius_f = 90 + 87 * std::sin (19.7f * sin (t_) * sin (i * f_f));
    olc::vf2d center (half_width_i + 160.0f * std::sin (t_ + i * f_f),
                      half_height_i + 160.0f * std::cos (t_ + i * f_f));
    //olc::PixelGameEngine::DrawCircle (center, static_cast<int32_t> (radius_f), color_s, 0xFF);
    draw_circle_arc (center,
                     radius_f,
                     0.0f,
                     2.0f * static_cast<float> (M_PI), // *NOTE*: also try: i * f_f
                     color_s);
  } // end FOR

  //++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_99::draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color)
{
  std::vector<olc::vf2d> points_arc;

  for (int i = 0; i <= ENGINE_PGE_94_DEFAULT_ARC_RESOLUTION + 1; ++i)
  {
    float angle_point =
      (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_94_DEFAULT_ARC_RESOLUTION));
    olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
    point_s *= radius;
    point_s += center;
    points_arc.push_back (point_s);
  } // end FOR

  for (int i = 0; i <= ENGINE_PGE_94_DEFAULT_ARC_RESOLUTION; i++)
    olc::PixelGameEngine::DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
}
