#include "stdafx.h"

#include "pge_83.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_83::PGE_83 ()
 : olc::PixelGameEngine ()
 , count_ (ENGINE_PGE_83_DEFAULT_COUNT)
 , size_ (ENGINE_PGE_83_DEFAULT_SIZE)
{
  sAppName = "Example 83";
}

bool
PGE_83::OnUserCreate ()
{
  return true;
}

bool
PGE_83::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_83_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_83_DEFAULT_ALPHA_DECAY : 0);

  count_ =
    Common_GL_Tools::map (olc::PixelGameEngine::GetMouseX (), 0, olc::PixelGameEngine::ScreenWidth () - 1, 10, ENGINE_PGE_83_DEFAULT_COUNT);
  size_ =
    Common_GL_Tools::map (olc::PixelGameEngine::GetMouseY (), 0, olc::PixelGameEngine::ScreenHeight () - 1, 1, 100);

  static int frame_count_i = 1;

  olc::Pixel color_s;
  float r, g, b, posX, posY;
  for (int32_t i = count_; i > 0; i--)
  {
    Common_Image_Tools::HSVToRGB ((i / static_cast<float> (count_)) * 360.0f, 200 / 255.0f, 100 / 255.0f, r, g, b);
    olc::Pixel color_s;
    color_s.r = static_cast<uint8_t> (r * 255.0f);
    color_s.g = static_cast<uint8_t> (g * 255.0f);
    color_s.b = static_cast<uint8_t> (b * 255.0f);
    //color_s.a = 255;

    posX = (olc::PixelGameEngine::ScreenWidth () / 2.0f)  + std::sin ((frame_count_i - i) / 20.0f) * 200.0f;
    posY = (olc::PixelGameEngine::ScreenHeight () / 2.0f) + std::sin ((frame_count_i - i) / 10.0f) * 100.0f;
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (posX), static_cast<int32_t> (posY), static_cast<int32_t> (size_ + i / 2.0f), color_s);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
