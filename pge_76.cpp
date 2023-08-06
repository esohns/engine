#include "stdafx.h"

#include "pge_76.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_76::PGE_76 ()
 : olc::PixelGameEngine ()
 , numCircles_ (0)
 , numVertices_ (0)
{
  sAppName = "Example 76";
}

bool
PGE_76::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_76::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static int frame_count_i = 1;

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  updateCntByMouse ();

  float time = frame_count_i / 20.0f;
  for (int ci = 0; ci < numCircles_; ci++)
  {
    float thetaC =
      Common_GL_Tools::map (static_cast<float> (ci), 0.0f, static_cast<float> (numCircles_), 0.0f, static_cast<float> (M_PI) * 2.0f);
    olc::vf2d circleCenter = getCenterByTheta (thetaC, time, ENGINE_PGE_76_DEFAULT_SCALE);
    float circleSize = getSizeByTheta (thetaC, time, ENGINE_PGE_76_DEFAULT_SCALE);
    olc::Pixel c = getColorByTheta (thetaC, time);

    for (int vi = 0; vi < numVertices_; vi++)
    {
      float thetaV =
        Common_GL_Tools::map (static_cast<float> (vi), 0.0f, static_cast<float> (numVertices_), 0.0f, static_cast<float> (M_PI) * 2.0f);
      int32_t x = static_cast<int32_t> (half_width_i + circleCenter.x + std::cos (thetaV) * circleSize);
      int32_t y = static_cast<int32_t> (half_height_i + circleCenter.y + std::sin (thetaV) * circleSize);
      static int32_t prev_x = x;
      static int32_t prev_y = y;
      //olc::PixelGameEngine::Draw (x, y, c);
      olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, c, 0xFFFFFFFF);
      prev_x = x;
      prev_y = y;
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_76::updateCntByMouse ()
{
  float xoffset = std::abs (static_cast<float> (olc::PixelGameEngine::GetMouseX ()) - olc::PixelGameEngine::ScreenWidth () / 2.0f);
  float yoffset = std::abs (static_cast<float> (olc::PixelGameEngine::GetMouseY ()) - olc::PixelGameEngine::ScreenHeight () / 2.0f);
  numCircles_ =
    static_cast<int32_t> (Common_GL_Tools::map (xoffset, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2), static_cast<float> (ENGINE_PGE_76_DEFAULT_MAX_CIRCLES), static_cast<float> (ENGINE_PGE_76_DEFAULT_MIN_CIRCLES)));
  numVertices_ =
    static_cast<int32_t> (Common_GL_Tools::map (yoffset, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2), static_cast<float> (ENGINE_PGE_76_DEFAULT_MAX_VERTICES), static_cast<float> (ENGINE_PGE_76_DEFAULT_MIN_VERTICES)));
}

olc::vf2d
PGE_76::getCenterByTheta (float theta_in, float time_in, float scale_in)
{
  olc::vf2d direction (std::cos (theta_in), std::sin (theta_in));
  float distance = 0.6f + 0.2f * std::cos (theta_in * 6.0f + std::cos (theta_in * 8.0f + time_in));
  olc::vf2d circle_center = direction;
  circle_center *= distance * scale_in;
  return circle_center;
}

float
PGE_76::getSizeByTheta (float theta_in, float time_in, float scale_in)
{
  float offset_f = 0.2f + 0.12f * std::cos (theta_in * 9.0f - time_in * 2.0f);
  float circle_size = scale_in * offset_f;
  return circle_size;
}

olc::Pixel
PGE_76::getColorByTheta (float theta_in, float time_in)
{
  float th = 8.0f * theta_in + time_in * 2.0f;
  float r = 0.6f + 0.4f * std::cos (th);
  float g = 0.6f + 0.4f * std::cos (th - static_cast<float> (M_PI) / 3.0f);
  float b = 0.6f + 0.4f * std::cos (th - static_cast<float> (M_PI) * 2.0f / 3.0f);
  uint8_t alpha =
    static_cast<uint8_t> (Common_GL_Tools::map (numCircles_, ENGINE_PGE_76_DEFAULT_MIN_CIRCLES, ENGINE_PGE_76_DEFAULT_MAX_CIRCLES, 150, 30));
  return olc::Pixel (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), alpha);
}
