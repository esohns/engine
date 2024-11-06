#include "stdafx.h"

#include "pge_396.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_396::PGE_396 ()
 : olc::PixelGameEngine ()
 , agents_ ()
 , world_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 396");
}

bool
PGE_396::OnUserCreate ()
{
  std::vector<bool> row_a (olc::PixelGameEngine::ScreenHeight (), false);
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i++)
    world_.push_back (row_a);

  for (int i = 0; i < ENGINE_PGE_396_DEFAULT_NUMBER_OF_AGENTS; i++)
    agents_.push_back (agent (world_, this));

  return true;
}

bool
PGE_396::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::BLACK);

    world_.clear ();
    std::vector<bool> row_a (olc::PixelGameEngine::ScreenHeight (), false);
    for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i++)
      world_.push_back (row_a);

    agents_.clear ();
    for (int i = 0; i < ENGINE_PGE_396_DEFAULT_NUMBER_OF_AGENTS; i++)
      agents_.push_back (agent (world_, this));
  } // end IF

  for (int i = 0; i < ENGINE_PGE_396_DEFAULT_AGENT_SPEED; i++)
    for (std::vector<agent>::iterator iterator = agents_.begin ();
         iterator != agents_.end ();
         ++iterator)
      (*iterator).update (world_, this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
