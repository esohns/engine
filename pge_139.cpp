#include "stdafx.h"

#include "pge_139.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_139::PGE_139 ()
 : olc::PixelGameEngine ()
 , vertices_ ()
 , zoom_ (0.0f)
{
  sAppName = "Example 139";
}

bool
PGE_139::OnUserCreate ()
{
  zoom_ = ENGINE_PGE_139_DEFAULT_ZOOM;

  return true;
}

bool
PGE_139::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    vertices_.clear ();

  olc::PixelGameEngine::Clear (olc::BLACK);

  vertices_.spawn (frame_count_i);
  vertices_.update ();
  vertices_.show (this, zoom_);
  vertices_.connectVertices (this, zoom_);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
