#include "stdafx.h"

#include "pge_411.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_19.h"

PGE_411::PGE_411 ()
 : olc::PixelGameEngine ()
 , f_ (0.0f)
{
  sAppName = "Example 411";
}

bool
PGE_411::OnUserCreate ()
{
  return true;
}

bool
PGE_411::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_411_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_411_DEFAULT_ALPHA_DECAY : 0);

  static int frame_count_i = 1;

  float huechange = frame_count_i * ENGINE_PGE_411_DEFAULT_HUE_SPEED;
  float r, g, b;
  for (float u = 0.0f; u < 2.0f * static_cast<float> (M_PI); u += static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_411_DEFAULT_COMPLEXITY))
  {
    float U = u + f_;
    for (float v = 0.0f; v < static_cast<float> (M_PI); v += static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_411_DEFAULT_COMPLEXITY))
    {
      float R =
        std::sin (v * std::abs (std::atan (std::tan (f_ / static_cast<float> (ENGINE_PGE_411_DEFAULT_FREQUENCY)))) * ENGINE_PGE_411_DEFAULT_VELOCITY + f_) * ENGINE_PGE_411_DEFAULT_DISPLACEMENT + ENGINE_PGE_411_DEFAULT_R;
      float Z = std::cos (U) * std::sin (v) * R;

      static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
      static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

      Common_Image_Tools::HSVToRGB (((static_cast<int> (std::abs (huechange + Z)) % 256) / 255.0f) * 360.0f,
                                    200.0f / 255.0f,
                                    255.0f / 255.0f,
                                    r, g, b);

      olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (std::sin (U) * std::sin (v) * ENGINE_PGE_411_DEFAULT_R),
                                        half_height_i + static_cast<int32_t> (std::cos (v) * ENGINE_PGE_411_DEFAULT_R),
                                        2,
                                        olc::PixelF (r, g, b, 1.0f),
                                        0xFF);
      //if (Z > reveal) point (sin (U) * sin (v) * r, cos (v) * r, Z); // *TODO*: use OpenGL for true 3D
    } // end FOR
  } // end FOR

  f_ += 0.2f;

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
