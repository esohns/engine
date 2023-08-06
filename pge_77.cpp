#include "stdafx.h"

#include "pge_77.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_77::PGE_77 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , particles_ ()
{
  sAppName = "Example 77";
}

bool
PGE_77::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  particle particle_s;
  for (int i = 0; i < ENGINE_PGE_77_DEFAULT_NUMBER_OF_PARTICLES; ++i)
    particles_.push_back (particle_s);

  return true;
}

bool
PGE_77::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_77_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_77_DEFAULT_ALPHA_DECAY : 0);

  static int frame_count_i = 1;

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  float t_f =
    static_cast<float> (frame_count_i % ENGINE_PGE_77_DEFAULT_WIDTH) / static_cast<float> (ENGINE_PGE_77_DEFAULT_WIDTH);
  t_f += std::log (static_cast<float> (std::pow (t_f, 4)));
  struct particle particles_s;
  particles_s.position_.x = (std::cos (t_f) * t_f * 9.0f) + half_width_i;
  particles_s.position_.y = (std::sin (t_f) * t_f * 9.0f) + half_height_i;
  particles_s.t_ = t_f;
  particles_[frame_count_i % ENGINE_PGE_77_DEFAULT_NUMBER_OF_PARTICLES] = particles_s;

  uint8_t color_i = static_cast<uint8_t> (ENGINE_PGE_77_DEFAULT_WIDTH);
  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    t_f =
      std::tan (((*iterator).t_ * ENGINE_PGE_77_DEFAULT_NUMBER_OF_PARTICLES) - (frame_count_i / 99.0f)) * 90.0f;
    olc::Pixel color = {color_i, color_i, color_i, static_cast<uint8_t> (t_f)};
    int32_t radius_i = static_cast<int32_t> (ENGINE_PGE_77_DEFAULT_WIDTH / t_f);
    if (radius_i > 50) radius_i = 50;
    olc::PixelGameEngine::FillCircle ((*iterator).position_, radius_i, color);
    float r_f =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((*iterator).t_, frame_count_i / static_cast<double> (ENGINE_PGE_77_DEFAULT_WIDTH), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 9.0f;
    (*iterator).position_.x += std::cos (r_f) * t_f / 99.0f;
    (*iterator).position_.y += std::sin (r_f) * t_f / 99.0f;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
