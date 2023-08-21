#include "stdafx.h"

#include "pge_134.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_134::PGE_134 ()
 : olc::PixelGameEngine ()
 , system_ ()
{
  sAppName = "Example 134";
}

bool
PGE_134::OnUserCreate ()
{
  system_.initialize ();
  system_.update (0);
  system_.relativeNeighborhoodGraph ();

  return true;
}

bool
PGE_134::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::WHITE);

  system_.update (frame_count_i);
  system_.show (this);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
