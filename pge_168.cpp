#include "stdafx.h"

#include "pge_168.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_168::PGE_168 ()
 : olc::PixelGameEngine ()
 , counter_ (0.0f)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 168");
}

bool
PGE_168::OnUserCreate ()
{

  return true;
}

bool
PGE_168::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  counter_ += 0.03f;

  D (0.0f, 0.0f, static_cast<float> (ENGINE_PGE_168_DEFAULT_W - 1), static_cast<float> (ENGINE_PGE_168_DEFAULT_W - 1), 8);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_168::D (float x, float y, float w, float h, int32_t step)
{
  if (step == 0) return;

  //strokeWeight(step)
  olc::PixelGameEngine::DrawRect (static_cast<int32_t> (x), static_cast<int32_t> (y),
                                  static_cast<int32_t> (w), static_cast<int32_t> (h),
                                  olc::BLACK);
  float n =
    //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / ENGINE_PGE_168_DEFAULT_W * 2.0f * static_cast<float> (M_PI),
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (x / ENGINE_PGE_168_DEFAULT_W * 2.0f * static_cast<float> (M_PI),
                                                               y / ENGINE_PGE_168_DEFAULT_W * 2.0f * static_cast<float> (M_PI),
                                                               counter_ / static_cast<float> (step))),
                          -1.0f, 1.0f, 0.0f, 1.0f);
  --step;
  if (step % 2)
  {
    D (x, y, w * n, h, step);
    D (x + w * n, y, w - w * n, h, step);
  } // end IF
  else
  {
    D (x, y, w, h * n, step);
    D (x, y + h * n, w, h - h * n, step);
  } // end ELSE
}
