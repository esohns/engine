#include "stdafx.h"

#include "pge_62.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "engine_common.h"

PGE_62::PGE_62 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , n_ (ENGINE_PGE_62_DEFAULT_N)
 , m_ (ENGINE_PGE_62_DEFAULT_M)
 , color_ (olc::WHITE)
 , lastColor_ (olc::WHITE)
{
  sAppName = "Example 62";
}

PGE_62::~PGE_62 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_62::OnUserCreate ()
{
  float r, g, b;
  Common_Image_Tools::HSVToRGB (n_ * 360.0f / static_cast<float> (ENGINE_PGE_62_DEFAULT_MAX_HARMONICS),
                                50.0f + m_ * 50.0f / static_cast<float> (ENGINE_PGE_62_DEFAULT_MAX_HARMONICS),
                                100.0f,
                                r, g, b);
  color_.r = static_cast<uint8_t> (r * 255.0);
  color_.g = static_cast<uint8_t> (g * 255.0);
  color_.b = static_cast<uint8_t> (b * 255.0);
  lastColor_ = color_;

  int num_points_i = (olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ()) / 200;
  if (num_points_i < 2000) num_points_i = 2000;
  else if (num_points_i > ENGINE_PGE_62_DEFAULT_MAX_NUMBER_OF_POINTS) num_points_i = ENGINE_PGE_62_DEFAULT_MAX_NUMBER_OF_POINTS;
  for (int i = 0; i < num_points_i; ++i)
    particles_.push_back (new particle ());

  shuffle ();

  return true;
}

bool
PGE_62::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static float seconds_f = 0.0f;
  seconds_f += fElapsedTime;
  if (seconds_f > lastChange_ + ENGINE_PGE_62_DEFAULT_INTERVAL) // change pattern every few seconds
  {
    lastChange_ = seconds_f;

    shuffle ();

    for (int i = 0; i < static_cast<int> (particles_.size ()); i += 2)
      particles_[i]->randomize ();

    lastColor_ = color_;
    float r, g, b;
    Common_Image_Tools::HSVToRGB (n_ * 360.0f / static_cast<float> (ENGINE_PGE_62_DEFAULT_MAX_HARMONICS),
                                  50.0f + m_ * 50.0f / static_cast<float> (ENGINE_PGE_62_DEFAULT_MAX_HARMONICS),
                                  100.0f,
                                  r, g, b);
    color_.r = static_cast<uint8_t> (r * 255.0);
    color_.g = static_cast<uint8_t> (g * 255.0);
    color_.b = static_cast<uint8_t> (b * 255.0);
  } // end IF

  float amount_f =
    Common_GL_Tools::map (seconds_f - lastChange_, 0.0f, ENGINE_PGE_62_DEFAULT_INTERVAL, 0.0f, 1.0f);
  Common_GL_Color_t color1, color2;
  color1.r = lastColor_.r;
  color1.g = lastColor_.g;
  color1.b = lastColor_.b;
  color2.r = color_.r;
  color2.g = color_.g;
  color2.b = color_.b;
  Common_GL_Color_t lerped_color = Common_GL_Tools::lerpRGB (color1, color2, amount_f);
  olc::Pixel color;
  color.r = lerped_color.r;
  color.g = lerped_color.g;
  color.b = lerped_color.b;
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator)->update (n_, m_);
    (*iterator)->show (this,
                       color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_62::shuffle ()
{
  n_ = Common_Tools::getRandomNumber (0, ENGINE_PGE_62_DEFAULT_MAX_HARMONICS);
  m_ = Common_Tools::getRandomNumber (0, ENGINE_PGE_62_DEFAULT_MAX_HARMONICS);
  while (m_ == n_)
    m_ = Common_Tools::getRandomNumber (0, ENGINE_PGE_62_DEFAULT_MAX_HARMONICS);
}
