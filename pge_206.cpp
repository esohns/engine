#include "stdafx.h"

#include "pge_206.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_206::PGE_206 ()
 : olc::PixelGameEngine ()
 , CX_ ()
 , CY_ ()
 , LR_ ()
 , BN_ ()
 , Rmax_ ()
 , Aone_ ()
 , Azero_ ()
 , R_ (0.0f)
 , I_ (0)
 , II_ (0)
 , L_ (0.0f)
 , OKNG_ (0)
 , NX_ (0.0f)
 , NY_ (0.0f)
 , NRmax_ (0.0f)
 , RND_ (0.0f)
 , NN_ (0)
 , KY_ (0.0f)
 , KX_ (0.0f)
 , KA_ (0.0f)
 , DX_ (0.0f)
 , DY_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 206");
}

bool
PGE_206::OnUserCreate ()
{
  CX_[0] = 40.0f;
  CY_[0] = 0.0f;
  Rmax_[0] = 40.0f;
  LR_[0] = 1;
  Azero_[0] = static_cast<float> (-M_PI);
  Aone_[0] = Azero_[0];
  
  CX_[1] = -40.0f;
  CY_[1] = 0.0f;
  Rmax_[1] = 40.0f;
  LR_[1] = 1;
  Azero_[1] = 0.0f;
  Aone_[1] = Azero_[1];

  NN_ = 2;

  return true;
}

