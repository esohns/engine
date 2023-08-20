#include "stdafx.h"

#include "pge_128.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_128::PGE_128 ()
 : olc::PixelGameEngine ()
 , counter_ (0.0f)
 , mX_ (0.0f)
 , mY_ (0.0f)
 , noise_ ()
{
  sAppName = "Example 128";
}

bool
PGE_128::OnUserCreate ()
{
  return true;
}

bool
PGE_128::OnUserUpdate (float fElapsedTime)
{
  mX_ = olc::PixelGameEngine::GetMouseX () * 0.001f;
  mY_ = olc::PixelGameEngine::GetMouseY () * 0.001f;

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); j += ENGINE_PGE_127_DEFAULT_UNIT)
    for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i += ENGINE_PGE_127_DEFAULT_UNIT)
      drawRect (i, j, ENGINE_PGE_127_DEFAULT_UNIT);

  counter_ += 0.002f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_128::drawRect (int x, int y, int size)
{
  float cx = x + size / 2.0f;
  float cy = y + size / 2.0f;
  float n =
    Common_GL_Tools::map (static_cast<float> (noise_.GetValue (cx / static_cast<double> (olc::PixelGameEngine::ScreenWidth ()) - mX_,
                                                               cy / static_cast<double> (olc::PixelGameEngine::ScreenHeight ()) - mY_,
                                                               std::sqrt (cx * cx + cy * cy) / static_cast<double> (olc::PixelGameEngine::ScreenHeight ()) - counter_)),
                          -1.0f, 1.0f, 0.0f, 1.0f);
  float threshold =
    Common_GL_Tools::map (static_cast<float> (size), 4.0f, static_cast<float> (ENGINE_PGE_127_DEFAULT_UNIT), 0.05f, 0.35f);
  if (std::abs (n - 0.5f) > threshold)
    olc::PixelGameEngine::DrawRect (x, y, size, size, olc::BLACK);
  else
  {
    size = size / 2;
    if (size >= 4)
    {
      drawRect (x, y, size);
      drawRect (x + size, y, size);
      drawRect (x, y + size, size);
      drawRect (x + size, y + size, size);
    } // end IF
  } // end ELSE
}
