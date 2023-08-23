#include "stdafx.h"

#include "pge_137.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_137::PGE_137 ()
 : olc::PixelGameEngine ()
 , tunnel_ ()
 , noise_ ()
 , zoom_ (0.0f)
{
  sAppName = "Example 137";
}

bool
PGE_137::OnUserCreate ()
{
  zoom_ = ENGINE_PGE_137_DEFAULT_ZOOM;

  return true;
}

bool
PGE_137::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  if ((frame_count_i % 4) == 0)
    tunnel_.spawn (&noise_, frame_count_i);
  tunnel_.update ();
  tunnel_.connectVertices (this, zoom_);
  tunnel_.blink (frame_count_i, this, zoom_);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
