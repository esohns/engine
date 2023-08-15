#include "stdafx.h"

#include "pge_108.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_108::PGE_108 ()
 : olc::PixelGameEngine ()
 , R_ (2.0f * static_cast<float> (std::sqrt ((4.0f * static_cast<float> (M_PI) * (200.0f * 200.0f) / ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS) / (2.0f * static_cast<float> (std::sqrt (3.0f))))))
{
  sAppName = "Example 108";
}

bool
PGE_108::OnUserCreate ()
{
  for (N_ = 0; N_ <= ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS; N_++)
  {
    X_[N_] = Common_Tools::getRandomNumber (-300.0f, 300.0f);
    Y_[N_] = Common_Tools::getRandomNumber (-300.0f, 300.0f);
    Z_[N_] = Common_Tools::getRandomNumber (-300.0f, 300.0f);
  } // end FOR
  ACE_OS::memset (V_, 0, sizeof (float) * ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1);
  ACE_OS::memset (dV_, 0, sizeof (float) * ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS + 1);
  L_ = 0.0f;
  Lmin_ = 600.0f;
  KX_ = 0.0f;
  KY_ = 0.0f;
  KZ_ = 0.0f;
  KV_ = 0.0f;
  KdV_ = 0.0f;
  K_ = 0;

  return true;
}

