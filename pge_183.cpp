#include "stdafx.h"

#include "pge_183.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_183::PGE_183 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 183");
}

bool
PGE_183::OnUserCreate ()
{
  return true;
}

bool
PGE_183::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  float s = Common_GL_Tools::map (std::sin (t_ / 9.0f), -1.0f, 1.0f, 28.0f, 8.0f);
  int32_t X = 100;
  for (float d = 0.0f; d < 1.0f; d += 1.0f / 500.0f)
  {
    float a = t_ + d * s;
    float x = Common_Math_Tools::lerp (X + 199.0f * std::cos (a / 2.0f - static_cast<float> (X)), static_cast<float> (X), d);
    float y = Common_Math_Tools::lerp (99.0f * std::sin (a) - 99.0f, static_cast<float> (ENGINE_PGE_183_DEFAULT_W), d);
    float r = static_cast<float> (static_cast<int32_t> (d * s));
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                    static_cast<int32_t> (x + std::cos (r) * 16.0f), static_cast<int32_t> (y + std::sin (r) * 16.0f),
                                    olc::WHITE, 0xFFFFFFFF);
  } // end FOR
  X = ENGINE_PGE_183_DEFAULT_W / 2;
  for (float d = 0.0f; d < 1.0f; d += 1.0f / 500.0f)
  {
    float a = t_ + d * s;
    float x = Common_Math_Tools::lerp (X + 199.0f * std::cos (a / 2.0f - static_cast<float> (X)), static_cast<float> (X), d);
    float y = Common_Math_Tools::lerp (99.0f * std::sin (a) - 99.0f, static_cast<float> (ENGINE_PGE_183_DEFAULT_W), d);
    float r = static_cast<float> (static_cast<int32_t> (d * s));
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                    static_cast<int32_t> (x + std::cos (r) * 16.0f), static_cast<int32_t> (y + std::sin (r) * 16.0f),
                                    olc::WHITE, 0xFFFFFFFF);
  } // end FOR
  X = ENGINE_PGE_183_DEFAULT_W - 100;
  for (float d = 0.0f; d < 1.0f; d += 1.0f / 500.0f)
  {
    float a = t_ + d * s;
    float x = Common_Math_Tools::lerp (X + 199.0f * std::cos (a / 2.0f - static_cast<float> (X)), static_cast<float> (X), d);
    float y = Common_Math_Tools::lerp (99.0f * std::sin (a) - 99.0f, static_cast<float> (ENGINE_PGE_183_DEFAULT_W), d);
    float r = static_cast<float> (static_cast<int32_t> (d * s));
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                    static_cast<int32_t> (x + std::cos (r) * 16.0f), static_cast<int32_t> (y + std::sin (r) * 16.0f),
                                    olc::WHITE, 0xFFFFFFFF);
  } // end FOR

  t_ += 0.1f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
