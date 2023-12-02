#include "stdafx.h"

#include "pge_37.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_gl_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_37::PGE_37 ()
 : olc::PixelGameEngine ()
 , circlePath_ ()
 , trianglePath_ ()
 , angle_ (0.0f)
{
  sAppName = "Example 37";
}

bool
PGE_37::OnUserCreate ()
{
  float startA = 0.0f;
  float endA = 120.0f;
  olc::vf2d start =
    polarToCartesian (ENGINE_PGE_37_DEFAULT_RADIUS, startA * (static_cast<float> (M_PI) / 180.0f));
  olc::vf2d end =
    polarToCartesian (ENGINE_PGE_37_DEFAULT_RADIUS, endA * (static_cast<float> (M_PI) / 180.0f));
  for (int a = static_cast<int> (startA);
       a < 360;
       a += ENGINE_PGE_37_DEFAULT_SPACING)
  {
    olc::vf2d cv =
      polarToCartesian (ENGINE_PGE_37_DEFAULT_RADIUS, static_cast<float> (a) * (static_cast<float> (M_PI) / 180.0f));
    circlePath_.push_back (cv);
    float amount_f = static_cast<float> (a % 120) / (endA - startA);
    olc::vf2d tv = lerpVector (start, end, amount_f);
    trianglePath_.push_back (tv);

    if ((a + ENGINE_PGE_37_DEFAULT_SPACING) % 120 == 0)
    {
      startA += 120.0f;
      endA += 120.0f;
      start =
        polarToCartesian (ENGINE_PGE_37_DEFAULT_RADIUS, startA * (static_cast<float> (M_PI) / 180.0f));
      end =
        polarToCartesian (ENGINE_PGE_37_DEFAULT_RADIUS, endA * (static_cast<float> (M_PI) / 180.0f));
    } // end IF
  } // end FOR

  return true;
}

bool
PGE_37::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  // *NOTE*: because of a missing translate-to-center function add necessary
  //         offsets - this works; however, the triangle is now displayed
  //         rotated clockwise by PI/2

  float amount_f = (sin (angle_ - (static_cast<float> (M_PI) / 2.0f)) + 1.0f) / 2.0f;
  angle_ += 0.02f;
  olc::vf2d previous_point;
  float x, y;
  for (int i = 0; i < circlePath_.size (); i++)
  {
    if (!i)
      previous_point = {(olc::PixelGameEngine::ScreenWidth () / 2.0f)  + circlePath_[0].x,
                        (olc::PixelGameEngine::ScreenHeight () / 2.0f) + circlePath_[0].y};

    x =
      (olc::PixelGameEngine::ScreenWidth () / 2.0f) + lerp (circlePath_[i].x, trianglePath_[i].x, amount_f);
    y =
      (olc::PixelGameEngine::ScreenHeight () / 2.0f) + lerp (circlePath_[i].y, trianglePath_[i].y, amount_f);
    olc::PixelGameEngine::DrawLine (previous_point,
                                    {static_cast<int32_t> (x), static_cast<int32_t> (y)},
                                    olc::WHITE, 0xFFFFFFFF);
    //olc::PixelGameEngine::Draw (static_cast<int32_t> (x), static_cast<int32_t> (y), olc::WHITE);

    if (i)
      previous_point = {x, y};
  } // end FOR
  x = (olc::PixelGameEngine::ScreenWidth () / 2.0f)  + circlePath_[0].x;
  y = (olc::PixelGameEngine::ScreenHeight () / 2.0f) + circlePath_[0].y;
  olc::PixelGameEngine::DrawLine (previous_point,
                                  {static_cast<int32_t> (x), static_cast<int32_t> (y)},
                                  olc::WHITE, 0xFFFFFFFF);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::vf2d
PGE_37::polarToCartesian (int32_t radius_in, float angle_in)
{
  return {static_cast<float> (radius_in) * cos (angle_in), static_cast<float> (radius_in) * sin (angle_in)};
}

olc::vf2d
PGE_37::lerpVector (const olc::vf2d& start_in, const olc::vf2d& end_in, float amount_in)
{
  return ((1.0f - amount_in) * start_in) + (amount_in * end_in);
}

float
PGE_37::lerp (float start_in, float end_in, float amount_in)
{
  return (1.0f - amount_in) * start_in + amount_in * end_in;
}

