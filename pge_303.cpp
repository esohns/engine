#include "stdafx.h"

#include "pge_303.h"

#include <random>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_303::PGE_303 ()
 : olc::PixelGameEngine ()
 , circles_ ()
 , segments_ ()
 , rectangles_ ()
 , triangles_ ()
 , points_ ()
 , pointer_ (0)
 , endtime_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 303");
}

bool
PGE_303::OnUserCreate ()
{
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i += 2)
    for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); j += 2)
      points_.push_back (olc::vf2d (static_cast<float> (i), static_cast<float> (j)));

  reset ();

  return true;
}

bool
PGE_303::OnUserUpdate (float fElapsedTime)
{
  static float elapsed_time_f = 0.0f;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    reset ();

  if (pointer_ < static_cast<int> (points_.size ()))
  {
    gradients (std::min (1000, static_cast<int> (points_.size ()) - pointer_));
    endtime_ = (elapsed_time_f * 1000.0f) + ENGINE_PGE_303_DEFAULT_INTERVAL_MS;
  } // end IF
  if ((elapsed_time_f * 1000.0f) > endtime_)
  { // hold final image for 5 seconds then restart
    reset ();
  } // end IF

  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_303::reset ()
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  static std::random_device rd;
  static std::default_random_engine rng (rd ());
  std::shuffle (points_.begin (), points_.end (), rng);

  pointer_ = 0;

  circles_.clear ();
  segments_.clear ();
  rectangles_.clear ();
  triangles_.clear ();

  for (int i = 0; i < 6; i++)
  {
    struct circle circle_s;
    circle_s.position.x = Common_Tools::getRandomNumber (-0.75f, -0.25f);
    circle_s.position.y = Common_Tools::getRandomNumber (-0.75f, -0.25f);
    circle_s.d = Common_Tools::getRandomNumber (0.05f, 0.25f);
    circles_.push_back (circle_s);
  } // end FOR
  for (int i = 0; i < 10; i++)
  {
    struct segment segment_s;
    segment_s.start.x = Common_Tools::getRandomNumber (0.0f, 1.0f);
    segment_s.start.y = Common_Tools::getRandomNumber (-1.0f, 0.0f);
    segment_s.end.x = Common_Tools::getRandomNumber (0.0f, 1.0f);
    segment_s.end.y = Common_Tools::getRandomNumber (-1.0f, 0.0f);
    segments_.push_back (segment_s);
  } // end FOR
  for (int i = 0; i < 5; i++)
  {
    struct rectangle rectangle_s;
    rectangle_s.position.x = Common_Tools::getRandomNumber (0.25f, 0.75f);
    rectangle_s.position.y = Common_Tools::getRandomNumber (0.25f, 0.75f);
    rectangle_s.w = Common_Tools::getRandomNumber (0.1f, 0.25f);
    rectangle_s.h = Common_Tools::getRandomNumber (0.1f, 0.25f);
    rectangle_s.a = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    rectangles_.push_back (rectangle_s);
  } // end FOR
  for (int i = 0; i < 3; i++)
  {
    struct triangle triangle_s;
    triangle_s.p1.x = Common_Tools::getRandomNumber (-1.0f, 0.0f);
    triangle_s.p1.y = Common_Tools::getRandomNumber (0.0f, 1.0f);
    triangle_s.p2.x = Common_Tools::getRandomNumber (-1.0f, 0.0f);
    triangle_s.p2.y = Common_Tools::getRandomNumber (0.0f, 1.0f);
    triangle_s.p3.x = Common_Tools::getRandomNumber (-1.0f, 0.0f);
    triangle_s.p3.y = Common_Tools::getRandomNumber (0.0f, 1.0f);
    triangles_.push_back (triangle_s);
  } // end FOR
}

void
PGE_303::gradients (int ppf)
{
  for (int i = 0; i < ppf; i++)
  {
    float gX =
      Common_GL_Tools::map (points_[pointer_].x, static_cast<float> (0.0f), static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), -1.0f, 1.0f);
    float gY =
      Common_GL_Tools::map (points_[pointer_].y, static_cast<float> (0.0f), static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), -1.0f, 1.0f);
    olc::vf2d temp (gX, gY);
    float d = sdf (temp);

    olc::Pixel c;
    float r, g, b;
    Common_Image_Tools::HSVToRGB (0.0f, 0.0f, 1.0f, r, g, b);
    c.r = static_cast<uint8_t> (r * 255.0f);
    c.g = static_cast<uint8_t> (g * 255.0f);
    c.b = static_cast<uint8_t> (b * 255.0f);
    float hue_f, int_fract_f;
    if (d < -0.008f)
    {
      hue_f = Common_GL_Tools::map (std::modf (d * 20.0f, &int_fract_f), 0.0f, 1.0f, 0.0f, 360.0f);
      Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 0.5f, r, g, b);
      c.r = static_cast<uint8_t> (r * 255.0f);
      c.g = static_cast<uint8_t> (g * 255.0f);
      c.b = static_cast<uint8_t> (b * 255.0f);
    } // end IF
    else if (d > 0.008f)
    {
      hue_f = Common_GL_Tools::map (1.0f - std::modf (d * 20.0f, &int_fract_f), 0.0f, 1.0f, 0.0f, 360.0f);
      Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
      c.r = static_cast<uint8_t> (r * 255.0f);
      c.g = static_cast<uint8_t> (g * 255.0f);
      c.b = static_cast<uint8_t> (b * 255.0f);
    } // end ELSE IF

    // convert to screen coordinates
    float X =
      static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) * (gX + 1.0f) / 2.0f;
    float Y =
      static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) - olc::PixelGameEngine::ScreenHeight () * (gY + 1.0f) / 2.0f;
    c.a = 255 / 5;
    //strokeWeight(6);
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (X), static_cast<int32_t> (Y), 2, c);
    c.a = 255;
    //strokeWeight(2);
    olc::PixelGameEngine::Draw (static_cast<int32_t> (X), static_cast<int32_t> (Y), c);
    pointer_++;
  } // end FOR
}

float
PGE_303::sdf (olc::vf2d& p)
{
  float S = 1.0f;

  for (int i = 0; i < static_cast<int> (circles_.size ()); i++)
  {
    olc::vf2d pt = p - circles_[i].position;
    S = sdXor (S, sdCircle (pt, circles_[i].d));
  } // end FOR

  for (int i = 0; i < static_cast<int> (segments_.size ()); i++)
  {
    S = sdXor (S, sdSegment (p, segments_[i].start, segments_[i].end));
  } // end FOR

  for (int i = 0; i < static_cast<int> (rectangles_.size ()); i++)
  {
    olc::vf2d pt = p - rectangles_[i].position;
    pt = sdRotate (pt, rectangles_[i].a);
    S = sdXor (S, sdBox (pt, rectangles_[i].w, rectangles_[i].h));
  } // end FOR

  for (int i = 0; i < static_cast<int> (triangles_.size ()); i++)
  {
    S = sdXor (S, sdTriangle (p, triangles_[i].p1, triangles_[i].p2, triangles_[i].p3));
  } // end FOR

  return S;
}
