#include "stdafx.h"

#include "pge_155.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_155::PGE_155 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , particles_ ()
 , t_ (0.0f)
{
  sAppName = "Example 155";
}

bool
PGE_155::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_155_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (particle (this));

  return true;
}

bool
PGE_155::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  t_ = frame_count_i / 300.0f;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < static_cast<int> (particles_.size ()); i++)
  {
    float n = Common_GL_Tools::map (static_cast<float> (noise_.GetValue (particles_[i].position_.x / 1000.0,
    //float n = Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (particles_[i].position_.x / 1000.0,
                                                                         particles_[i].position_.y / 1000.0 + t_,
                                                                         0.0)),
                                    -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI) * 4.0f;
    particles_[i].position_.x += std::cos (n) * ENGINE_PGE_155_DEFAULT_STEP;
    particles_[i].position_.y += std::sin (n) * ENGINE_PGE_155_DEFAULT_STEP;

    if (particles_[i].size_ < 2.0f ||
        particles_[i].position_.x < 0 || particles_[i].position_.y < 0 ||
        particles_[i].position_.x > olc::PixelGameEngine::ScreenWidth () - 1 || particles_[i].position_.y > olc::PixelGameEngine::ScreenHeight () - 1)
    {
      particles_[i].position_.x = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
      particles_[i].position_.y = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
      particles_[i].size_ = 2.0f;
    } // end IF

    olc::Pixel color;
    float r, g, b;
    Common_Image_Tools::HSVToRGB (particles_[i].size_ * 3.0f, 1.0f, 1.0f, r, g, b);
    color.r = static_cast<uint8_t> (r * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);
    olc::PixelGameEngine::FillCircle (particles_[i].position_, static_cast<int32_t> (particles_[i].size_ / 2.0f), color);

    float ratio = 1.06f; // slightly grow particles until there is a collision
    for (int j = i + 1; j < static_cast<int> (particles_.size ()); j++)
    {
      float dis = particles_[i].position_.dist (particles_[j].position_);
      if (dis < particles_[i].size_ || dis < particles_[j].size_)
      { // --> collision: shrink both particles
        ratio = 0.92f;
        particles_[j].size_ *= ratio;
        break;
      } // end IF
    } // end FOR
    particles_[i].size_ *= ratio;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
