#include "stdafx.h"

#include "pge_346.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_346::PGE_346 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
 , hue_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 346");
}

bool
PGE_346::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  hue_ = Common_Tools::getRandomNumber (0.0f, 360.0f);

  return true;
}

bool
PGE_346::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a >= ENGINE_PGE_346_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_346_DEFAULT_ALPHA_DECAY : 0);

  float r, g, b;
  for (int x = 10; x <= olc::PixelGameEngine::ScreenWidth (); x += 20)
    for (int y = 10; y <= olc::PixelGameEngine::ScreenHeight (); y += 20)
    {
      Common_Image_Tools::HSVToRGB (std::fmod (hue_ + x / static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) * 360.0f, 360.0f), 200.0f / 255.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 130);
      float d = 30.0f * std::sin ((static_cast<float> (M_PI) / 180.0f) * (x + y));
      int32_t radius_i = static_cast<int32_t> (d / 2.0f);
      if (radius_i > 0)
        olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x + 20.0f * std::sin ((static_cast<float> (M_PI) / 180.0f) * (t_ + x))),
                                          static_cast<int32_t> (y + 20.0f * std::sin ((static_cast<float> (M_PI) / 180.0f) * (t_ + y))),
                                          radius_i, color);
      
      t_ += 0.002f;
    } // end FOR
  hue_ += 1.0f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
