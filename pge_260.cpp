#include "stdafx.h"

#include "pge_260.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_260::PGE_260 ()
 : olc::PixelGameEngine ()
 , positions_ ()
 , velocities_ ()
 , num_ (ENGINE_PGE_260_DEFAULT_NUMBER_OF_PARTICLES)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 260");
}

bool
PGE_260::OnUserCreate ()
{
  num_ = ENGINE_PGE_260_DEFAULT_NUMBER_OF_PARTICLES;
  resetParticles ();

  return true;
}

bool
PGE_260::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int frames_down_i = 0;

  olc::PixelGameEngine::Clear (olc::BLACK);

  olc::HWButton mouse_0_s = olc::PixelGameEngine::GetMouse (0);

  if (frame_count_i % 20 == 0)
  {
    //uint32_t fps_i = olc::PixelGameEngine::GetFPS ();
    //if (fps_i > 55)
    //{
    //  for (int i = 0; i < 10; i++)
    //  {
    //    positions_.push_back (olc::vf2d (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f));
    //    velocities_.push_back (olc::vf2d (0.0f, 0.0f));
    //  } // end FOR
    //  num_ += 10;
    //}
    //else if (fps_i > 50)
    //  num_ -= 10;
  } // end IF

  if (mouse_0_s.bPressed || mouse_0_s.bHeld)
    frames_down_i++;

  for (int i = 0; i < num_; i++)
  {
    positions_[i] += velocities_[i];
    if (positions_[i].x >= static_cast<float> (olc::PixelGameEngine::ScreenWidth ()))
      positions_[i].x -= static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
    if (positions_[i].x < 0.0f)
      positions_[i].x += static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
    if (positions_[i].y >= static_cast<float> (olc::PixelGameEngine::ScreenHeight ()))
      positions_[i].y -= static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
    if (positions_[i].y < 0.0f)
      positions_[i].y += static_cast<float> (olc::PixelGameEngine::ScreenHeight ());

    olc::vf2d temp (Common_Tools::getRandomNumber (-1.0f, 1.0f), Common_Tools::getRandomNumber (-1.0f, 1.0f));
    temp *= 0.2f;
    velocities_[i] += temp;
    velocities_[i] *= 0.991f;

    float r, g, b;
    float hue_f = std::fmod (frame_count_i * 3 + i / static_cast<float> (num_) * 360.0f, 360.0f);
    Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
    olc::Pixel color;
    color.r = static_cast<uint8_t> (r * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (positions_[i].x), static_cast<int32_t> (positions_[i].y),
                                    static_cast<int32_t> (positions_[i].x - velocities_[i].x * 5.0f), static_cast<int32_t> (positions_[i].y - velocities_[i].y * 5.0f),
                                    color, 0xFFFFFFFF);
  
    if (mouse_0_s.bPressed || mouse_0_s.bHeld)
    {
      float baseAttraction = static_cast<float> (std::sqrt (frames_down_i));
      olc::vf2d mouse_position_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
      float attraction = baseAttraction / std::pow (positions_[i].dist (mouse_position_s), 0.1f);
      olc::vf2d attractionVector (mouse_position_s.x - positions_[i].x, mouse_position_s.y - positions_[i].y);
      attractionVector = attractionVector.norm ();
      attractionVector *= attraction;
      velocities_[i] += attractionVector;
    } // end IF
  } // end FOR

  if (mouse_0_s.bReleased)
    frames_down_i = 0;

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_260::resetParticles ()
{
  positions_.clear ();
  velocities_.clear ();

  for (int i = 0; i < num_; i++)
  {
    positions_.push_back (olc::vf2d (olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f));
    velocities_.push_back (olc::vf2d (Common_Tools::getRandomNumber (-1.0f, 1.0f), Common_Tools::getRandomNumber (-1.0f, 1.0f)));
  } // end FOR
}
