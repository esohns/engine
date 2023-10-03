#include "stdafx.h"

#include "pge_190.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_190::PGE_190 ()
 : olc::PixelGameEngine ()
 , color_ ()
 , dMin_ (0.0f)
 , rMax_ (0.0f)
 , circles_ (ENGINE_PGE_190_DEFAULT_NUMBER_OF_CIRCLES, circle ())
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 190");
}
 
bool
PGE_190::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  dMin_ = std::max (olc::PixelGameEngine::ScreenWidth () - 1, olc::PixelGameEngine::ScreenHeight () - 1) / 3.6f;
  rMax_ = std::min (olc::PixelGameEngine::ScreenWidth () - 1, olc::PixelGameEngine::ScreenHeight () - 1) / 2.0f;

  initialize (false);

  return true;
}

bool
PGE_190::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    initialize (false);
  if (olc::PixelGameEngine::GetMouse (1).bPressed)
    initialize (true);

  color_.update ();

  for (size_t j = 0; j < circles_.size (); j++)
  {
    circles_[j].update (rMax_);

    for (size_t i = j + 1; i < circles_.size (); i++)
    {
      float rCoeff =
        Common_GL_Tools::map (std::min (std::abs (circles_[j].radius_), std::abs (circles_[i].radius_)), 0.0f, rMax_, 0.08f, 1.0f);
      for (int l = 0; l < circles_[j].nbLines_; l++)
      {
        olc::vf2d p1 = {
          circles_[j].position_.x + circles_[j].radius_ * std::cos (l * 2.0f * static_cast<float> (M_PI) / static_cast<float> (circles_[j].nbLines_) + circles_[j].theta_),
          circles_[j].position_.y + circles_[j].radius_ * std::sin (l * 2.0f * static_cast<float> (M_PI) / static_cast<float> (circles_[j].nbLines_) + circles_[j].theta_) };
        for (int k = 0; k < circles_[i].nbLines_; k++)
        {
          olc::vf2d p2 = {
          circles_[i].position_.x + circles_[i].radius_ * std::cos (k * 2.0f * static_cast<float> (M_PI) / static_cast<float> (circles_[i].nbLines_) + circles_[i].theta_),
          circles_[i].position_.y + circles_[i].radius_ * std::sin (k * 2.0f * static_cast<float> (M_PI) / static_cast<float> (circles_[i].nbLines_) + circles_[i].theta_) };

          float d = p1.dist (p2);
          if (d < dMin_)
          {
            olc::Pixel color = {
              static_cast<uint8_t> (color_.R_ + circles_[i].radius_ / 1.5f),
              static_cast<uint8_t> (color_.G_ + circles_[i].radius_ / 2.2f),
              static_cast<uint8_t> (color_.B_ + circles_[i].radius_ / 1.5f),
              static_cast<uint8_t> (Common_GL_Tools::map (d, 0.0f, dMin_, 140.0f, 0.0f) * rCoeff)
            };
            olc::PixelGameEngine::DrawLine (static_cast<int32_t> (p1.x) + half_width_i, static_cast<int32_t> (p1.y) + half_height_i,
                                            static_cast<int32_t> (p2.x) + half_width_i, static_cast<int32_t> (p2.y) + half_height_i,
                                            color, 0xFFFFFFFF);
          } // end IF
        } // end FOR
      } // end FOR
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_190::initialize (bool random_in)
{
  for (size_t i = 0; i < circles_.size (); i++)
    circles_[i] = circle (Common_Tools::getRandomNumber (0.0f, rMax_),
                          random_in ? Common_Tools::getRandomNumber (-(olc::PixelGameEngine::ScreenWidth ()  - 1) / 3.0f, olc::PixelGameEngine::ScreenWidth ()  - 1 / 3.0f) : 0.0f,
                          random_in ? Common_Tools::getRandomNumber (-(olc::PixelGameEngine::ScreenHeight () - 1) / 3.0f, olc::PixelGameEngine::ScreenHeight () - 1 / 3.0f) : 0.0f);

  color_ = color ();
}
