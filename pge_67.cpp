#include "stdafx.h"

#include "pge_67.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_67::PGE_67 ()
 : olc::PixelGameEngine ()
 , friends_ ()
{
  sAppName = "Example 67";
}

PGE_67::~PGE_67 ()
{
  for (std::vector<friend_*>::iterator iterator = friends_.begin ();
       iterator != friends_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_67::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  olc::PixelGameEngine::Clear (olc::WHITE);

  reset ();

  return true;
}

bool
PGE_67::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::WHITE);

    reset ();
  } // end IF

  for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS; i++)
    friends_[i]->update ();
  for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS; i++)
  {
    friends_[i]->expose (this);
    friends_[i]->exposeConnections (friends_, this);
  } // end FOR
  if (frame_count_i % 2 == 0)
    for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS; i++)
      friends_[i]->findHappyPlace (friends_);

  frame_count_i++;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_67::reset ()
{
  for (std::vector<friend_*>::iterator iterator = friends_.begin ();
       iterator != friends_.end ();
       ++iterator)
    delete *iterator;
  friends_.clear ();

  for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS; i++)
  {
    float fx =
      static_cast<float> (olc::PixelGameEngine::ScreenWidth ()  / 2.0f + 0.4f * ENGINE_PGE_67_DEFAULT_DIMENSIONS * std::cos (M_PI * 2.0 * i / static_cast<float> (ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS)));
    float fy =
      static_cast<float> (olc::PixelGameEngine::ScreenHeight () / 2.0f + 0.4f * ENGINE_PGE_67_DEFAULT_DIMENSIONS * std::sin (M_PI * 2.0 * i / static_cast<float> (ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS)));
    friends_.push_back (new friend_ (fx, fy, i));
  } // end FOR

  for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS * 2.2; i++)
  {
    int a = Common_Tools::getRandomNumber (0, ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS - 1);
    int b = static_cast<int> (std::fmod (std::floor (a + Common_Tools::getRandomNumber (0.0f, 22.0f)), static_cast<float> (ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS)));
    if (b >= ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS)
      b = 0;
    else if (b < 0)
      b = 0;
    if (a != b)
    {
      friends_[a]->connectTo (b);
      friends_[b]->connectTo (a);
    } // end IF
  } // end FOR
}
