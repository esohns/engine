#include "stdafx.h"

#include "pge_176.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_176::PGE_176 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 176");
}

bool
PGE_176::OnUserCreate ()
{
  return true;
}

bool
PGE_176::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  int32_t NUM = 35 - (olc::PixelGameEngine::GetMouseX () % 32);
  for (int i = 0; i < NUM - 1; i++)
    for (int j = 0; j < NUM; j++)
    {
      olc::vf2d index_s (static_cast<float> (i), static_cast<float> (j));
      olc::vf2d p = pos (index_s, frame_count_i, NUM);
      index_s.x = static_cast<float> (i + 1);
      index_s.y = static_cast<float> (j);
      olc::vf2d pLeft = pos (index_s, frame_count_i, NUM);
      joint (p, pLeft);
      if (j < NUM - 1)
      {
        index_s.x = static_cast<float> (i);
        index_s.y = static_cast<float> (j + 1);
        olc::vf2d pDown = pos (index_s, frame_count_i, NUM);
        joint (p, pDown);
      } // end IF
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::vf2d
PGE_176::pos (olc::vf2d& p,
              int frameCount,
              int32_t NUM)
{
  float rad = static_cast<float> (M_PI) / 180.0f * (frameCount + (p.x + p.y * NUM) * 20.0f);
  p.x =
    Common_GL_Tools::map (p.x, 0.0f, static_cast<float> (NUM - 1), -olc::PixelGameEngine::ScreenWidth () / 2.0f - ENGINE_PGE_176_DEFAULT_MARGIN, olc::PixelGameEngine::ScreenWidth () / 2.0f + ENGINE_PGE_176_DEFAULT_MARGIN);
  p.y =
    Common_GL_Tools::map (p.y, 0.0f, static_cast<float> (NUM - 1), -olc::PixelGameEngine::ScreenHeight () / 2.0f - ENGINE_PGE_176_DEFAULT_MARGIN, olc::PixelGameEngine::ScreenHeight () / 2.0f + ENGINE_PGE_176_DEFAULT_MARGIN);
  p.x += std::cos (rad) * 30.0f;
  p.y += std::sin (rad) * 30.0f;
  return p;
}

void
PGE_176::joint (olc::vf2d& p1, olc::vf2d& p2)
{
  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  float x1 = p1.x;
  float y1 = p1.y;
  float x2 = p2.x;
  float y2 = p2.y;

  float dis = p1.dist (p2);
  float R1 = dis * 0.8f;
  float R2 = dis * 1.2f;

  float rad = std::atan2 (y2 - y1, x2 - x1);
  float rad2 = std::acos (((dis * dis) + (R1 * R1) - (R2 * R2)) / (2.0f * dis * R1));
  float x3 = x1 + R1 * std::cos (rad + rad2);
  float y3 = y1 + R1 * std::sin (rad + rad2);

  olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (x1), half_height_i + static_cast<int32_t> (y1), half_width_i + static_cast<int32_t> (x3), half_height_i + static_cast<int32_t> (y3),
                                  olc::WHITE, 0xFFFFFFFF);
  olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (x2), half_height_i + static_cast<int32_t> (y2), half_width_i + static_cast<int32_t> (x3), half_height_i + static_cast<int32_t> (y3),
                                  olc::WHITE, 0xFFFFFFFF);
}
