#include "stdafx.h"

#include "pge_98.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_98::PGE_98 ()
 : olc::PixelGameEngine ()
 , xLoop_ (2.0f, 50.0f, 400.0f)
 , yLoop_ (2.0f, 50.0f, 400.0f)
 , noise_ ()
 , xMove_ (Common_Tools::getRandomNumber (0.0f, 1000.0f))
 , yMove_ (Common_Tools::getRandomNumber (0.0f, 1000.0f))
 , scaler_ (1.0f)
{
  sAppName = "Example 98";
}

bool
PGE_98::OnUserCreate ()
{
  return true;
}

bool
PGE_98::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  //if (scaler_ < 0.0f) goto end;

  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth() * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_98_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_98_DEFAULT_ALPHA_DECAY : 0);

  xMove_ += 0.001f;
  yMove_ += 0.001f;
  float xmover =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (xMove_, frame_count_i / 1000.0, 0.0)), -1.0f, 1.0f, -olc::PixelGameEngine::ScreenWidth () / 5.0f, olc::PixelGameEngine::ScreenWidth () / 5.0f);
  float ymover =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (yMove_, frame_count_i / 1000.0, 0.0)), -1.0f, 1.0f, -olc::PixelGameEngine::ScreenHeight () / 5.0f, olc::PixelGameEngine::ScreenHeight () / 5.0f);
  //scale(scaler)
  for (float i = 0; i <= 2.0f * static_cast<float> (M_PI); i += 2.0f * static_cast<float> (M_PI) / 100.0f)
  {
    int32_t x = static_cast<int32_t> (std::cos (i) * xLoop_.value (i, frame_count_i, &noise_) + half_width_i + xmover);
    int32_t y = static_cast<int32_t> (std::sin (i) * yLoop_.value (i, frame_count_i, &noise_) + half_height_i + ymover);
    static int32_t prev_x = x;
    static int32_t prev_y = y;
    olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y, {255, 255, 255, 255}, 0xFFFFFFFF);
    prev_x = x;
    prev_y = y;
  } // end FOR
  scaler_ -= 0.0004f;

//end:
  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
