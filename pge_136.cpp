#include "stdafx.h"

#include "pge_136.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_136::PGE_136 ()
 : olc::PixelGameEngine ()
 , dots_ ()
 , noise_ ()
{
  sAppName = "Example 136";
}

bool
PGE_136::OnUserCreate ()
{

  return true;
}

bool
PGE_136::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_136_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_136_DEFAULT_ALPHA_DECAY : 0);

  dots_.clear ();
  for (int i = 0; i < ENGINE_PGE_136_DEFAULT_NUMBER_OF_DOTS; i++)
  {
    double noiseIndex = (frame_count_i * 0.75 + i) / 100.0;
    float a =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseIndex, 0.0, 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 3.0f;
    float b =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseIndex, 1.0, 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 3;
    float theta =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_136_DEFAULT_NUMBER_OF_DOTS), 0.0f, 2.0f * static_cast<float> (M_PI));
    float t = frame_count_i / 240.0f;
    float x = std::cos (a * theta + t) * ENGINE_PGE_136_DEFAULT_RADIUS;
    float y = std::sin (b * theta + t) * ENGINE_PGE_136_DEFAULT_RADIUS;
    dots_.push_back ({x, y});
  } // end FOR

  for (int i = static_cast<int> (dots_.size ()); i--;)
    for (int j = i; j--;)
      if ((dots_[i].x - dots_[j].x) * (dots_[i].x - dots_[j].x) + (dots_[i].y - dots_[j].y) * (dots_[i].y - dots_[j].y) < ENGINE_PGE_136_DEFAULT_DISTANCE * ENGINE_PGE_136_DEFAULT_DISTANCE)
        olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (dots_[i].x), half_height_i + static_cast<int32_t> (dots_[i].y),
                                        half_width_i + static_cast<int32_t> (dots_[j].x), half_height_i + static_cast<int32_t> (dots_[j].y),
                                        olc::WHITE, 0xFFFFFFFF);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
