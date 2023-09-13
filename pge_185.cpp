#include "stdafx.h"

#include "pge_185.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_185::PGE_185 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 185");
}
 
bool
PGE_185::OnUserCreate ()
{
  //olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_185::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  float f = Common_GL_Tools::map (std::sin (t_), -1.0f, 1.0f, 64.0f, 200.0f);
  for (int32_t y = 2; y < ENGINE_PGE_185_DEFAULT_W; y += 4)
    for (int32_t x = 2; x < ENGINE_PGE_185_DEFAULT_W; x += 4)
    {
      float r =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / static_cast<double> (ENGINE_PGE_185_DEFAULT_W),
        //Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (x / static_cast<double> (ENGINE_PGE_185_DEFAULT_W),
                                                                   y / static_cast<double> (ENGINE_PGE_185_DEFAULT_W) - t_,
                                                                   0.0)),
                              -1.0f, 1.0f, 0.0f, 1.0f);
      //olc::PixelGameEngine::FillCircle (static_cast<int32_t> (std::cos (r * f) * 2.0f * static_cast<float> (M_PI)) + x,
      //                                  static_cast<int32_t> (std::sin (r) *     2.0f * static_cast<float> (M_PI)) + y,
      //                                  2,
      //                                  olc::BLACK);
      olc::PixelGameEngine::Draw (static_cast<int32_t> (std::cos (r * f) * 2.0f * static_cast<float> (M_PI)) + x,
                                  static_cast<int32_t> (std::sin (r) *     2.0f * static_cast<float> (M_PI)) + y,
                                  olc::BLACK);
    } // end FOR

  t_ += 0.01f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
