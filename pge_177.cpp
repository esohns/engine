#include "stdafx.h"

#include "pge_177.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_177::PGE_177 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 177");
}

bool
PGE_177::OnUserCreate ()
{
  return true;
}

bool
PGE_177::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_177_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_177_DEFAULT_ALPHA_DECAY : 0);

  float s = std::sin (frame_count_i / 299.0f);
  float n = Common_GL_Tools::map (s, -1.0f, 1.0f, 12.0f, 88.0f);
  for (int32_t y = 0; y < olc::PixelGameEngine::ScreenWidth () / 7; y++)
    for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth () / 2; x++)
      if ((y + x - frame_count_i & y - x + frame_count_i) % 47 <= 3)
      {
        olc::vf2d p (static_cast<float> (x), static_cast<float> (y));
        float r =
          Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) + s,
                                                                     y / static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) * s,
                                                                     p.mag () / static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) + frame_count_i / static_cast<float> (olc::PixelGameEngine::ScreenWidth ()))),
                                -1.0f, 1.0f, 0.0f, 1.0f);
        olc::PixelGameEngine::Draw (static_cast<int32_t> (x * 2.0f + std::cos (r * n) * 30.0f),
                                    static_cast<int32_t> (y * 3.0f + sin (r) * 20.0f + static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) / 8.0f),
                                    olc::WHITE);
        //olc::PixelGameEngine::FillRect (static_cast<int32_t> (x * 2.0f + std::cos (r * n) * 30.0f),
        //                                static_cast<int32_t> (y * 3.0f + sin (r) * 20.0f + static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) / 8.0f),
        //                                2, 2,
        //                                olc::WHITE);
      } // end IF

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
