#include "stdafx.h"

#include "pge_193.h"

#include "ace/Log_Msg.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_193::PGE_193 ()
 : olc::PixelGameEngine ()
 , creatures_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 193");
}

bool
PGE_193::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_193_DEFAULT_NUMBER_OF_CREATURES; i++)
  {
    olc::vf2d position (static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth() - 1)),
                        static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight() - 1)));
    creatures_.push_back (creature (position, 15.0f, 15.0f, 12, 30, 10.0f, Common_Tools::getRandomNumber (1.0f, 5.0f)));
  } // end FOR

  return true;
}

bool
PGE_193::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (size_t i = 0; i < creatures_.size (); i++)
    creatures_[i].update (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_193::easeInOutExpo (float t, float b, float c, float d)
{
  if (t == 0.0f) return b;
  if (t == d) return b + c;
  t /= d / 2.0f;
  if (t < 1.0f)
    return (c / 2.0f) * static_cast<float> (std::pow (2, 10.0f * (t - 1.0f))) + b;
  t -= 1.0f;
  return (c / 2.0f) * (static_cast<float> (-std::pow (2, -10.0f * t)) + 2.0f) + b;
}
