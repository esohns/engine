#include "stdafx.h"

#include "pge_276.h"

#include <random>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_276::PGE_276 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , travelers_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 276");
}

bool
PGE_276::OnUserCreate ()
{
  static float d = 15.0f;
  olc::vf2d p;
  points_.push_back (p);
  int c = 0;
  while (c < 2000)
  {
    float r =
      Common_Tools::getRandomNumber (-std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) / 2.5f, 0.0f);
    float a = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    p.x = r * std::cos (a);
    p.y = r * std::sin (a);

    c++;
    bool b = true;
    for (int i = 0; i < static_cast<int> (points_.size ()); i++)
      if (p.dist (points_[i]) < d)
        b = false;
    if (b)
      points_.push_back (p);
  } // end WHILE

  travelers_.push_back (traveler (0.0f, 0.0f,
                                  Common_Tools::getRandomNumber (20.0f, olc::PixelGameEngine::ScreenWidth () / 4.0f),
                                  0.0f,
                                  ENGINE_PGE_276_DEFAULT_SPEED,
                                  30.0f));

  return true;
}

bool
PGE_276::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  float t = frame_count_i / 30.0f;
  //rotate(t);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::vf2d mouse_pos (static_cast<float> (olc::PixelGameEngine::GetMouseX () - half_width_i),
                         static_cast<float> (olc::PixelGameEngine::GetMouseY () - half_height_i));
    static olc::vf2d center_s;
    if (mouse_pos.dist (center_s) < std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) / 2.2f)
    {
      float r = std::sqrt (mouse_pos.x * mouse_pos.x + mouse_pos.y * mouse_pos.y);
      float a;
      if (mouse_pos.x >= 0.0f)
        a = std::asin (mouse_pos.y / r);
      else if (mouse_pos.x < 0.0f && mouse_pos.y > 0.0f)
        a = std::acos (mouse_pos.x / r);
      else
        a = static_cast<float> (M_PI) - std::asin (mouse_pos.y / r);

      static std::random_device rd;
      static std::mt19937 m (rd ());
      static std::normal_distribution<float> dist (30.0f, 10.0f);

      travelers_.push_back (traveler (0.0f, 0.0f,
                                      r,
                                      a - t,
                                      Common_Tools::getRandomNumber (-ENGINE_PGE_276_DEFAULT_SPEED, ENGINE_PGE_276_DEFAULT_SPEED),
                                      dist (m)));
    } // end IF
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
  {
    points_.clear ();
    travelers_.clear ();
    OnUserCreate ();
  } // end IF

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < static_cast<int> (travelers_.size ()); i++)
  {
    travelers_[i].position_.x = travelers_[i].r_ * std::cos (travelers_[i].a_ + travelers_[i].t_);
    travelers_[i].position_.y = travelers_[i].r_ * std::sin (travelers_[i].a_ + travelers_[i].t_);
    travelers_[i].t_ += travelers_[i].speed_;
  } // end FOR

  for (int i = 0; i < static_cast<int> (points_.size ()); i++)
  {
    static olc::Pixel color (100, 150, 200);
    olc::PixelGameEngine::FillCircle (half_width_i + static_cast<int32_t> (points_[i].x), half_height_i + static_cast<int32_t> (points_[i].y),
                                      1, color);
  } // end FOR

  for (int i = 0; i < static_cast<int> (points_.size ()); i++)
    for (int j = 0; j < static_cast<int> (travelers_.size ()); j++)
    {
      float s = points_[i].dist (travelers_[j].position_);
      if (s < travelers_[j].d_)
      {
        float rd =
          Common_GL_Tools::map (travelers_[j].position_.x, -olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenWidth () / 2.0f, 50.0f, 200.0f);
        float gr =
          Common_GL_Tools::map (travelers_[j].position_.y, -olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenWidth () / 2.0f, 80.0f, 150.0f);
        float bl =
          Common_GL_Tools::map (travelers_[j].position_.y, -olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenWidth () / 2.0f, 100.0f, 250.0f);
        olc::Pixel color (static_cast<uint8_t> (rd), static_cast<uint8_t> (gr), static_cast<uint8_t> (bl), 100);
        olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (points_[i].x), half_height_i + static_cast<int32_t> (points_[i].y),
                                        half_width_i + static_cast<int32_t> (travelers_[j].position_.x), half_height_i + static_cast<int32_t> (travelers_[j].position_.y),
                                        color, 0xFFFFFFFF);
      } // end IF
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
