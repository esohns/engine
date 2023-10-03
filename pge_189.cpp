#include "stdafx.h"

#include "pge_189.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_189::PGE_189 ()
 : olc::PixelGameEngine ()
 , angle_ (0.0f)
 , p0_ ()
 , points_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 189");
}
 
bool
PGE_189::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  p0_.x = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
  p0_.y = 0.0f;
  angle_ = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_189::OnUserUpdate (float fElapsedTime)
{
  olc::vf2d point_s;
  point_s.x =
    static_cast<float> (static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () + p0_.x + ENGINE_PGE_188_DEFAULT_STEP * std::cos (angle_)) % olc::PixelGameEngine::ScreenWidth ());
  point_s.y =
    static_cast<float> (static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () + p0_.y + ENGINE_PGE_188_DEFAULT_STEP * std::sin (angle_)) % olc::PixelGameEngine::ScreenHeight ());
  olc::vf2d* pt = &point_s;

  for (std::vector<olc::vf2d>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
    if (pt->dist (*iterator) < ENGINE_PGE_188_DEFAULT_MIN_DIST)
    {
      if (points_.size () > 1080)
        pt = &points_[Common_Tools::getRandomNumber (0, 1080)];
      else
      {
        point_s.x =
          Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
        point_s.y = 0.0f;
        pt = &point_s;
      } // end ELSE
      break;
    } // end IF

  olc::Pixel color = {0, 0, 0, 80};
  for (std::vector<olc::vf2d>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
    if (pt->dist (*iterator) < ENGINE_PGE_188_DEFAULT_MAX_DIST)
      olc::PixelGameEngine::DrawLine (*iterator, *pt, color, 0xFFFFFFFF);

  points_.push_back (*pt);
  p0_ = *pt;
  angle_ += Common_Tools::getRandomNumber (-0.2f, 0.2f);

  shadow ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_189::shadow ()
{
  olc::vf2d point_s;
  point_s.x =
    static_cast<float> (static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () + p0_.x + ENGINE_PGE_188_DEFAULT_STEP * std::cos (angle_)) % olc::PixelGameEngine::ScreenWidth ());
  point_s.y =
    static_cast<float> (static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () + p0_.y + ENGINE_PGE_188_DEFAULT_STEP * std::sin (angle_)) % olc::PixelGameEngine::ScreenHeight ());
  point_s.x += 8.0f;
  point_s.y += 8.0f;
  olc::vf2d* pt = &point_s;

  for (std::vector<olc::vf2d>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
    if (pt->dist (*iterator) < ENGINE_PGE_188_DEFAULT_MIN_DIST)
    {
      if (points_.size () > 1080)
        pt = &points_[Common_Tools::getRandomNumber (0, 1080)];
      else
      {
        point_s.x =
          Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
        point_s.y = 0.0f;
        pt = &point_s;
      } // end ELSE
      break;
    } // end IF

  olc::Pixel color = {100, 100, 100, 1};
  for (std::vector<olc::vf2d>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
    if (pt->dist (*iterator) < ENGINE_PGE_188_DEFAULT_MAX_DIST)
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (iterator->x) + 20, static_cast<int32_t> (iterator->y) + 20,
                                      static_cast<int32_t> (pt->x)       + 20, static_cast<int32_t> (pt->y)       + 20,
                                      color, 0xFFFFFFFF);
}
