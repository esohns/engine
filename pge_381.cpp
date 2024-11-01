#include "stdafx.h"

#include "pge_381.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_381::PGE_381 ()
 : olc::PixelGameEngine ()
 , R_ (0.0f)
 , minR_ (0.0f)
 , maxR_ (0.0f)
 , mode_ (MODE_PICTURE)
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 381");
}

bool
PGE_381::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  generate ();

  return true;
}

bool
PGE_381::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    generate ();
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
  {
    mode_ = (mode_ == MODE_ELEMENTS) ? MODE_PICTURE : MODE_ELEMENTS;
    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  if (unlikely (mode_ == MODE_ELEMENTS))
    olc::PixelGameEngine::Clear (olc::BLACK);
  else
  {
    //int pixels =
    //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
    //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
    //for (int i = 0; i < pixels; i++)
    //  p[i].a = (p[i].a > ENGINE_PGE_381_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_381_DEFAULT_ALPHA_DECAY : 0);
  } // end ELSE

  simulate ();

  if (unlikely (mode_ == MODE_ELEMENTS))
    for (std::vector<particle>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      (*iterator).draw (this);
  else
  {
    int n = static_cast<int> (particles_.size ());
    for (int i = 0; i < n; i++)
    {
      particle& a = particles_[i];
      for (int j = i + 1; j < n; j++)
      {
        particle& b = particles_[j];
        if (std::abs (a.penetration (b)) < minR_ * ENGINE_PGE_381_DEFAULT_DRAW_RATIO)
        {
          uint8_t color_i =
            static_cast<uint8_t> (Common_GL_Tools::map (std::pow (a.radius_ + b.radius_, ENGINE_PGE_381_DEFAULT_COLOR_RATIO_POWER),
                                                        std::pow (2.0f * minR_, ENGINE_PGE_381_DEFAULT_COLOR_RATIO_POWER), std::pow (2.0f * maxR_, ENGINE_PGE_381_DEFAULT_COLOR_RATIO_POWER),
                                                        0.0f, 255.0f));
          olc::Pixel color (color_i, color_i, color_i, 20);
          olc::PixelGameEngine::DrawLine (a.position_, b.position_, color, 0xFFFFFFFF);
        } // end IF
      } // end FOR
    } // end FOR
  } // end ELSE

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

PGE_381::particle
PGE_381::randomElement ()
{
  olc::vf2d direction (1.0f, 0.0f);
  float angle_f = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
  float x_rotated = std::cos (angle_f) * direction.x - std::sin (angle_f) * direction.y;
  float y_rotated = std::sin (angle_f) * direction.x + std::cos (angle_f) * direction.y;
  direction.x = x_rotated;
  direction.y = y_rotated;

  olc::vf2d v = direction * Common_Tools::getRandomNumber (0.0f, R_ * ENGINE_PGE_381_DEFAULT_CREATION_SPREAD);
  static float half_width_f = static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) / 2.0f;
  static float half_height_f = static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) / 2.0f;
  v += olc::vf2d (half_width_f, half_height_f);

  direction *= ENGINE_PGE_381_DEFAULT_SPEED;

  return particle (v.x, v.y, Common_Tools::getRandomNumber (minR_, maxR_), &direction);
}

void
PGE_381::generate ()
{
  R_ =
    std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) / 2.0f;
  float area_f = static_cast<float> (M_PI) * (R_ * R_);
	int n = Common_Tools::getRandomNumber (100, 400);
	float circleAreaRatio = Common_Tools::getRandomNumber (2.0f, 30.0f);
  float avgCircleArea = area_f / static_cast<float> (n);
  float minCircleArea = 2.0f * avgCircleArea / (circleAreaRatio + 1.0f);
  minR_ = std::sqrt (minCircleArea) / static_cast<float> (M_PI);
  maxR_ = std::sqrt (minCircleArea * circleAreaRatio) / static_cast<float> (M_PI);
	
  particles_.clear ();
  for (int i = 0; i < n; i++)
    particles_.push_back (randomElement ());

  olc::PixelGameEngine::Clear (olc::BLACK);
}

void
PGE_381::simulate ()
{
  std::vector<particle> newElements;

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator).move ();

    static float half_width_f = static_cast<float> (olc::PixelGameEngine::ScreenWidth()) / 2.0f;
    static float half_height_f = static_cast<float> (olc::PixelGameEngine::ScreenHeight()) / 2.0f;
    if ((*iterator).inCircle (half_width_f, half_height_f, R_))
      newElements.push_back (*iterator);
    else
      newElements.push_back (randomElement ());
  } // end FOR

  particles_ = newElements;

  int n = static_cast<int> (particles_.size ());
  for (int i = 0; i < n; i++)
  {
    particle& a = particles_[i];
    for (int j = i + 1; j < n; j++)
    {
      particle& b = particles_[j];
      a.interact (b, minR_);
      b.interact (a, minR_);
    } // end FOR
  } // end FOR
}
