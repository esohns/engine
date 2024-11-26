#include "stdafx.h"

#include "pge_429.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_429::PGE_429 ()
 : olc::PixelGameEngine ()
 , modules_ ()
 , size_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 429");
}

PGE_429::~PGE_429 ()
{
  for (std::vector<module*>::iterator iterator = modules_.begin ();
       iterator != modules_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_429::OnUserCreate ()
{
  initialize ();

  return true;
}

bool
PGE_429::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_429_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_429_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    initialize ();

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  static int counter_i = 0;
  if (counter_i == 2000)
  {
    initialize ();
    counter_i = 0;
  } // end IF
  ++counter_i;

  for (std::vector<module*>::iterator iterator = modules_.begin ();
       iterator != modules_.end ();
       ++iterator)
    (*iterator)->draw (size_, this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_429::initialize ()
{
  for (std::vector<module*>::iterator iterator = modules_.begin ();
       iterator != modules_.end ();
       ++iterator)
    delete *iterator;
  modules_.clear ();

  size_ = 5.0f * (std::ceil (Common_Tools::getRandomNumber (0.0f, 1.0f) * 16.0f) + 2.0f);
  int totalX =
    static_cast<int> (std::floor ((olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_429_DEFAULT_MARGIN * 2) / size_));
  int totalY =
    static_cast<int> (std::floor ((olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_429_DEFAULT_MARGIN * 2) / size_));

  olc::vf2d margin_s ((olc::PixelGameEngine::ScreenWidth ()  - totalX * size_) / 2.0f,
                      (olc::PixelGameEngine::ScreenHeight () - totalY * size_) / 2.0f);

  static std::vector<olc::Pixel> palette_a = { olc::Pixel (0xF2, 0x7E, 0xA9), olc::Pixel (0x36, 0x6C, 0xD9), olc::Pixel (0x5E, 0xAD, 0xF2),
                                               olc::Pixel (0x63, 0x6E, 0x73), olc::Pixel (0xF2, 0xE6, 0xD8) };
  for (int i = 0; i < totalX; i++)
    for (int j = 0; j < totalY; j++)
      modules_.push_back (new module (olc::vi2d (i, j), size_, margin_s, palette_a, this));
}

//////////////////////////////////////////

void
draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color, olc::PixelGameEngine* engine)
{
  std::vector<olc::vf2d> points_arc;

  for (int i = 0; i <= ENGINE_PGE_429_DEFAULT_ARC_RESOLUTION + 1; ++i)
  {
    float angle_point =
      (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_429_DEFAULT_ARC_RESOLUTION));
    olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
    point_s *= radius;
    point_s += center;
    points_arc.push_back (point_s);
  } // end FOR

  for (int i = 0; i <= ENGINE_PGE_429_DEFAULT_ARC_RESOLUTION; i++)
    engine->DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
}
