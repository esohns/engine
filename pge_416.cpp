#include "stdafx.h"

#include "pge_416.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_416::PGE_416 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , agents_ ()
 , world_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 416");
}

bool
PGE_416::OnUserCreate ()
{
  std::vector<bool> row_a (olc::PixelGameEngine::ScreenHeight (), false);
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i++)
    world_.push_back (row_a);

  agents_.push_back (agent (world_, this));

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_416::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::WHITE);

    world_.clear ();
    std::vector<bool> row_a (olc::PixelGameEngine::ScreenHeight (), false);
    for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i++)
      world_.push_back (row_a);

    agents_.clear ();
    agents_.push_back (agent (world_, this));
  } // end IF

  for (int i = 0; i < ENGINE_PGE_416_DEFAULT_AGENT_SPEED; i++)
    for (std::vector<agent>::iterator iterator = agents_.begin ();
         iterator != agents_.end ();
         ++iterator)
      (*iterator).update (world_, &noise_, frame_count_i, this);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
