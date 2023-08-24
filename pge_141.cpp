#include "stdafx.h"

#include "pge_141.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_141::PGE_141 ()
 : olc::PixelGameEngine ()
 , system_ ()
 , zoom_ (0.0f)
{
  sAppName = "Example 141";
}

bool
PGE_141::OnUserCreate ()
{
  zoom_ = ENGINE_PGE_141_DEFAULT_ZOOM;

  return true;
}

bool
PGE_141::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::WHITE);

  if (frame_count_i % 2 == 0)
    system_.spawn ();
  system_.update ();
  system_.connectPoints (this, zoom_);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
