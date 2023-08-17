#include "stdafx.h"

#include "pge_115.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_115::PGE_115 ()
 : olc::PixelGameEngine ()
 , positions_ ()
 , velocities_ ()
 , accelerations_ ()
{
  sAppName = "Example 115";
}

bool
PGE_115::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_115_DEFAULT_NUMBER_OF_PARTICLES; i++)
  {
    positions_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                           Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});
    velocities_.push_back ({0.0f, 0.0f});
    accelerations_.push_back ({0.0f, 0.0f});
  } // end FOR

  return true;
}

bool
PGE_115::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    positions_.clear ();
    velocities_.clear ();
    accelerations_.clear ();
    for (int i = 0; i < ENGINE_PGE_115_DEFAULT_NUMBER_OF_PARTICLES; i++)
    {
      positions_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                             Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});
      velocities_.push_back ({0.0f, 0.0f});
      accelerations_.push_back ({0.0f, 0.0f});
    } // end FOR

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  olc::vf2d mouse_position (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                            static_cast<float> (olc::PixelGameEngine::GetMouseY ()));

  for (int i = 0; i < ENGINE_PGE_115_DEFAULT_NUMBER_OF_PARTICLES; i++)
  {
    float distance = positions_[i].dist (mouse_position);
    if (distance > 3.0f)
      accelerations_[i] = {ENGINE_PGE_115_DEFAULT_MAGNETISM * (mouse_position.x - positions_[i].x) / (distance * distance),
                           ENGINE_PGE_115_DEFAULT_MAGNETISM * (mouse_position.y - positions_[i].y) / (distance * distance)};
    velocities_[i] += accelerations_[i];
    velocities_[i] *= ENGINE_PGE_115_DEFAULT_GENSOKU;

    positions_[i] += velocities_[i];

    float sokudo = velocities_[i].dist ({0.0f, 0.0f});
    uint8_t r = static_cast<uint8_t> (Common_GL_Tools::map (sokudo, 0.0f, 5.0f, 0.0f, 255.0f));
    uint8_t g = static_cast<uint8_t> (Common_GL_Tools::map (sokudo, 0.0f, 5.0f, 64.0f, 255.0f));
    uint8_t b = static_cast<uint8_t> (Common_GL_Tools::map (sokudo, 0.0f, 5.0f, 128.0f, 255.0f));
    olc::PixelGameEngine::Draw (positions_[i], {r, g, b, 32});
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
