#include "stdafx.h"

#include "pge_252.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_252::PGE_252 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , insideColor_ (0x40, 0xbc, 0xd8, 100U)
 , outsideColor_ (0xf2, 0x42, 0x36)
 , edgeColor_ (0xdd, 0x60, 0x31)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 252");
}

bool
PGE_252::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_252::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  for (int i = 0; i < ENGINE_PGE_252_DEFAULT_ITERATIONS; i++)
  {
    float p1 = Common_Tools::getRandomNumber (-1.0f, 1.0f);
    float p2 = Common_Tools::getRandomNumber (-1.0f, 1.0f);
    float d = sdf (p1, p2, frame_count_i);
    olc::Pixel color = edgeColor_;
    if (d < -0.01f) color = insideColor_;
    if (d > 0.01f) color = outsideColor_;
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> ((p1 + 1.0f) * half_width_i),
                                      static_cast<int32_t> ((p2 + 1.0f) * half_height_i),
                                      3,
                                      color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_252::length (float x, float y)
{
  return std::sqrt (x * x + y * y);
}

float
PGE_252::edge (float a, float b)
{
  return a > 0.0f && b > 0.0f ? length (a, b) 
                              : a > b ? a
                                      : b;
}

float
PGE_252::sdfBox (float x, float y, float cx, float cy, float w, float h)
{
  x -= cx;
  y -= cy;

  return edge (std::abs (x) - w, std::abs (y) - h);
}

float
PGE_252::sdfRepeat (float x, float r)
{
  x /= r;
  x -= std::floor (x) + 0.5f;
  x *= r;

  return x;
}

float
PGE_252::sdf (float x, float y, int frameCount)
{
  float mover = Common_GL_Tools::map (std::sin (frameCount / 200.0f), -1.0f, 1.0f, 0.25f, 0.75f);
  x = std::abs (x) - mover;
  y = std::abs (y) - mover;

  float sizer =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (x, y, frameCount / 200.0)), -1.0f, 1.0f, 0.01f, 1.0f);
  float boxed = sdfBox (x, y, 0.0f, 0.0f, sizer, sizer);
  boxed = std::abs (boxed) - 0.3f;
  boxed = sdfRepeat (boxed, 0.1f);

  return boxed;
}
