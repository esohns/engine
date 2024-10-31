#include "stdafx.h"

#include "pge_380.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_380::PGE_380 ()
 : olc::PixelGameEngine ()
 , f_ (0.0f)
 , R_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 380");
}

bool
PGE_380::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_380::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_380_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_380_DEFAULT_ALPHA_DECAY : 0);

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static float P = static_cast<float> (M_PI) / 16.0f;
  static int N = 30;

  float n = 0.1f;

  for (float i = 0.0f; i < 2.0f * static_cast<float> (M_PI); i += P)
  {
    for (int r = -N; r < 155; r += N)
    {
      float F = f_ / 99.0f;

      float Z = i + n - F;
      int K = static_cast<int> (r + R_) % 220;
      int32_t x = static_cast<int32_t> (std::sin (Z) * K);
      int32_t y = static_cast<int32_t> (std::cos (Z) * K);

      float H = i - n - F;
      K = K + N;
      int32_t X = static_cast<int32_t> (std::sin (H) * K);
      int32_t Y = static_cast<int32_t> (std::cos (H) * K);

      olc::PixelGameEngine::DrawLine (half_width_i + x, half_height_i + y, half_width_i + X, half_height_i + Y, olc::WHITE, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawCircle (half_width_i + x / 2, half_height_i + y / 2, 2, olc::WHITE, 0xFF);

      n = -n;
    } // end FOR

    R_ += 0.02f;
  } // end FOR

  f_ += P;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
