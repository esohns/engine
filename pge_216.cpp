#include "stdafx.h"

#include "pge_216.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_216::PGE_216 ()
 : olc::PixelGameEngine ()
 , a_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 216");
}

bool
PGE_216::OnUserCreate ()
{


  return true;
}

bool
PGE_216::OnUserUpdate (float fElapsedTime)
{
  static int32_t halfw = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t halfh = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int32_t x = -10; x < 10; x++)
    for (int32_t z = -10; z < 10; z++)
    {
      int32_t y = static_cast<int32_t> (40.0f * std::cos (0.55f * distance (x, z, 0, 0) + a_));

      float xm = x * 17.0f - 8.5f;
      float xt = x * 17.0f + 8.5f;
      float zm = z * 17.0f - 8.5f;
      float zt = z * 17.0f + 8.5f;

      int32_t isox1 = static_cast<int32_t> (xm - zm + halfw);
      int32_t isoy1 = static_cast<int32_t> ((xm + zm) * 0.5f + halfh);
      int32_t isox2 = static_cast<int32_t> (xm - zt + halfw);
      int32_t isoy2 = static_cast<int32_t> ((xm + zt) * 0.5f + halfh);
      int32_t isox3 = static_cast<int32_t> (xt - zt + halfw);
      int32_t isoy3 = static_cast<int32_t> ((xt + zt) * 0.5f + halfh);
      int32_t isox4 = static_cast<int32_t> (xt - zm + halfw);
      int32_t isoy4 = static_cast<int32_t> ((xt + zm) * 0.5f + halfh);

      olc::Pixel color (Common_Tools::getRandomNumber (100, 200), Common_Tools::getRandomNumber (200, 220), Common_Tools::getRandomNumber (240, 255), 255);
      olc::PixelGameEngine::DrawLine (isox2, isoy2 - y, isox3, isoy3 - y, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox3, isoy3 - y, isox3, isoy3 + 40, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox3, isoy3 + 40, isox2, isoy2 + 40, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox2, isoy2 + 40, isox2, isoy2 - y, color, 0xFFFFFFFF);

      color =
        olc::Pixel (Common_Tools::getRandomNumber (240, 255), Common_Tools::getRandomNumber (100, 200), Common_Tools::getRandomNumber (200, 220), 255);
      olc::PixelGameEngine::DrawLine (isox3, isoy3 - y, isox4, isoy4 - y, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox4, isoy4 - y, isox4, isoy4 + 40, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox4, isoy4 + 40, isox3, isoy3 + 40, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox3, isoy3 + 40, isox3, isoy3 - y, color, 0xFFFFFFFF);
      
      color = olc::Pixel (237, 237, 230);
      olc::PixelGameEngine::DrawLine (isox1, isoy1 - y, isox2, isoy2 - y, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox2, isoy2 - y, isox3, isoy3 - y, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox3, isoy3 - y, isox4, isoy4 - y, color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (isox4, isoy4 - y, isox1, isoy1 - y, color, 0xFFFFFFFF);
    } // end FOR

  a_ -= 0.1f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_216::distance (int32_t x, int32_t y, int32_t cx, int32_t cy)
{
  return static_cast<float> (std::sqrt ((cx - x) * (cx - x) + (cy - y) * (cy - y)));
}
