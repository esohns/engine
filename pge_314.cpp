#include "stdafx.h"

#include "pge_314.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_314::PGE_314 ()
 : olc::PixelGameEngine ()
 , QMax_ (0)
 , T_ (0.0f)
 , KurosawaMode_ (-1)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 314");
}

bool
PGE_314::OnUserCreate ()
{
  X_ = -400;
  Y_ = -400;
  AE_ = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI_2));
  S_ = 0;

  return true;
}

bool
PGE_314::OnUserUpdate (float fElapsedTime)
{
  static float elapsed_time_f = 0.0f;
  static olc::vi2d center_s (olc::PixelGameEngine::ScreenWidth () / 2,
                             olc::PixelGameEngine::ScreenHeight () / 2);

  for (Q_ = 0; Q_ < QMax_; Q_++)
  {
    KX_ = X_ + 0.01f; KY_ = Y_ + 0.01f;
    R_ = std::sqrt (static_cast<float> (std::pow (KX_, 2) + std::pow (KY_, 2)));
    if (R_ < 400.0f)
    {
      for (I_ = 0; I_ < S_; I_++)
      {
        if (KX_ > 0.0f && KY_ > 0.0f) { A_ = std::atan (KY_ / KX_); }
        if (KX_ <= 0.0f && KY_ > 0.0f) { A_ = std::atan (KY_ / KX_) + static_cast<float> (M_PI); }
        if (KX_ <= 0.0f && KY_ <= 0.0f) { A_ = std::atan (KY_ / KX_) + static_cast<float> (M_PI); }
        if (KX_ > 0.0f && KY_ <= 0.0f) { A_ = std::atan (KY_ / KX_) + (2.0f * static_cast<float> (M_PI)); }
        if (A_ < AE_) { A_ = A_ + (2.0f * static_cast<float> (M_PI)); }
        A_ = (A_ - AE_) / 2.0f;
        KX_ = R_ * std::cos (A_);
        KY_ = R_ * std::sin (A_);
        H_ = std::sqrt (static_cast<float> (std::pow (400, 2) - std::pow (KX_, 2)));
        KY_ = H_ - ((H_ - KY_) * 2.0f);
        R_ = std::sqrt (static_cast<float> (std::pow (KX_, 2) + std::pow (KY_, 2)));
      }//I
      olc::Pixel color;
      if (KurosawaMode_ < 0)
      {
        color.r = static_cast<uint8_t> (125 * std::sin (R_ / 5.0f) + 125);
        color.g = static_cast<uint8_t> (125 * std::sin (R_ / 7.0f) + 125);
        color.b = static_cast<uint8_t> (125 * std::sin (R_ / 11.0f) + 125);
      } // end IF
      else
      {
        color.r = static_cast<uint8_t> (125 * std::sin (R_ / 11.0f) + 125);
        color.g = color.b = color.r;
      } // end ELSE
      olc::PixelGameEngine::Draw (X_ + center_s.x, Y_ + center_s.y, color);
    }//R

    X_ = X_ + 1; if (X_ >= 400) { X_ = -400; Y_ = Y_ + 1; }
    if (Y_ >= 400) { Y_ = -400; S_ = S_ + 1; }
  }//Q

  if (elapsed_time_f - T_ < 0.02f) { QMax_ = QMax_ + 10; }
  else { QMax_ = QMax_ - 10; }
  T_ = elapsed_time_f;

  if (olc::PixelGameEngine::GetMouse(0).bPressed || S_ > 40) { KurosawaMode_ = KurosawaMode_ * (-1); OnUserCreate (); }

  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
