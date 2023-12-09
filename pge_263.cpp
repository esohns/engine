#include "stdafx.h"

#include "pge_263.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_263::PGE_263 ()
 : olc::PixelGameEngine ()
 , f_ (0)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 263");
}

bool
PGE_263::OnUserCreate ()
{
  return true;
}

bool
PGE_263::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int y = 100; y < 300; y += 20)
    for (int x = -200; x < 600; x++)
    {
      int F = f_ % 700 - 200;
      if (x > F && x < (F + 200))
      {
        float Y = 0.0f;
        if (x > 200)
          Y =
            //(Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / 400.0, y + y / 66.0, f_ / 400.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * (200.0f - x);
            (Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (x / 400.0, y + y / 66.0, f_ / 400.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * (200.0f - x);
        float gray_f = std::abs (270.0f - Y * 22.0f);
        uint8_t gray =
          static_cast<uint8_t> (gray_f > 255.0f ? 255.0f : gray_f < 0.0f ? 0.0f : gray_f);
        olc::Pixel color (gray, gray, gray, 255);
        olc::PixelGameEngine::DrawLine (static_cast<int32_t> (y + Y), static_cast<int32_t> (x + Y * 2.0f),
                                        static_cast<int32_t> (y + Y + 20.0f), static_cast<int32_t> (x + Y * 2.0f),
                                        color, 0xFFFFFFFF);
      } // end IF
    } // end FOR

  f_ += 2;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
