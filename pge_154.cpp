#include "stdafx.h"

#include "pge_154.h"

#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_154::PGE_154 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , t_ (0.0f)
{
  sAppName = "Example 154";
}

bool
PGE_154::OnUserCreate ()
{
  return true;
}

bool
PGE_154::OnUserUpdate (float fElapsedTime)
{
  t_ += 0.1f;

  for (int32_t y = ENGINE_PGE_154_DEFAULT_RANGE; y--;)
    for (int32_t x = ENGINE_PGE_154_DEFAULT_RANGE; x--;)
    {
      float c = mesh (Common_GL_Tools::map (static_cast<float> (x), 0.0f, static_cast<float> (ENGINE_PGE_154_DEFAULT_RANGE - 1), -10.0f, 10.0f),
                      Common_GL_Tools::map (static_cast<float> (y), 0.0f, static_cast<float> (ENGINE_PGE_154_DEFAULT_RANGE - 1), -10.0f, 10.0f));
      olc::Pixel color (static_cast<uint8_t> (c * 4.0f > 255.0f ? 255.0f : c * 4.0f),
                        static_cast<uint8_t> (c * 2.0f > 255.0f ? 255.0f : c * 2.0f),
                        static_cast<uint8_t> (c        > 255.0f ? 255.0f : c),
                        255);
      olc::PixelGameEngine::Draw (x, y, color);
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_154::mesh (float x, float y, int n)
{
  return n ? mesh (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / 3.0, y / 3.0 + t_ / 3.0, 0.0)),
                                         -1.0f, 1.0f, 0.0f, 1.0f) * x * y,
                   std::sin (x) + std::sin (y + t_),
                   n - 1)
           : (x - y) * (x - y) * 16.0f;
}
