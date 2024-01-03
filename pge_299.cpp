#include "stdafx.h"

#include "pge_299.h"

#include <algorithm>
#include <random>

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_13.h"

PGE_299::PGE_299 ()
 : olc::PixelGameEngine ()
 , rectangles_ ()
 , objects_ ()
 , ccc_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 299");
}

PGE_299::~PGE_299 ()
{
  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_299::OnUserCreate ()
{
  size_t seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
  std::shuffle (std::begin (colors_a), std::end (colors_a),
                std::default_random_engine (static_cast<unsigned int> (seed)));

  initialize ();

  olc::PixelGameEngine::Clear (colors_a[5]);

  return true;
}

bool
PGE_299::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator)->show (this);
    (*iterator)->move ();
  } // end FOR

  if (frame_count_i % (60 * 3) == 0)
    initialize ();

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_299::easeInOutQuart (float x)
{
  return x < 0.5f ? 8.0f * x * x * x * x
                  : 1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 4)) / 2.0f;
}

void 
PGE_299::divideRect (int x, int y, int w, int h, int min)
{
  if (w > min && h > min)
  {
    if (w >= h)
    {
      int rndw = static_cast<int> (Common_Tools::getRandomNumber (0.05f, 0.95f) * w);
      divideRect (x, y, rndw, h, min);
      divideRect (x + rndw, y, w - rndw, h, min);
    } // end IF
    else
    {
      int rndh = static_cast<int> (Common_Tools::getRandomNumber (0.05f, 0.95f) * h);
      divideRect (x, y, w, rndh, min);
      divideRect (x, y + rndh, w, h - rndh, min);
    } // end ELSE
  } // end IF
  else
  {
    struct rectangle rectangle_s;
    rectangle_s.position.x = x + w / 2.0f;
    rectangle_s.position.y = y + h / 2.0f;
    rectangle_s.w = w + 1;
    rectangle_s.h = h + 1;
    rectangles_.push_back (rectangle_s);
  } // end ELSE
}

void
PGE_299::initialize ()
{
  static unsigned int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static unsigned int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static olc::vf2d half_size_s (static_cast<float> (half_width_i),
                                static_cast<float> (half_height_i));

  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
    delete *iterator;
  objects_.clear ();

  rectangles_.clear ();
  divideRect (0, 0, olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight (), ENGINE_PGE_299_DEFAULT_MIN);

  for (std::vector<struct rectangle>::iterator iterator = rectangles_.begin ();
       iterator != rectangles_.end ();
       ++iterator)
  {
    float dst = half_size_s.dist ((*iterator).position);
    int t =
      static_cast<int> (Common_GL_Tools::map (dst, 0.0f, half_size_s.mag (), 0.0f, static_cast<float> (ENGINE_PGE_299_DEFAULT_MIN_T)));
    olc::Pixel col = colors_a[ccc_ % 6];
    if (Common_Tools::testRandomProbability (0.5f))
      objects_.push_back (new CRR01 ((*iterator).position, (*iterator).w + 1, (*iterator).h + 1, t, col));
    else
      objects_.push_back (new CRR02 ((*iterator).position, (*iterator).w - 1, (*iterator).h - 1, t, col));
  } // end FOR

  ccc_++;
}
