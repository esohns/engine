#include "stdafx.h"

#include "pge_405.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_18.h"

PGE_405::PGE_405 ()
 : olc::PixelGameEngine ()
 , cr_ ()
 , beta_ ()
 , theta_ ()
 , zeta_ ()
 , eta_ ()
{
  sAppName = "Example 405";
}

bool
PGE_405::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_405_DEFAULT_N; i++)
  {
    cr_.push_back ((i * 2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_405_DEFAULT_N));
    beta_.push_back ((i * 2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_405_DEFAULT_N));
    theta_.push_back ((i * 2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_405_DEFAULT_N));
    zeta_.push_back ((i * 2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_405_DEFAULT_N) + static_cast<float> (M_PI) / 4.0f);
    eta_.push_back ((i * 2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_405_DEFAULT_N) + static_cast<float> (M_PI) / 4.0f);
  } // end FOR

  return true;
}

bool
PGE_405::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_405_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_405_DEFAULT_ALPHA_DECAY : 0);

  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);
  for (int i = 0; i < ENGINE_PGE_405_DEFAULT_N; i++)
  {
    theta_[i] += static_cast<float> (M_PI) / 250.0f;
    zeta_[i] += static_cast<float> (M_PI) / 200.0f;
    if (theta_[i] >= static_cast<float> (M_PI))
      beta_[i] += static_cast<float> (M_PI) / 350.0f;
    if (zeta_[i] >= 2.0f * static_cast<float> (M_PI))
      eta_[i] += static_cast<float> (M_PI) / 300.0f;

    float x1 = ENGINE_PGE_405_DEFAULT_R * cr_[i] * std::cos (theta_[i]);
    float y1 = ENGINE_PGE_405_DEFAULT_R * cr_[i] * std::sin (theta_[i]);
    float x2 = ENGINE_PGE_405_DEFAULT_R * cr_[i] * std::cos (beta_[i]);
    float y2 = ENGINE_PGE_405_DEFAULT_R * cr_[i] * std::sin (beta_[i]);
    float x3 = ENGINE_PGE_405_DEFAULT_G * cr_[i] * std::cos (zeta_[i]);
    float y3 = ENGINE_PGE_405_DEFAULT_G * cr_[i] * std::sin (zeta_[i]);
    float x4 = ENGINE_PGE_405_DEFAULT_G * cr_[i] * std::cos (eta_[i]);
    float y4 = ENGINE_PGE_405_DEFAULT_G * cr_[i] * std::sin (eta_[i]);

    olc::vf2d xy1 (x1, y1);
    olc::vf2d xy2 (x2, y2);
    olc::vf2d xy3 (x3, y3);
    olc::vf2d xy4 (x4, y4);

    olc::PixelGameEngine::DrawLine (center_s + xy1, center_s + xy2, olc::WHITE, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (center_s + xy3, center_s + xy4, olc::WHITE, 0xFFFFFFFF);

    if (theta_[i] >= 3.0f * 2.0f * static_cast<float> (M_PI))
      theta_[i] = 0.0f;
    if (theta_[i] <= static_cast<float> (M_PI))
      beta_[i] -= static_cast<float> (M_PI) / 175.0f;
    if (zeta_[i] >= 5.0f * 2.0f * static_cast<float> (M_PI))
      zeta_[i] = 0.0f;
    if (zeta_[i] <= 2.0f * static_cast<float> (M_PI))
      eta_[i] -= static_cast<float> (M_PI) / 150.0f;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
