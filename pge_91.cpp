#include "stdafx.h"

#include "pge_91.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_91::PGE_91 ()
 : olc::PixelGameEngine ()
 , fade_ (0.0f)
 , fadeAmount_ (1.0f)
 , particles_ ()
 , noise_ ()
{
  sAppName = "Example 91";
}

bool
PGE_91::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  olc::PixelGameEngine::Clear ({100, 70, 40, static_cast<uint8_t> (0.07f * 255.0f)});

  particle particle_s;
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i += 20)
    for (int o = 0; o < olc::PixelGameEngine::ScreenHeight (); o += 5)
    {
      particle_s.position_ = {static_cast<float> (i), static_cast<float> (o)};
      particle_s.color_ = {static_cast<uint8_t> (olc::PixelGameEngine::GetMouseX () * 0.1f),
                           static_cast<uint8_t> (olc::PixelGameEngine::GetMouseY () * 0.4f + 0), // frame_count_i ?
                           250,
                           250};
      particles_.push_back (particle_s);
    } // end FOR

  return true;
}

bool
PGE_91::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::Pixel color_s;
  for (size_t i = 0; i < particles_.size (); i++)
  {
    color_s.r = static_cast<uint8_t> (olc::PixelGameEngine::GetMouseX () * 0.5f);
    color_s.g = static_cast<uint8_t> (olc::PixelGameEngine::GetMouseY () * 0.5f + frame_count_i);
    color_s.b = static_cast<uint8_t> (fade_);
    color_s.a = static_cast<uint8_t> (fade_);
    if (fade_ < 0.0f) fadeAmount_ = 1.0f;
    if (fade_ > 255.0f) fadeAmount_ = -10.0f;
    fade_ += fadeAmount_;
    olc::PixelGameEngine::Draw (static_cast<int32_t> (particles_[i].position_.x + 30),
                                static_cast<int32_t> (particles_[i].position_.y + 30),
                                color_s);

    particles_[i].position_.x +=
      (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (particles_[i].position_.x / 200.0,
                                                                  particles_[i].position_.y / 200.0,
                                                                  3000.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.6f) * 3.0f;
    particles_[i].position_.y +=
      (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (particles_[i].position_.x / 200.0,
                                                                  particles_[i].position_.y / 200.0,
                                                                  3000.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * 3.0f;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
