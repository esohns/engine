#include "stdafx.h"

#include "pge_462.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_21.h"

PGE_462::PGE_462 ()
 : olc::PixelGameEngine ()
 , startColor_ (olc::BLACK)
 , endColor_ (olc::BLACK)
 , bgColor_ (olc::BLACK)
 , circles_ ()
{
  sAppName = "Example 462";
}

bool
PGE_462::OnUserCreate ()
{
  float r, g, b;
  Common_Image_Tools::HSVToRGB (Common_Tools::getRandomNumber (0.0f, 360.0f),
                                Common_Tools::getRandomNumber (0.0f, 1.0f),
                                Common_Tools::getRandomNumber (0.0f, 1.0f),
                                r, g, b);
  startColor_ = olc::PixelF (r, g, b, 1.0f);
  Common_Image_Tools::HSVToRGB (Common_Tools::getRandomNumber (0.0f, 360.0f),
                                Common_Tools::getRandomNumber (0.0f, 1.0f),
                                Common_Tools::getRandomNumber (0.0f, 1.0f),
                                r, g, b);
  endColor_ = olc::PixelF (r, g, b, 1.0f);

  Common_Image_Tools::HSVToRGB (32.0f, 6.0f / 100.0f, 95.0f / 100.0f, r, g, b);
  float r_2, g_2, b_2;
  Common_Image_Tools::HSVToRGB (220.0f, 50.0f / 100.0f, 20.0f / 100.0f, r_2, g_2, b_2);
  static std::vector<olc::Pixel> palette_a = { olc::PixelF (r, g, b, 1.0f), olc::PixelF (r_2, g_2, b_2, 1.0f) };
  static std::random_device rd;
  static std::default_random_engine rng (rd ());
  std::shuffle (palette_a.begin (), palette_a.end (), rng);
  bgColor_ = palette_a[0];

  int32_t minSide =
    std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ());
  int32_t numberOfCircles = Common_Tools::getRandomNumber (6, 16);
  struct circle circle_s;
  for (int i = 0; i < numberOfCircles; i++)
  {
    circle_s.radius = (Common_Tools::getRandomNumber (0.0f, 1.0f) * 0.9f + 0.1f) * minSide * 0.15f;
    circle_s.position.x =
      Common_Tools::getRandomNumber (circle_s.radius, olc::PixelGameEngine::ScreenWidth () - circle_s.radius);
    circle_s.position.y =
      Common_Tools::getRandomNumber (circle_s.radius, olc::PixelGameEngine::ScreenHeight () - circle_s.radius);
    circle_s.velocity.x = Common_Tools::getRandomNumber (0.1f, 1.0f);
    circle_s.velocity.x = Common_Tools::testRandomProbability (0.5f) ? -circle_s.velocity.x : circle_s.velocity.x;
    circle_s.velocity.y = Common_Tools::getRandomNumber (0.1f, 1.0f);
    circle_s.velocity.y = Common_Tools::testRandomProbability (0.5f) ? -circle_s.velocity.y : circle_s.velocity.y;
    circles_.push_back (circle_s);
  } // end FOR

  return true;
}

bool
PGE_462::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    circles_.clear ();
    OnUserCreate ();
  } // end IF

  olc::PixelGameEngine::Clear (bgColor_);

  for (int32_t y = 0; y < olc::PixelGameEngine::ScreenHeight (); y += ENGINE_PGE_462_DEFAULT_LINE_THICKNESS + ENGINE_PGE_462_DEFAULT_LINE_SPACING)
  {
    float t = y / static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1);
    olc::Pixel c = olc::PixelLerp (startColor_, endColor_, t);

    std::vector<int32_t> events_a = getEvents (y);
    for (int i = 0; i < events_a.size (); i += 2)
      olc::PixelGameEngine::DrawLine (events_a[i], y, events_a[i + 1], y, c, 0xFFFFFFFF);
  } // end FOR

  for (std::vector<struct circle>::iterator iterator = circles_.begin ();
       iterator != circles_.end ();
       ++iterator)
  {
    (*iterator).position += (*iterator).velocity;

    if ((*iterator).position.x - (*iterator).radius < 0.0f)
      (*iterator).velocity.x = -(*iterator).velocity.x;
    else if ((*iterator).position.x + (*iterator).radius > static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1))
      (*iterator).velocity.x = -(*iterator).velocity.x;
    if ((*iterator).position.y - (*iterator).radius < 0.0f)
      (*iterator).velocity.y = -(*iterator).velocity.y;
    else if ((*iterator).position.y + (*iterator).radius > static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))
      (*iterator).velocity.y = -(*iterator).velocity.y;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<int32_t>
PGE_462::getEvents (int32_t y)
{
  std::vector<int32_t> events_a = {0, olc::PixelGameEngine::ScreenWidth () - 1};
  for (std::vector<struct circle>::iterator iterator = circles_.begin ();
       iterator != circles_.end ();
       ++iterator)
    if (y > (*iterator).position.y - (*iterator).radius && y < (*iterator).position.y + (*iterator).radius)
    {
      float a = std::cos (std::asin (((*iterator).position.y - y) / (*iterator).radius));
      events_a.push_back (static_cast<int32_t> ((*iterator).position.x - a * (*iterator).radius));
      events_a.push_back (static_cast<int32_t> ((*iterator).position.x + a * (*iterator).radius));
    } // end IF
  std::sort (events_a.begin (), events_a.end ());

  return events_a;
}
