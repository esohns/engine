#include "stdafx.h"

#include "pge_395.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_395::PGE_395 ()
 : olc::PixelGameEngine ()
 , agents_ ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 395");
}

bool
PGE_395::OnUserCreate ()
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_395::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_395_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_395_DEFAULT_ALPHA_DECAY : 0);

  static int frame_count_i = 1;

  while (agents_.size () < ENGINE_PGE_395_DEFAULT_NUMBER_OF_AGENTS)
    agents_.push_back (agent (this, frame_count_i));

  std::vector<agent> new_agents_a;
  for (std::vector<agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
  {
    (*iterator).update (&noise_, this);

    bool remove_b = Common_Tools::getRandomNumber (0, 5) < 1;
    if (((*iterator).position_.x < 0.0f) || ((*iterator).position_.x > static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)))
      remove_b = true;
    if (((*iterator).position_.y < 0.0f) || ((*iterator).position_.y > static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1)))
      remove_b = true;
    if (!remove_b)
      new_agents_a.push_back (*iterator);
  } // end FOR
  agents_ = new_agents_a;

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
