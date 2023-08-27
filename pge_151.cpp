#include "stdafx.h"

#include "pge_151.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_151::PGE_151 ()
 : olc::PixelGameEngine ()
 , grid_ ()
{
  sAppName = "Example 151";
}

bool
PGE_151::OnUserCreate ()
{
  for (int j = 5; j--;)
    for (int i = 5; i--;)
    {
      float x =
        Common_GL_Tools::map (static_cast<float> (i), 0.0f, 4.0f, static_cast<float> (ENGINE_PGE_151_DEFAULT_MARGIN), static_cast<float> (olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_151_DEFAULT_MARGIN));
      float y =
        Common_GL_Tools::map (static_cast<float> (j), 0.0f, 4.0f, static_cast<float> (ENGINE_PGE_151_DEFAULT_MARGIN), static_cast<float> (olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_151_DEFAULT_MARGIN));
      grid_.push_back ({x, y});
    } // end FOR

  return true;
}

bool
PGE_151::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  olc::vf2d mouse_position (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                            static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  olc::Pixel color;
  float r_2, g, b;

  for (std::vector<olc::vf2d>::iterator iterator = grid_.begin ();
       iterator != grid_.end ();
       ++iterator)
  {
    color = olc::WHITE;
    if ((*iterator).dist (mouse_position) <= ENGINE_PGE_151_DEFAULT_RADIUS)
      color = {44, 44, 44, 255};
    olc::PixelGameEngine::FillCircle (*iterator, 5, color);
  } // end FOR

  for (float r = 0.0f; r < 2.0f * static_cast<float> (M_PI); r += 2.0f * static_cast<float> (M_PI) / 36.0f)
  {
    float a = r + frame_count_i / 360.0f;
    float hue = Common_GL_Tools::map (r, 0.0f, 2.0f * static_cast<float> (M_PI), 0.0f, 360.0f);
    Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r_2, g, b);
    color.r = static_cast<uint8_t> (r_2 * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);

    float x = mouse_position.x + ENGINE_PGE_151_DEFAULT_RADIUS * std::cos (a);
    float y = mouse_position.y + ENGINE_PGE_151_DEFAULT_RADIUS * std::sin (a);
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y), 2, olc::WHITE);
    drawLine (x, y, color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_151::drawLine (float x, float y, olc::Pixel& color)
{
  bool is_first_b = true;
  for (int i = 500; i--;)
  {
    float angle = getAngle (x, y);
    if (angle == -1.0f) break;

    static int32_t prev_x, prev_y;
    if (is_first_b)
    {
      is_first_b = false;
      prev_x = static_cast<int32_t> (x);
      prev_y = static_cast<int32_t> (y);
    } // end IF
    x += std::cos (angle) * 3.0f;
    y += std::sin (angle) * 3.0f;

    olc::PixelGameEngine::DrawLine (prev_x, prev_y, static_cast<int32_t> (x), static_cast<int32_t> (y), color, 0xFFFFFFFF);

    prev_x = static_cast<int32_t> (x);
    prev_y = static_cast<int32_t> (y);
  } // end FOR
}

float
PGE_151::getAngle (float x, float y)
{
  float angle = 0.0f;
  
  olc::vf2d mouse_position (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                            static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  for (std::vector<olc::vf2d>::iterator iterator = grid_.begin ();
       iterator != grid_.end ();
       ++iterator)
    if ((*iterator).dist (mouse_position) > ENGINE_PGE_151_DEFAULT_RADIUS)
    {
      float dy = (*iterator).y - y;
      float dx = (*iterator).x - x;
      if (std::sqrt (dx * dx + dy * dy) < 3.0f) return -1.0f;
      angle += std::atan2 (dy, dx);
    } // end IF

  return angle;
}
