#include "stdafx.h"

#include "pge_175.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_175::PGE_175 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 175");
}

bool
PGE_175::OnUserCreate ()
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_175::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  for (int y = 15; y--;)
    for (int x = 15; x--;)
    {
      olc::PixelGameEngine::FillRect (x * ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH, y * ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH,
                                      ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH - 2, ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH - 2,
                                      olc::BLACK);
      float s =
        Common_GL_Tools::map (std::sin (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (x / 15.0, y / 15.0 - frame_count_i / 300.0, 0.0)),
                                                              -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI)),
                              -1.0f, 1.0f, 0.0f, static_cast<float> (ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH - 1));
      olc::PixelGameEngine::FillRect (x * ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH, y * ENGINE_PGE_175_DEFAULT_RECTANGLE_WIDTH,
                                      static_cast<int32_t> (s), static_cast<int32_t> (s),
                                      olc::WHITE);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
