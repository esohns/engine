#include "stdafx.h"

#include "pge_325.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_325::PGE_325 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , isQuad_ (false)
 , mult_ (ENGINE_PGE_325_DEFAULT_MULT)
 , r_ (0.0f)
 , columns_ (0)
 , rows_ (0)
 , back_ (false)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 325");
}

bool
PGE_325::OnUserCreate ()
{
  columns_ = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_325_DEFAULT_SIZE;
  rows_ = olc::PixelGameEngine::ScreenHeight () / ENGINE_PGE_325_DEFAULT_SIZE;

  float zoomX =
    2.0f * ENGINE_PGE_325_DEFAULT_ZOOM * (olc::PixelGameEngine::ScreenWidth () / static_cast<float> (columns_ + 1));
  float zoomY =
    ENGINE_PGE_325_DEFAULT_ZOOM * (olc::PixelGameEngine::ScreenHeight () / static_cast<float> (rows_ + 1));

  for (int i = 0; i < columns_; i++)
    for (int j = 0; j < rows_; j++)
      particles_.push_back (particle (zoomX * (i + 1 + (j % 4 == 0 || j % 4 == 3 ? 0.5f : 0.0f)) - (olc::PixelGameEngine::ScreenWidth () / static_cast<float> (4.0f)),
                                      zoomY * (j + 1) - (olc::PixelGameEngine::ScreenHeight () / static_cast<float> (4.0f))));

  r_ = 10.0f * mult_ * (1.0f + 3.0f * static_cast<int> (!isQuad_));

  return true;
}

bool
PGE_325::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  int32_t mouse_wheel_i = olc::PixelGameEngine::GetMouseWheel ();

  if (olc::PixelGameEngine::GetKey (olc::Key::C).bPressed || mouse_wheel_i < 0)
    if (mult_ > ENGINE_PGE_325_DEFAULT_R_MIN)
    {
      mult_--;
      r_ = 40.0f * mult_;
    } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::Q).bPressed) isQuad_ = !isQuad_;
  if (olc::PixelGameEngine::GetKey (olc::Key::X).bPressed || mouse_wheel_i > 0)
    if (mult_ < ENGINE_PGE_325_DEFAULT_R_MAX)
    {
      mult_++;
      r_ = 40.0f * mult_;
    } // end IF
  olc::HWButton space_s = olc::PixelGameEngine::GetKey (olc::Key::SPACE);
  back_ = (space_s.bHeld || space_s.bPressed);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    for (std::vector<particle>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      (*iterator).moveToMouse (r_, mult_, isQuad_, this);
  if (olc::PixelGameEngine::GetMouse (1).bHeld)
    for (std::vector<particle>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      (*iterator).moveToHome (r_, mult_, isQuad_, this);

  if (back_)
    for (std::vector<particle>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      (*iterator).backToOrigin (10.0f);

  olc::vf2d mouse_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                     static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  olc::Pixel color (255, 0, 255 * static_cast<int> (!isQuad_), 50);
  olc::PixelGameEngine::FillCircle (mouse_s, static_cast<int32_t> (r_), color);

  static olc::Pixel color_2 (0, 255, 255, 255);
  for (int i = 0; i < columns_ - 1; i++)
    for (int j = 0; j < rows_ - 1; j++)
    {
      olc::PixelGameEngine::DrawLine (particles_[i * rows_ + j].position_, particles_[i * rows_ + (j + 1)].position_, color_2, 0xFFFFFFFF);
      switch (j % 4)
      {
        case 0:
          olc::PixelGameEngine::DrawLine (particles_[i * rows_ + j].position_, particles_[(i + 1) * rows_ + (j + 1)].position_, color_2, 0xFFFFFFFF);
          break;
        case 2:
          if (i > 0) olc::PixelGameEngine::DrawLine (particles_[i * rows_ + j].position_, particles_[(i - 1) * rows_ + (j + 1)].position_, color_2, 0xFFFFFFFF);
          break;
      } // end SWITCH
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

