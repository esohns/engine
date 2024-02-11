#include "stdafx.h"

#include "pge_354.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_16.h"

PGE_354::PGE_354 ()
 : olc::PixelGameEngine ()
 , X (ENGINE_PGE_354_DEFAULT_N_MAX, 0.0f) , Y (ENGINE_PGE_354_DEFAULT_N_MAX, 0.0f)
 , FX (ENGINE_PGE_354_DEFAULT_N_MAX, 0.0f), FY (ENGINE_PGE_354_DEFAULT_N_MAX, 0.0f)
 , VX (ENGINE_PGE_354_DEFAULT_N_MAX, 0.0f), VY (ENGINE_PGE_354_DEFAULT_N_MAX, 0.0f)
 , Q (ENGINE_PGE_354_DEFAULT_N_MAX, 0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 354");
}

bool
PGE_354::OnUserCreate ()
{
  X[0] = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  Y[0] = olc::PixelGameEngine::ScreenHeight () / 2.0f;
  Q[0] = 1;
  N = 0; VX[0] = 0.0f; VY[0] = 0.0f;

  return true;
}

bool
PGE_354::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    ++N;
    if (N >= ENGINE_PGE_354_DEFAULT_N_MAX) { OnUserCreate (); }
    else
    {
      X[N] = static_cast<float> (olc::PixelGameEngine::GetMouseX ());
      Y[N] = static_cast<float> (olc::PixelGameEngine::GetMouseY ());
      Q[N] = -Q[N - 1];
      VX[N] = 0.0f;
      VY[N] = 0.0f;
      OKNG = 1;
      for (I = 0; I < N; I++)
      {
        L = std::sqrt (((X[N] - X[I]) * (X[N] - X[I])) + ((Y[N] - Y[I]) * (Y[N] - Y[I])));
        if (L < static_cast<float> (ENGINE_PGE_354_DEFAULT_R))
          OKNG = 0;
      } // end FOR
      if (OKNG == 0)
        --N;
    } // end ELSE
  } // end IF
  if (olc::PixelGameEngine::GetMouse (1).bPressed)
    OnUserCreate ();

  olc::Pixel color;
  float temp_f;
  static olc::vf2d factor_s (olc::PixelGameEngine::ScreenWidth() / static_cast<float> (ENGINE_PGE_354_DEFAULT_TY),
                             olc::PixelGameEngine::ScreenHeight () / static_cast<float> (ENGINE_PGE_354_DEFAULT_TY));
  for (I = 0; I < ENGINE_PGE_354_DEFAULT_TY; I++)
    for (II = 0; II < ENGINE_PGE_354_DEFAULT_TY; II++)
    {
      KX = I  * factor_s.x;
      KY = II * factor_s.y;

      C = 0.0f;
      for (III = 0; III <= N; III++)
      {
        L =
          std::sqrt (((X[III] - KX) * (X[III] - KX)) + ((Y[III] - KY) * (Y[III] - KY)));
        C = C + (500.0f * Q[III] / std::sqrt (L));
      } // end FOR
      temp_f = std::cos (C);
      color =
        {static_cast<uint8_t> (127 - (127 * temp_f)), static_cast<uint8_t> (127 - (127 * temp_f)), static_cast<uint8_t> (127 - (127 * temp_f)), 255};

      olc::PixelGameEngine::FillRect (static_cast<int32_t> (KX), static_cast<int32_t> (KY),
                                      static_cast<int32_t> (factor_s.x), static_cast<int32_t> (factor_s.y),
                                      color);
    } // end FOR

  for (I = 0; I <= N; I++)
  {
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (X[I]), static_cast<int32_t> (Y[I]),
                                      ENGINE_PGE_354_DEFAULT_R, olc::WHITE);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (X[I] - (ENGINE_PGE_354_DEFAULT_R * 0.5f)), static_cast<int32_t> (Y[I]),
                                    static_cast<int32_t> (X[I] + (ENGINE_PGE_354_DEFAULT_R * 0.5f)), static_cast<int32_t> (Y[I]),
                                    olc::BLACK, 0xFFFFFFFF);
    if (Q[I] > 0)
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (X[I]), static_cast<int32_t> (Y[I] - (ENGINE_PGE_354_DEFAULT_R * 0.5f)),
                                      static_cast<int32_t> (X[I]), static_cast<int32_t> (Y[I] + (ENGINE_PGE_354_DEFAULT_R * 0.5f)),
                                      olc::BLACK, 0xFFFFFFFF);

    for (II = I + 1; II <= N; II++)
    {
      L = std::sqrt (((X[I] - X[II]) * (X[I] - X[II])) + ((Y[I] - Y[II]) * (Y[I] - Y[II])));
      FX[I] = FX[I] + (Q[I] * Q[II] * ENGINE_PGE_354_DEFAULT_FPE * (X[I] - X[II]) / (L * L * L));
      FY[I] = FY[I] + (Q[I] * Q[II] * ENGINE_PGE_354_DEFAULT_FPE * (Y[I] - Y[II]) / (L * L * L));
      FX[II] = FX[II] - (Q[I] * Q[II] * ENGINE_PGE_354_DEFAULT_FPE * (X[I] - X[II]) / (L * L * L));
      FY[II] = FY[II] - (Q[I] * Q[II] * ENGINE_PGE_354_DEFAULT_FPE * (Y[I] - Y[II]) / (L * L * L));
      if (L < static_cast<float> (2 * ENGINE_PGE_354_DEFAULT_R))
      {
        FX[I] = FX[I] - (((X[II] - X[I]) / L) * (ENGINE_PGE_354_DEFAULT_K * (2.0f * ENGINE_PGE_354_DEFAULT_R - L)));
        FY[I] = FY[I] - (((Y[II] - Y[I]) / L) * (ENGINE_PGE_354_DEFAULT_K * (2.0f * ENGINE_PGE_354_DEFAULT_R - L)));
        FX[II] = FX[II] + (((X[II] - X[I]) / L) * (ENGINE_PGE_354_DEFAULT_K * (2.0f * ENGINE_PGE_354_DEFAULT_R - L)));
        FY[II] = FY[II] + (((Y[II] - Y[I]) / L) * (ENGINE_PGE_354_DEFAULT_K * (2.0f * ENGINE_PGE_354_DEFAULT_R - L)));
      } // end IF
    } // II
    VX[I] = (VX[I] + (FX[I] / ENGINE_PGE_354_DEFAULT_M)) * ENGINE_PGE_354_DEFAULT_H;
    VY[I] = (VY[I] + (FY[I] / ENGINE_PGE_354_DEFAULT_M)) * ENGINE_PGE_354_DEFAULT_H;
    L = std::sqrt ((VX[I] * VX[I]) + (VY[I] * VY[I]));
    if (L > static_cast<float> (ENGINE_PGE_354_DEFAULT_V_MAX))
    {
      VX[I] = VX[I] * (static_cast<float> (ENGINE_PGE_354_DEFAULT_V_MAX) / L);
      VY[I] = VY[I] * (static_cast<float> (ENGINE_PGE_354_DEFAULT_V_MAX) / L);
    } // end IF

    if (L > 0.0f)
    {
      X[I] = X[I] + VX[I];
      Y[I] = Y[I] + VY[I];

      if (X[I] < static_cast<float> (ENGINE_PGE_354_DEFAULT_R))
        X[I] = static_cast<float> (ENGINE_PGE_354_DEFAULT_R);
      if (X[I] > static_cast<float> (olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_354_DEFAULT_R))
        X[I] = static_cast<float> (olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_354_DEFAULT_R);
      if (Y[I] < static_cast<float> (ENGINE_PGE_354_DEFAULT_R))
        Y[I] = static_cast<float> (ENGINE_PGE_354_DEFAULT_R);
      if (Y[I] > static_cast<float> (olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_354_DEFAULT_R))
        Y[I] = static_cast<float> (olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_354_DEFAULT_R);
    } // end IF

    FX[I] = 0.0f;
    FY[I] = 0.0f;
  } // I

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
