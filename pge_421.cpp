#include "stdafx.h"

#include "pge_421.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_421::PGE_421 ()
 : olc::PixelGameEngine ()
 , tree_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 421");
}

PGE_421::~PGE_421 ()
{
  tree_.clear ();
}

bool
PGE_421::OnUserCreate ()
{
  return true;
}

bool
PGE_421::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetKey (olc::Key::C).bPressed)
    tree_.clear ();
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    tree_.addLeaves (this);

  olc::PixelGameEngine::Clear (olc::BLACK);

  tree_.grow ();
  tree_.show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