bool
PGE_206::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    OnUserCreate ();

  olc::PixelGameEngine::Clear (olc::BLACK);

  static olc::vf2d center_s (static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2),
                             static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2));
  olc::PixelGameEngine::FillCircle (center_s, 300, {0, 100, 0, 255});

  for (I_ = 0; I_ < NN_; I_++)
  {
    static olc::vf2d prev_s (0.0f, 0.0f);
    bool is_first_b = true;

    if (LR_[I_] > 0)
    {
      for (KA_ = Azero_[I_]; KA_ < Aone_[I_]; KA_ = KA_ + ENGINE_PGE_206_DEFAULT_DA)
      { // draUzu ()
        R_ = Rmax_[I_] - ((KA_ - Azero_[I_]) * LR_[I_] * 3.0f);
        KX_ = CX_[I_] + (R_ * std::cos (KA_)) + DX_;
        KY_ = CY_[I_] + (R_ * std::sin (KA_)) + DY_;
        L_ = std::sqrt (static_cast<float> (std::pow (KX_, 2) + std::pow (KY_, 2)));
        KX_ = center_s.x * std::sin (L_ / center_s.x) * KX_ / L_;
        KY_ = center_s.y * std::sin (L_ / center_s.y) * KY_ / L_;
        if (L_ / 300.0f > -static_cast<float> (M_PI_2) && L_ / 300.0f < static_cast<float> (M_PI_2))
        {
          olc::vf2d point_s (KX_ + center_s.x, KY_ + center_s.y);
          if (is_first_b)
          {
            is_first_b = false;
            prev_s = point_s;
          } // end IF
          olc::PixelGameEngine::DrawLine (prev_s, point_s, olc::WHITE, 0xFFFFFFFF);
          prev_s = point_s;
        } // end IF
      } // end FOR
    } // end IF

    is_first_b = true;
    if (LR_[I_] < 0)
    {
      for (KA_ = Azero_[I_]; KA_ > Aone_[I_]; KA_ = KA_ - ENGINE_PGE_206_DEFAULT_DA)
      { // draUzu ()
        R_ = Rmax_[I_] - ((KA_ - Azero_[I_]) * LR_[I_] * 3.0f);
        KX_ = CX_[I_] + (R_ * std::cos (KA_)) + DX_;
        KY_ = CY_[I_] + (R_ * std::sin (KA_)) + DY_;
        L_ = std::sqrt (static_cast<float> (std::pow (KX_, 2) + std::pow (KY_, 2)));
        KX_ = center_s.x * std::sin (L_ / center_s.x) * KX_ / L_;
        KY_ = center_s.y * std::sin (L_ / center_s.y) * KY_ / L_;
        if (L_ / 300.0f > -static_cast<float> (M_PI_2) && L_ / 300.0f < static_cast<float> (M_PI_2))
        {
          olc::vf2d point_s (KX_ + center_s.x, KY_ + center_s.y);
          if (is_first_b)
          {
            is_first_b = false;
            prev_s = point_s;
          } // end IF
          olc::PixelGameEngine::DrawLine (prev_s, point_s, olc::WHITE, 0xFFFFFFFF);
          prev_s = point_s;
        } // end IF
      } // end FOR
    } // end IF

    R_ = Rmax_[I_] - ((Aone_[I_] - Azero_[I_]) * LR_[I_] * 3);
    if (R_ > 5.0f) Aone_[I_] = Aone_[I_] + (ENGINE_PGE_206_DEFAULT_DA * LR_[I_]);
    if (LR_[I_] > 0 && std::abs (Azero_[I_] - Aone_[I_]) < (3.0f * static_cast<float> (M_PI_2))) RND_ = Common_Tools::getRandomNumber (Azero_[I_], Aone_[I_]);
    if (LR_[I_] < 0 && std::abs (Azero_[I_] - Aone_[I_]) < (3.0f * static_cast<float> (M_PI_2))) RND_ = Common_Tools::getRandomNumber (Aone_[I_], Azero_[I_]);
    if (LR_[I_] > 0 && std::abs (Azero_[I_] - Aone_[I_]) > (3.0f * static_cast<float> (M_PI_2))) RND_ = Common_Tools::getRandomNumber (Azero_[I_], Azero_[I_] + (3.0f * static_cast<float> (M_PI_2)));
    if (LR_[I_] < 0 && std::abs (Azero_[I_] - Aone_[I_]) > (3.0f * static_cast<float> (M_PI_2))) RND_ = Common_Tools::getRandomNumber (Azero_[I_] - (3.0f * static_cast<float> (M_PI_2)), Azero_[I_]);
    NRmax_ = Common_Tools::getRandomNumber (20.0f, 50.0f);
    NX_ = CX_[I_] + ((Rmax_[I_] - ((RND_ - Azero_[I_]) * LR_[I_] * 3.0f) + NRmax_) * std::cos (RND_));
    NY_ = CY_[I_] + ((Rmax_[I_] - ((RND_ - Azero_[I_]) * LR_[I_] * 3.0f) + NRmax_) * std::sin (RND_));
    OKNG_ = 1;
    for (II_ = 0; II_ < NN_; II_++)
    {
      L_ = std::sqrt (static_cast<float> (std::pow (NX_ - CX_[II_], 2) + std::pow (NY_ - CY_[II_], 2)));
      if (II_ != I_ && L_ < NRmax_ + Rmax_[II_]) OKNG_ = 0;
    } // II
    if (OKNG_ == 1 && NN_ < ENGINE_PGE_206_DEFAULT_NMAX - 1)
    {
      CX_[NN_] = NX_;
      CY_[NN_] = NY_;
      Rmax_[NN_] = NRmax_;
      Azero_[NN_] = RND_ + static_cast<float> (M_PI);
      Aone_[NN_] = Azero_[NN_];
      LR_[NN_] = -LR_[I_]; BN_[NN_] = I_;
      NN_ = NN_ + 1;
    } // OK
  } // 1<=I<NN

  DX_ = DX_ + (static_cast<float> (center_s.x - olc::PixelGameEngine::GetMouseX ()) / 30.0f);
  DY_ = DY_ + (static_cast<float> (center_s.y - olc::PixelGameEngine::GetMouseY ()) / 30.0f);
  if (std::abs (DX_) > 800.0f) DX_ = DX_ * 800.0f / std::abs (DX_);
  if (std::abs (DY_) > 800.0f) DY_ = DY_ * 800.0f / std::abs (DY_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