bool
PGE_108::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    Lmin_ = 600.0f; NN_ = 0;
    for (N_ = 0; N_ <= ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS; N_++)
    {
      L_ =
        std::sqrt (((olc::PixelGameEngine::GetMouseX () - (300 + X_[N_])) * (olc::PixelGameEngine::GetMouseX () - (300 + X_[N_]))) + ((olc::PixelGameEngine::GetMouseY () - (300 + Y_[N_])) * (olc::PixelGameEngine::GetMouseY () - (300 + Y_[N_]))));
      if (Z_[N_] > 0.0f && L_ < Lmin_) { NN_ = N_; Lmin_ = L_; }
    } // end FOR
    if (K_ == 0) { dV_[NN_] = -200.0f; K_ = 1; }
    else { dV_[NN_] = 200.0f; K_ = 0; }
  } // end IF

  olc::PixelGameEngine::Clear (olc::BLACK);

  int32_t x1, x2, y1, y2;
  uint8_t grayscale_i;
  for (N_ = 0; N_ <= ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS; N_++)
  {
    for (NN_ = N_ + 1; NN_ <= ENGINE_PGE_108_DEFAULT_NUMBER_OF_POINTS; NN_++)
    {
      L_ =
        std::sqrt (((X_[N_] - X_[NN_]) * (X_[N_] - X_[NN_])) + ((Y_[N_] - Y_[NN_]) * (Y_[N_] - Y_[NN_])));
      L_ =
        std::sqrt (((Z_[N_] - Z_[NN_]) * (Z_[N_] - Z_[NN_])) + (L_ * L_));
      if (L_ < R_)
      {
        X_[N_] = X_[N_] - ((X_[NN_] - X_[N_]) * ((R_ - L_) / (2 * L_)));
        Y_[N_] = Y_[N_] - ((Y_[NN_] - Y_[N_]) * ((R_ - L_) / (2 * L_)));
        Z_[N_] = Z_[N_] - ((Z_[NN_] - Z_[N_]) * ((R_ - L_) / (2 * L_)));
        X_[NN_] = X_[NN_] + ((X_[NN_] - X_[N_]) * ((R_ - L_) / (2 * L_)));
        Y_[NN_] = Y_[NN_] + ((Y_[NN_] - Y_[N_]) * ((R_ - L_) / (2 * L_)));
        Z_[NN_] = Z_[NN_] + ((Z_[NN_] - Z_[N_]) * ((R_ - L_) / (2 * L_)));
        dV_[N_] = dV_[N_] + ((V_[NN_] - V_[N_]) / ENGINE_PGE_108_DEFAULT_M);
        dV_[NN_] = dV_[NN_] - ((V_[NN_] - V_[N_]) / ENGINE_PGE_108_DEFAULT_M);

        x1 = static_cast<int32_t> ((X_[N_] * 1.2f * (200.0f + V_[N_]) / 200.0f) + 300.0f);
        y1 = static_cast<int32_t> ((Y_[N_] * 1.2f * (200.0f + V_[N_]) / 200.0f) + 300.0f);
        x2 = static_cast<int32_t> ((X_[NN_] * 1.2f * (200.0f + V_[NN_]) / 200.0f) + 300.0f);
        y2 = static_cast<int32_t> ((Y_[NN_] * 1.2f * (200.0f + V_[NN_]) / 200.0f) + 300.0f);
        if (x1 < 0 || x1 >= olc::PixelGameEngine::ScreenWidth ()) goto continue_;
        if (y1 < 0 || y1 >= olc::PixelGameEngine::ScreenHeight ()) goto continue_;
        if (x2 < 0 || x2 >= olc::PixelGameEngine::ScreenWidth ()) goto continue_;
        if (y2 < 0 || y2 >= olc::PixelGameEngine::ScreenHeight ()) goto continue_;
        grayscale_i = static_cast<uint8_t> (125.0f + (Z_[N_] / 2.0f));
        olc::PixelGameEngine::DrawLine (x1, y1, x2, y2, {grayscale_i, grayscale_i, grayscale_i, 255}, 0xFFFFFFFF);
      } // end IF
continue_:
      if (Z_[N_] > Z_[NN_])
      {
        KX_ = X_[N_]; KY_ = Y_[N_]; KZ_ = Z_[N_]; KV_ = V_[N_]; KdV_ = dV_[N_];
        X_[N_] = X_[NN_]; Y_[N_] = Y_[NN_]; Z_[N_] = Z_[NN_]; V_[N_] = V_[NN_]; dV_[N_] = dV_[NN_];
        X_[NN_] = KX_; Y_[NN_] = KY_; Z_[NN_] = KZ_; V_[NN_] = KV_; dV_[NN_] = KdV_;
      } // end IF
    } // end FOR

    L_ = std::sqrt ((X_[N_] * X_[N_]) + (Y_[N_] * Y_[N_]));
    L_ = std::sqrt ((Z_[N_] * Z_[N_]) + (L_ * L_));
    X_[N_] = X_[N_] + (X_[N_] * (200.0f - L_) / (2.0f * L_));
    Y_[N_] = Y_[N_] + (Y_[N_] * (200.0f - L_) / (2.0f * L_));
    Z_[N_] = Z_[N_] + (Z_[N_] * (200.0f - L_) / (2.0f * L_));
    KZ_ = Z_[N_]; KX_ = X_[N_];
    Z_[N_] =
      (KZ_ * std::cos ((300.0f - olc::PixelGameEngine::GetMouseX ()) / 10000.0f)) - (KX_ * std::sin ((300.0f - olc::PixelGameEngine::GetMouseX ()) / 10000.0f));
    X_[N_] =
      (KZ_ * std::sin ((300.0f - olc::PixelGameEngine::GetMouseX ()) / 10000.0f)) + (KX_ * std::cos ((300.0f - olc::PixelGameEngine::GetMouseX ()) / 10000.0f));
    KZ_ = Z_[N_]; KY_ = Y_[N_];
    Z_[N_] =
      (KZ_ * std::cos ((300.0f - olc::PixelGameEngine::GetMouseY ()) / 10000.0f)) - (KY_ * std::sin ((300.0f - olc::PixelGameEngine::GetMouseY ()) / 10000.0f));
    Y_[N_] =
      (KZ_ * std::sin ((300.0f - olc::PixelGameEngine::GetMouseY ()) / 10000.0f)) + (KY_ * std::cos ((300.0f - olc::PixelGameEngine::GetMouseY ()) / 10000.0f));
    dV_[N_] = dV_[N_] - (V_[N_] * ENGINE_PGE_108_DEFAULT_HH);
    V_[N_] = V_[N_] + dV_[N_]; dV_[N_] = dV_[N_] * ENGINE_PGE_108_DEFAULT_H;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
