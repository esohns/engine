#include "stdafx.h"

#include "pge_187.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_187::PGE_187 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , t_ (0.0f)
 , h_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 187");
}
 
bool
PGE_187::OnUserCreate ()
{
  h_ = ENGINE_PGE_187_DEFAULT_W / 2;

  return true;
}

bool
PGE_187::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  static int32_t prev_x, prev_y, first_x, first_y;
  int32_t x, y;
  bool is_first_b = true;
  float k, n;
  for (float i = 0.0f; i < 2.0f * static_cast<float> (M_PI) * static_cast<float> (ENGINE_PGE_187_DEFAULT_R); i += 2.0f * static_cast<float> (M_PI) / 24.0f)
  {
    k =
      Common_GL_Tools::map (i, 0.0f, 2.0f * static_cast<float> (M_PI) * static_cast<float> (ENGINE_PGE_187_DEFAULT_R), 30.0f / static_cast<float> (h_), 1.0f);
    n =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (std::cos (i * 3.0f),
      //Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (std::cos (i * 3.0f),
                                                                 std::sin (i * 3.0f) * k + t_,
                                                                 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 0.7f;
    x = static_cast<int32_t> (std::cos (i) * ENGINE_PGE_187_DEFAULT_W * n * k + h_);
    y = static_cast<int32_t> (std::sin (i) * ENGINE_PGE_187_DEFAULT_W * n * k + h_);
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
  //olc::PixelGameEngine::DrawLine (x, y, first_x, first_y,
  //                                olc::BLACK, 0xFFFFFFFF);

  t_ += 0.02f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
