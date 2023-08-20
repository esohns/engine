#include "stdafx.h"

#include "pge_124.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_124::PGE_124 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , positions_ ()
{
  sAppName = "Example 124";
}

bool
PGE_124::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  reset ();

  return true;
}

bool
PGE_124::OnUserUpdate (float fElapsedTime)
{
  //static int32_t prev_mouse_x = olc::PixelGameEngine::GetMouseX ();
  //static int32_t prev_mouse_y = olc::PixelGameEngine::GetMouseY ();

  //olc::vf2d mouse_position;
  //if (prev_mouse_x != olc::PixelGameEngine::GetMouseX () || prev_mouse_y != olc::PixelGameEngine::GetMouseY ())
  //  mouse_position = {static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
  //                    static_cast<float> (olc::PixelGameEngine::GetMouseY ())};
  //else
  //  mouse_position = {-ENGINE_PGE_124_DEFAULT_MAX_DIST_MOUSE, -ENGINE_PGE_124_DEFAULT_MAX_DIST_MOUSE};
  //prev_mouse_x = olc::PixelGameEngine::GetMouseX ();
  //prev_mouse_y = olc::PixelGameEngine::GetMouseY ();

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    reset ();

  for (int i = 0; i < ENGINE_PGE_124_DEFAULT_NUMBER_OF_POINTS; i++)
    points_[i].process (this, points_);
  for (int i = 0; i < ENGINE_PGE_124_DEFAULT_NUMBER_OF_POINTS; i++)
    points_[i].update (this);

  olc::PixelGameEngine::Clear (olc::BLACK);

  std::vector<triangle> triangles;
  triangulator::triangulate (positions_, triangles);
  olc::Pixel color;
  color.b = 255;
  color.a = 255;
  for (std::vector<triangle>::iterator iterator = triangles.begin ();
       iterator != triangles.end ();
       ++iterator)
  {
    color.r =
      static_cast<uint8_t> (Common_GL_Tools::map (static_cast<int32_t> ((*iterator).p1->x), 0, olc::PixelGameEngine::ScreenWidth () - 1, 0, 255));
    color.g =
      static_cast<uint8_t> (Common_GL_Tools::map (static_cast<int32_t> ((*iterator).p1->y), 0, olc::PixelGameEngine::ScreenHeight () - 1, 0, 255));
    olc::PixelGameEngine::FillTriangle (*(*iterator).p1, *(*iterator).p2, *(*iterator).p3, color);
    olc::PixelGameEngine::DrawTriangle (*(*iterator).p1, *(*iterator).p2, *(*iterator).p3, olc::BLACK);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_124::reset ()
{
  points_.clear ();
  for (int i = 0; i < ENGINE_PGE_124_DEFAULT_NUMBER_OF_POINTS; i++)
  {
    float x = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
    float y = Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
    points_.push_back (point (x, y, i));
  } // end FOR

  positions_.clear ();
  for (int i = 0; i < ENGINE_PGE_124_DEFAULT_NUMBER_OF_POINTS; i++)
    positions_.push_back (&points_[i].position_);
}
