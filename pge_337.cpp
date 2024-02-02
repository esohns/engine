#include "stdafx.h"

#include "pge_337.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_337::PGE_337 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , particles_ ()
 , t_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 337");
}

bool
PGE_337::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  struct particle particle_s;
  particle_s.r = 0.0f;
  particle_s.d = 0.0f;
  particles_.assign ((ENGINE_PGE_337_DEFAULT_WIDTH * 3) + 90, particle_s);

  return true;
}

bool
PGE_337::OnUserUpdate (float fElapsedTime)
{
  static olc::vi2d center_s (olc::PixelGameEngine::ScreenWidth () / 2,
                             olc::PixelGameEngine::ScreenHeight () / 2);

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a >= ENGINE_PGE_337_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_337_DEFAULT_ALPHA_DECAY : 0);

  if (t_ % 30 == 0)
    for (int i = 90; i--;)
    {
      struct particle particle_s;
      particle_s.r = i / 90.0f * (2.0f * static_cast<float> (M_PI));
      particle_s.d = 0.0f;
      particles_[(t_ * 3) % (ENGINE_PGE_337_DEFAULT_WIDTH * 3) + i] = particle_s;
    } // end FOR

  for (std::vector<struct particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    olc::Pixel color (olc::WHITE);
    color.a = static_cast<uint8_t> (((*iterator).d / 16.0f)) % 256;

    static int A = 90;
    float N =
      //Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((*iterator).d / static_cast<float> (A) - t_ / static_cast<float> (A),
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate ((*iterator).d / static_cast<float> (A) - t_ / static_cast<float> (A),
                                                                 std::sin ((*iterator).r * 2.0f + t_ / static_cast<float> (A)),
                                                                 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
    (*iterator).r += N / 40.0f;
    float S = static_cast<float> (std::pow (N, 4)) * A;
    (*iterator).d += S;
    olc::PixelGameEngine::FillCircle (center_s.x + static_cast<int32_t> (std::cos ((*iterator).r) * (*iterator).d),
                                      center_s.y + static_cast<int32_t> (std::sin ((*iterator).r) * (*iterator).d),
                                      static_cast<int32_t> (S * 2.0f),
                                      color);
  } // end FOR

  ++t_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
