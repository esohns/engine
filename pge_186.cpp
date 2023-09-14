#include "stdafx.h"

#include "pge_186.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_186::PGE_186 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 186");
}
 
bool
PGE_186::OnUserCreate ()
{
  return true;
}

bool
PGE_186::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_186_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_186_DEFAULT_ALPHA_DECAY : 0);

  for (int32_t x = 0; x < ENGINE_PGE_186_DEFAULT_W; x++)
  {
    float n =
      //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (frame_count_i * 0.001,
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (frame_count_i * 0.001,
                                                                 (x - ENGINE_PGE_186_DEFAULT_W / 2) / static_cast<double> (ENGINE_PGE_186_DEFAULT_W),
                                                                 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 5.0f + 3.0f;
    //olc::PixelGameEngine::Draw (x, static_cast<int32_t> (x + frame_count_i * n) % ENGINE_PGE_186_DEFAULT_W,
    //                            olc::WHITE);
    olc::PixelGameEngine::FillCircle (x, static_cast<int32_t> (x + frame_count_i * n) % ENGINE_PGE_186_DEFAULT_W,
                                      static_cast<int32_t> (n / 3.0f),
                                      olc::WHITE);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
