#include "stdafx.h"

#include "pge_49.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_49::PGE_49 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , yOffset_ (0.0)
{
  sAppName = "Example 49";
}

bool
PGE_49::OnUserCreate ()
{
  return true;
}

bool
PGE_49::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  static olc::vf2d prev = {0.0f, 0.0f};

  double xOffset_d = 0.0;
  static float da = static_cast<float> (M_PI) / 200.0f;
  for (float a = 0.0f; a <= static_cast<float> (2.0 * M_PI); a += da)
  {
    float n = static_cast<float> (noise_.GetValue (xOffset_d, yOffset_, 0.0));
    float r = static_cast<float> (std::sin (2.0f * a)) * Common_GL_Tools::map (n, -1.0f, 1.0f, 50.0f, 300.0f);
    float x = r * cos (a);
    float y = r * sin (a);
    if (a < static_cast<float> (M_PI))
      xOffset_d += ENGINE_PGE_49_DEFAULT_DX;
    else
      xOffset_d -= ENGINE_PGE_49_DEFAULT_DX;

    //olc::PixelGameEngine::Draw (half_width_i + static_cast<int32_t> (x), half_height_i + static_cast<int32_t> (y), olc::MAGENTA);
    prev.x = (a > 0.0f) ? prev.x : x;
    prev.y = (a > 0.0f) ? prev.y : y;
    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev.x), half_height_i + static_cast<int32_t> (prev.y),
                                    half_width_i + static_cast<int32_t> (x), half_height_i + static_cast<int32_t> (y), olc::MAGENTA, 0xFFFFFFFF);
    prev.x = x;
    prev.y = y;
  } // end FOR

  yOffset_ += ENGINE_PGE_49_DEFAULT_DY;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
