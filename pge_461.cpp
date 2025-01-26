#include "stdafx.h"

#include "pge_461.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_21.h"

PGE_461::PGE_461 ()
 : olc::PixelGameEngine ()
 , points_ ()
{
  sAppName = "Example 461";
}

bool
PGE_461::OnUserCreate ()
{
  return true;
}

bool
PGE_461::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  static int frame_count_i = 1;
  float t =
    (frame_count_i % ENGINE_PGE_461_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_461_DEFAULT_NUMBER_OF_FRAMES);
  createCompo (t);
  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_461::subdivide (const struct rectangle& rect, bool horizontally)
{
  struct rectangle rectangle1, rectangle2;
  if (horizontally)
  {
    int w1 =
      Common_Tools::getRandomNumber (rect.size.x / 4, 3 * rect.size.x / 4), w2 = rect.size.x - w1;
    rectangle1.position = rect.position;
    rectangle1.size = { w1, rect.size.y };
    rectangle2.position = { rect.position.x + w1, rect.position.y };
    rectangle2.size = { w2, rect.size.y };
  } // end IF
  else
  {
    int h1 = Common_Tools::getRandomNumber (rect.size.y / 4, 3 * rect.size.y / 4), h2 = rect.size.y - h1;
    rectangle1.position = rect.position;
    rectangle1.size = { rect.size.x, h1 };
    rectangle2.position = { rect.position.x, rect.position.y + h1 };
    rectangle2.size = { rect.size.x, h2 };
  } // end ELSE

  for (std::vector<int>::const_iterator iterator = rect.contains.begin ();
       iterator != rect.contains.end();
       ++iterator)
  {
    if (pointInRectangle (points_[*iterator], rectangle1)) rectangle1.contains.push_back (*iterator);
    else rectangle2.contains.push_back (*iterator);
  } // end FOR

  if (rectangle1.contains.size () > 1 && (rectangle1.size.x > 1 || rectangle1.size.y > 1)) subdivide (rectangle1, !horizontally);
  else olc::PixelGameEngine::DrawRect (rectangle1.position, rectangle1.size, olc::BLACK);
  if (rectangle2.contains.size () > 1 && (rectangle2.size.x > 1 || rectangle2.size.y > 1)) subdivide (rectangle2, !horizontally);
  else olc::PixelGameEngine::DrawRect (rectangle2.position, rectangle2.size, olc::BLACK);
}

void
PGE_461::createCompo (float t)
{
  points_.clear ();

  static olc::vf2d center_s =
    { olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f };
  static float thetaStep =
    (2.0f * static_cast<float> (M_PI)) / ENGINE_PGE_461_DEFAULT_NUMBER_OF_CORNERS;
  float theta0 =
    t * (2.0f * static_cast<float> (M_PI)) / ENGINE_PGE_461_DEFAULT_NUMBER_OF_CORNERS;
  for (int i = 0; i < ENGINE_PGE_461_DEFAULT_NUMBER_OF_CORNERS; i++)
  {
    float x1 =
      center_s.x + ENGINE_PGE_461_DEFAULT_NUMBER_OF_RADIUS * std::cos (i * thetaStep + theta0);
    float y1 =
      center_s.y + ENGINE_PGE_461_DEFAULT_NUMBER_OF_RADIUS * std::sin (i * thetaStep + theta0);
    float x2 =
      center_s.x + ENGINE_PGE_461_DEFAULT_NUMBER_OF_RADIUS * std::cos ((i + 1) * thetaStep + theta0);
    float y2 =
      center_s.y + ENGINE_PGE_461_DEFAULT_NUMBER_OF_RADIUS * std::sin ((i + 1) * thetaStep + theta0);
    for (int j = 0; j < ENGINE_PGE_461_DEFAULT_NUMBER_OF_POINTS; j++)
    {
      float t_2 = j / static_cast<float> (ENGINE_PGE_461_DEFAULT_NUMBER_OF_POINTS);
      float x = (1.0f - t_2) * x1 + t_2 * x2;
      float y = (1.0f - t_2) * y1 + t_2 * y2;
      points_.push_back ({ x, y });
    } // end FOR
  } // end FOR

  struct rectangle rectangle_s;
  rectangle_s.position = { 0, 0 };
  static olc::vi2d screen_size_s =
    { olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight () };
  rectangle_s.size = screen_size_s;
  for (int i = 0; i < static_cast<int> (points_.size ()); i++)
    rectangle_s.contains.push_back (i);
  subdivide (rectangle_s, Common_Tools::testRandomProbability (0.5f));
}
