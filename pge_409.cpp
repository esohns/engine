#include "stdafx.h"

#include "pge_409.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_18.h"

PGE_409::PGE_409 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 409";
}

bool
PGE_409::OnUserCreate ()
{
  static float width_f = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  static float height_f = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
  static float half_width_f = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  static float half_height_f = olc::PixelGameEngine::ScreenHeight () / 2.0f;

  CX[0] = 0.0f;    CY[0] = 0.0f;     R[0] = half_width_f;  D[0] = 0.0f;
  CX[1] = width_f; CY[1] = 0.0f;     R[1] = half_height_f; D[1] = static_cast<float> (M_PI) / 2.0f;
  CX[2] = 0.0f;    CY[2] = height_f; R[2] = half_width_f;  D[2] = static_cast<float> (M_PI);
  CX[3] = width_f; CY[3] = height_f; R[3] = half_height_f; D[3] = 3.0f * static_cast<float> (M_PI) / 2.0f;

  return true;
}

bool
PGE_409::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    olc::PixelGameEngine::Clear (olc::BLACK);

  static float width_f = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  static float height_f = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
  static float half_width_f = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  static float half_height_f = olc::PixelGameEngine::ScreenHeight () / 2.0f;

  for (int P = 0; P < ENGINE_PGE_409_DEFAULT_P_MAX; P++)
  {
    X = Common_Tools::getRandomNumber (-9999999.0f, 9999999.0f);
    Y = Common_Tools::getRandomNumber (-9999999.0f, 9999999.0f);
    for (int Q = 0; Q < ENGINE_PGE_409_DEFAULT_Q_MAX; Q++)
    {
      I = Common_Tools::getRandomNumber (0, 3); II = Common_Tools::getRandomNumber (0, 3);
      Zx[0] = X; Zy[0] = Y;
      setting (); hen (); kan ();
      X = Wx[0]; Y = Wy[0];
    } // end FOR

    if (X > half_width_f)
      X = X - width_f;
    if (Y > half_height_f)
      Y = Y - height_f;

    olc::PixelGameEngine::Draw (static_cast<int32_t> (X + half_width_f), static_cast<int32_t> (Y + half_height_f), olc::WHITE);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_409::setting ()
{
  Zx[1] = R[I]  * std::cos (D[I])                                                                            + CX[I];
  Zy[1] = R[I] *  std::sin (D[I])                                                                            + CY[I];
  Zx[2] = R[I]  * std::cos (D[I]  + (static_cast<float> (M_PI) / 3.0f))                                      + CX[I];
  Zy[2] = R[I] *  std::sin (D[I]  + (static_cast<float> (M_PI) / 3.0f))                                      + CY[I];
  Zx[3] = R[I]  * std::cos (D[I]  - (static_cast<float> (M_PI) / 4.0f))                                      + CX[I];
  Zy[3] = R[I] *  std::sin (D[I]  - (static_cast<float> (M_PI) / 4.0f))                                      + CY[I];

  Wx[1] = R[II] * std::cos (D[II] + (static_cast<float> (M_PI) / 2.0f))                                      + CX[II];
  Wy[1] = R[II] * std::sin (D[II] + (static_cast<float> (M_PI) / 2.0f))                                      + CY[II];
  Wx[2] = R[II] * std::cos (D[II] + (static_cast<float> (M_PI) / 2.0f) - (static_cast<float> (M_PI) / 3.0f)) + CX[II];
  Wy[2] = R[II] * std::sin (D[II] + (static_cast<float> (M_PI) / 2.0f) - (static_cast<float> (M_PI) / 3.0f)) + CY[II];
  Wx[3] = R[II] * std::cos (D[II] + (static_cast<float> (M_PI) / 2.0f) + (static_cast<float> (M_PI) / 4.0f)) + CX[II];
  Wy[3] = R[II] * std::sin (D[II] + (static_cast<float> (M_PI) / 2.0f) + (static_cast<float> (M_PI) / 4.0f)) + CY[II];
}

void
PGE_409::hen ()
{
  A.x = Wx[3] - Wx[2]; A.y = Wy[3] - Wy[2];
  B.x = Zx[3] - Zx[1]; B.y = Zy[3] - Zy[1];
  D_2.x = Wx[3] - Wx[1]; D_2.y = Wy[3] - Wy[1];
  E.x = Zx[3] - Zx[2]; E.y = Zy[3] - Zy[2];
}

void
PGE_409::kan ()
{
  C.x = Zx[0] - Zx[2]; C.y = Zy[0] - Zy[2];
  F.x = Zx[0] - Zx[1]; F.y = Zy[0] - Zy[1];
  K.x = (A.x * B.x) - (A.y * B.y); K.y = (A.x * B.y) + (A.y * B.x); LD.x = K.x; LD.y = K.y;
  K.x = (LD.x * C.x) - (LD.y * C.y); K.y = (LD.x * C.y) + (LD.y * C.x); LD.x = K.x; LD.y = K.y;
  K.x = (D_2.x * E.x) - (D_2.y * E.y); K.y = (D_2.x * E.y) + (D_2.y * E.x); RD.x = K.x; RD.y = K.y;
  K.x = (RD.x * F.x) - (RD.y * F.y); K.y = (RD.x * F.y) + (RD.y * F.x); RD.x = K.x; RD.y = K.y;
  K.x = (Wx[1] * LD.x) - (Wy[1] * LD.y); K.y = (Wx[1] * LD.y) + (Wy[1] * LD.x); LU.x = K.x; LU.y = K.y;
  K.x = (Wx[2] * RD.x) - (Wy[2] * RD.y); K.y = (Wx[2] * RD.y) + (Wy[2] * RD.x); RU.x = K.x; RU.y = K.y;
  NAKA.x = LU.x - RU.x; NAKA.y = LU.y - RU.y; HOPPY.x = LD.x - RD.x; HOPPY.y = LD.y - RD.y;
  Wx[0] = (NAKA.x * HOPPY.x) + (NAKA.y * HOPPY.y); Wy[0] = -(NAKA.x * HOPPY.y) + (NAKA.y * HOPPY.x);
  Wx[0] = Wx[0] / ((HOPPY.x * HOPPY.x) + (HOPPY.y * HOPPY.y));
  Wy[0] = Wy[0] / ((HOPPY.x * HOPPY.x) + (HOPPY.y * HOPPY.y));
}
