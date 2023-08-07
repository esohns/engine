#include "stdafx.h"

#include "pge_79.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_79::PGE_79 ()
 : olc::PixelGameEngine ()
 , agents_ ()
 , noise_ ()
{
  sAppName = "Example 79";
}

PGE_79::~PGE_79 ()
{
  for (std::vector<agent*>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_79::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_79_DEFAULT_NUMBER_OF_AGENTS; i++)
    agents_.push_back (new agent (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));

  return true;
}

bool
PGE_79::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_79_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_79_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    for (std::vector<agent*>::iterator iterator = agents_.begin ();
         iterator != agents_.end ();
         ++iterator)
      delete *iterator;
    agents_.clear ();
    for (int i = 0; i < ENGINE_PGE_79_DEFAULT_NUMBER_OF_AGENTS; i++)
      agents_.push_back (new agent (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                                    Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  static int frame_count_i = 1;

  for (std::vector<agent*>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
  {
    (*iterator)->show (this);
    (*iterator)->update (&noise_, frame_count_i, this);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
