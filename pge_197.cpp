#include "stdafx.h"

#include "pge_197.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_197::PGE_197 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 197");
}

bool
PGE_197::OnUserCreate ()
{
  return true;
}

float
PGE_197::wave (float x, float y, float freq, float phase, float min, float max)
{
  olc::vf2d pos (x, y);
  float dist1 = pos.dist ({0.0f, 0.0f});
  float dist2 = pos.dist({static_cast<float> (ENGINE_PGE_197_DEFAULT_WIDTH), 0.0f});
  float dist3 = pos.dist ({0.0f, static_cast<float> (ENGINE_PGE_197_DEFAULT_HEIGHT)});
  float dist4 = pos.dist ({static_cast<float> (ENGINE_PGE_197_DEFAULT_WIDTH), static_cast<float> (ENGINE_PGE_197_DEFAULT_HEIGHT)});
  float wave1 = std::sin (static_cast<float> (M_PI) / 180.0f * (((dist1 * freq) + phase)));
  float wave2 = std::sin (static_cast<float> (M_PI) / 180.0f * (((dist2 * freq) + phase)));
  float wave3 = std::sin (static_cast<float> (M_PI) / 180.0f * (((dist3 * freq) + phase)));
  float wave4 = std::sin (static_cast<float> (M_PI) / 180.0f * (((dist4 * freq) + phase)));
  return Common_GL_Tools::map (wave1 + wave2 + wave3 + wave4, -1.0f, 1.0f, min, max);
}

bool
PGE_197::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();

  for (int i = 0; i < ENGINE_PGE_197_DEFAULT_WIDTH; i++)
    for (int j = 0; j < ENGINE_PGE_197_DEFAULT_HEIGHT; j++)
    {
      float temp =
        wave (static_cast<float> (i), static_cast<float> (j), 20.0f, static_cast<float> (-frame_count_i * 5), 0.0f, 255.0f);
      if (temp < 0.0f) temp = 0.0f;
      else if (temp > 255.0f) temp = 255.0f;
      olc::Pixel color = {static_cast<uint8_t> (temp), static_cast<uint8_t> (temp), static_cast<uint8_t> (temp), 255};
      p[i + j * ENGINE_PGE_197_DEFAULT_WIDTH] = color;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
