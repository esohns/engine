#include "stdafx.h"

#include "pge_140.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_140::PGE_140 ()
 : olc::PixelGameEngine ()
 , system_ ()
 , zoom_ (0.0f)
{
  sAppName = "Example 140";
}

bool
PGE_140::OnUserCreate ()
{
  zoom_ = ENGINE_PGE_140_DEFAULT_ZOOM;

  system_.initialize ();

  return true;
}

bool
PGE_140::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  system_.update ();
  system_.relativeNeighborhoodGraph (this, zoom_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
