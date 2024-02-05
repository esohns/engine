#include "stdafx.h"

#include "pge_344.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_344::PGE_344 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , particles_ ()
 , t_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 344");
}

bool
PGE_344::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                      olc::PixelGameEngine::ScreenHeight () / 2.0f);
  struct particle particle_s;
  particle_s.position = center_s;
  particle_s.r = 0.0f;
  particle_s.l = 0;
  particles_.assign (ENGINE_PGE_344_DEFAULT_WIDTH * 2, particle_s);

  return true;
}

bool
PGE_344::OnUserUpdate (float fElapsedTime)
{
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a >= ENGINE_PGE_344_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_344_DEFAULT_ALPHA_DECAY : 0);

  struct particle particle_s;
  particle_s.position = center_s;
  particle_s.r = static_cast<float> (t_);
  particle_s.l = Common_Tools::getRandomNumber (0, 99);
  particles_[t_ % (ENGINE_PGE_344_DEFAULT_WIDTH * 2)] = particle_s;

  olc::Pixel color (olc::WHITE);
  for (std::vector<struct particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    float A = (*iterator).l++ / 99.0f;
    float T = static_cast<float> (std::pow (std::tan (A), 2)) * 99.0f;
    color.a = static_cast<uint8_t> (std::fmod (T, 255.0f));
    float radius_f = std::min (std::max (((*iterator).l / T) / 3.0f + 2.0f, 1.0f), 5.0f);
    olc::PixelGameEngine::FillCircle ((*iterator).position,
                                      static_cast<int32_t> (radius_f),
                                      color);

    float N =
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate ((*iterator).position.x / 99.0f,
                                                                 (*iterator).position.y / 99.0f,
                                                                 t_ / static_cast<float> (ENGINE_PGE_344_DEFAULT_WIDTH))), -1.0f, 1.0f, 0.0f, 1.0f);
    float R = (*iterator).r + N * 9.0f;
    float D =
      static_cast<float> (std::pow (std::sin (A), 8)) * ((*iterator).l / 360.0f);
    (*iterator).position.x += std::cos (R) * D;
    (*iterator).position.y += std::sin (R) * D;
  } // end FOR

  ++t_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
