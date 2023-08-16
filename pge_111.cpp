#include "stdafx.h"

#include "pge_111.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_111::PGE_111 ()
 : olc::PixelGameEngine ()
 , theta_ (static_cast<float> (M_PI))
{
  sAppName = "Example 111";
}

bool
PGE_111::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  //olc::PixelGameEngine::Clear ({0, 0, 0, 20});

  return true;
}

bool
PGE_111::OnUserUpdate (float fElapsedTime)
{
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () * 0.75f);
  static olc::Pixel color_s = olc::WHITE;

  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_111_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_111_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < ENGINE_PGE_111_DEFAULT_NUM; i++)
  {
    float offset_f =
      2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_111_DEFAULT_NUM) * static_cast<float> (i);
    float arc_end_f =
      Common_GL_Tools::map (static_cast<float> (std::sin (theta_ + offset_f)), -1.0f, 1.0f, static_cast<float> (M_PI), 2.0f * static_cast<float> (M_PI));
    draw_circle_arc (center_s, static_cast<float> (i * ENGINE_PGE_111_DEFAULT_STEP), static_cast<float> (M_PI), arc_end_f, color_s);
  } // end FOR
  theta_ += 0.0523f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_111::draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color)
{
  std::vector<olc::vf2d> points_arc;

  for (int i = 0; i <= ENGINE_PGE_111_DEFAULT_ARC_RESOLUTION + 1; ++i)
  {
    float angle_point =
      (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_111_DEFAULT_ARC_RESOLUTION));
    olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
    point_s *= radius;
    point_s += center;
    points_arc.push_back (point_s);
  } // end FOR

  for (int i = 0; i <= ENGINE_PGE_111_DEFAULT_ARC_RESOLUTION; i++)
    olc::PixelGameEngine::DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
}
