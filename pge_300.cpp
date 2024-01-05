#include "stdafx.h"

#include "pge_300.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_300::PGE_300 ()
 : olc::PixelGameEngine ()
 , X_ (0)
 , Y_ (0)
 , P_ (0.0f)
 , color_ (false)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 300");
}

bool
PGE_300::OnUserCreate ()
{
  X_ = -olc::PixelGameEngine::ScreenWidth () / 2;
  Y_ = -olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  return true;
}

bool
PGE_300::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  if (olc::PixelGameEngine::GetKey (olc::SPACE).bPressed)
    color_ = !color_;

  while (Y_ < half_height_i)
  {
    float KX = static_cast<float> (X_);
    float KY = static_cast<float> (Y_);
    int T = 0;
    int TT = ENGINE_PGE_300_DEFAULT_TTMIN;
    while (TT <= ENGINE_PGE_300_DEFAULT_TTMAX)
    {
      float dX = ENGINE_PGE_300_DEFAULT_H * std::sin (KY * ENGINE_PGE_300_DEFAULT_F);
      float dY = ENGINE_PGE_300_DEFAULT_H * std::sin (KX * ENGINE_PGE_300_DEFAULT_F + P_);
      KX = KX + dX;
      KY = KY + dY;
      ++T;
      if (T > TT)
      {
        TT = TT * 2;
        T = 0;
        KX = KX * ENGINE_PGE_300_DEFAULT_B;
        KY = KY * ENGINE_PGE_300_DEFAULT_B;
      } // end IF
    } // TT
    olc::Pixel color;
    if (color_)
    {
      uint8_t red_i   = static_cast<uint8_t> (std::sin ((KY - KX) / 500.0f) * 125.0f + 125.0f);
      uint8_t green_i = static_cast<uint8_t> (std::cos ((KY - KX) / 500.0f) * 125.0f + 125.0f);
      uint8_t blue_i  = static_cast<uint8_t> (std::tan ((KY - KX) / 500.0f) * 125.0f + 125.0f);
      color.r = red_i;
      color.g = green_i;
      color.b = blue_i;
    } // end IF
    else
    {
      uint8_t gray_i = static_cast<uint8_t> (std::sin ((KY - KX) / 500.0f) * 125.0f + 125.0f);
      color.r = color.g = color.b = gray_i;
    } // end ELSE
    olc::PixelGameEngine::Draw (olc::PixelGameEngine::ScreenWidth () - 1, Y_ + half_height_i, color);
    ++Y_;
  } // Y

  // shift all pixels to the left by one
  olc::Pixel* data_p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); ++y)
    for (int x = 0; x < olc::PixelGameEngine::ScreenWidth () - 1; ++x)
      data_p[y * olc::PixelGameEngine::ScreenWidth () + x] = data_p[y * olc::PixelGameEngine::ScreenWidth () + (x + 1)];

  ++X_;
  Y_ = -half_height_i;
  P_ += ENGINE_PGE_300_DEFAULT_DP;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
