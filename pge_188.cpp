#include "stdafx.h"

#include "pge_188.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_188::PGE_188 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , C_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 188");
}
 
bool
PGE_188::OnUserCreate ()
{
  C_ = -static_cast<float> (M_PI) / 2.0f;

  return true;
}

bool
PGE_188::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  static int32_t prev_x, prev_y, first_x, first_y;
  int32_t x, y = 0;
  bool is_first_b = true;
  float f = std::sin (C_) * (ENGINE_PGE_188_DEFAULT_W - 1) + ENGINE_PGE_188_DEFAULT_W, n;
  for (int32_t i = 0; i < ENGINE_PGE_188_DEFAULT_W; i++)
  {
    x = static_cast<int32_t> (f + i) % (ENGINE_PGE_188_DEFAULT_W / 6) * 6;
    olc::vf2d pos (static_cast<float> (x), static_cast<float> (y));
    n =
      //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / static_cast<float> (ENGINE_PGE_188_DEFAULT_W),
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (x / static_cast<float> (ENGINE_PGE_188_DEFAULT_W),
                                                                 y / static_cast<float> (ENGINE_PGE_188_DEFAULT_W),
                                                                 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * (pos.mag () + f) * 0.05f;
    x = static_cast<int32_t> ((x + std::cos (n) * ENGINE_PGE_188_DEFAULT_R) * 0.8f + 72.0f);
    y = static_cast<int32_t> ( i + std::sin (n) * ENGINE_PGE_188_DEFAULT_R);
    if (is_first_b)
    {
      is_first_b = false;
      first_x = x;
      first_y = y;
      prev_x = x;
      prev_y = y;
    } // end IF
    olc::PixelGameEngine::DrawLine (prev_x, prev_y, x, y,
                                    olc::BLACK, 0xFFFFFFFF);
    prev_x = x;
    prev_y = y;
  } // end FOR
  olc::PixelGameEngine::DrawLine (x, y, first_x, first_y,
                                  olc::BLACK, 0xFFFFFFFF);

  C_ += 0.0015f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
