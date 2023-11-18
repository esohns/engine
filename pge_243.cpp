#include "stdafx.h"

#include "pge_243.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_243::PGE_243 ()
 : olc::PixelGameEngine ()
 , c_ (1)
 , f_ (0)
 , p_(0.0f)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 243");
}

bool
PGE_243::OnUserCreate ()
{
  return true;
}

bool
PGE_243::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int32_t y = 0; y < ENGINE_PGE_243_DEFAULT_HEIGHT * 2; y += 25)
  {
    for (int32_t x = 0; x < ENGINE_PGE_243_DEFAULT_WIDTH * 2; x++)
    {
      float n =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((x + f_ * 1.5) / 666.0, (y + f_) / static_cast<double> (ENGINE_PGE_243_DEFAULT_HEIGHT), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
        //Common_GL_Tools::map (static_cast<float> (noise_.Evaluate ((x + f_ * 1.5) / 666.0, (y + f_) / static_cast<double> (ENGINE_PGE_243_DEFAULT_HEIGHT), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      float n_2 = Common_GL_Tools::map (n, 0.0f, 1.0f, 0.0f, static_cast<float> (ENGINE_PGE_243_DEFAULT_WIDTH));
      float n_3 = (y - n_2 / 1.6f) - ((n_2 * 0.5f) * c_);
      uint8_t value_i =
        c_ > 0 ? static_cast<uint8_t> (std::clamp (n_3, 0.0f, 255.0f)) : 0;
      olc::Pixel color (value_i, value_i, value_i, 255);
      olc::PixelGameEngine::DrawLine (x, ENGINE_PGE_243_DEFAULT_HEIGHT,
                                      x, static_cast<int32_t> (y + n * (std::sin (static_cast<float> (M_PI) / 800.0f * static_cast<float> (x)) * ENGINE_PGE_243_DEFAULT_HEIGHT) - ENGINE_PGE_243_DEFAULT_HEIGHT),
                                      color, 0xFFFFFFFF);
      p_ = n;
    } // end FOR
    c_ = -c_;
  } // end FOR

  f_ -= 4;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
