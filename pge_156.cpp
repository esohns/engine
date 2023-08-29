#include "stdafx.h"

#include "pge_156.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_156::PGE_156 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , y_ (0)
{
  sAppName = "Example 156";
}

bool
PGE_156::OnUserCreate ()
{
  for (int i = ENGINE_PGE_156_DEFAULT_NUMBER_OF_PARTICLES; i--;)
    particles_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                           Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});

  return true;
}

bool
PGE_156::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    particles_.clear ();
    OnUserCreate ();
    y_ = 0;
    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  if (y_ < olc::PixelGameEngine::ScreenHeight ())
    for (int32_t x = olc::PixelGameEngine::ScreenWidth (); x--;)
    {
      float a = 0.0f;
      for (std::vector<olc::vf2d>::iterator iterator = particles_.begin ();
           iterator != particles_.end ();
           ++iterator)
        a += 3.0f * sin (3.0f * std::atan2 ((*iterator).y - y_, (*iterator).x - x));
      uint8_t b = static_cast<uint8_t> (Common_GL_Tools::map (std::sin (a), -1.0f, 1.0f, 0.0f, 255.0f));
      olc::PixelGameEngine::Draw (x, y_, {b, b, b, 255});
    } // end FOR
  
  y_++;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
