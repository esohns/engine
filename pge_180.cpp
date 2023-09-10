#include "stdafx.h"

#include "pge_180.h"

#include "ace/Log_Msg.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_180::PGE_180 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 180");
}

bool
PGE_180::OnUserCreate ()
{
  return true;
}

bool
PGE_180::OnUserUpdate (float fElapsedTime)
{
  static int32_t h = ENGINE_PGE_180_DEFAULT_W / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (float r = 0.0f; r < static_cast<float> (M_PI) / 2.0f * 3.0f; r += static_cast<float> (M_PI) / 2.0f)
    for (float tt = 0.0f; tt < 1.0f; tt += 1.0f / 500.0f)
    {
      float a = t_ - tt * ENGINE_PGE_180_DEFAULT_D;
      float x = Common_Math_Tools::lerp (h + 99 * std::cos (a / 4), std::cos (r) * h + h, tt);
      float y = Common_Math_Tools::lerp (h / 4.0f + 49 * std::sin (a / 3.0f), std::sin (r) * h + h, tt);
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                      static_cast<int32_t> (x + std::cos (a * a / 19) * 20.0f), static_cast<int32_t> (y + std::sin (a * a / 19.0f) * 20.0f),
                                      olc::BLACK, 0xFFFFFFFF);
    } // end FOR

  t_ += 0.2f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
