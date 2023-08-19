#include "stdafx.h"

#include "pge_127.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_127::PGE_127 ()
 : olc::PixelGameEngine ()
 , step_ (0)
 , theta_ (0.0f)
{
  sAppName = "Example 127";
}

bool
PGE_127::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  step_ =
    static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / static_cast<float> (ENGINE_PGE_127_DEFAULT_NUM));

  return true;
}

bool
PGE_127::OnUserUpdate (float fElapsedTime)
{
  static olc::Pixel color (0xFF, 0xB7, 0x00, 255);
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_127_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_127_DEFAULT_ALPHA_DECAY : 0);

  olc::PixelGameEngine::FillCircle (half_width_i, half_height_i, static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () * 0.3f), color);
  int i = 1;
  int32_t edge = 2 * step_;
  for (int32_t y = 2 * step_; y < olc::PixelGameEngine::ScreenHeight () - 2 * step_ + 1; y += step_)
  {
    float sw =
      Common_GL_Tools::map (std::sin (theta_ + (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_127_DEFAULT_NUM) * i)), -1.0f, 1.0f, 1.5f, 12.0f);
    //strokeWeight (sw);
    for (int j = 0; j < static_cast<int32_t> (sw / 1.5f); j++)
    {
      olc::PixelGameEngine::DrawLine (edge, y - j, olc::PixelGameEngine::ScreenWidth () - edge, y - j, olc::BLACK, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (edge, y + j, olc::PixelGameEngine::ScreenWidth () - edge, y + j, olc::BLACK, 0xFFFFFFFF);
    } // end FOR
    i++;
  } // end FOR
  theta_ += 0.0523f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
