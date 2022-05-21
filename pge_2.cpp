#include "stdafx.h"

#include "pge_2.h"

#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "engine_common.h"

PGE_2::PGE_2 ()
 : columns_ (0)
 , rows_ (0)
 , scaleFactor_ (0)
 , x_ (0.0)
 , y_ (0.0)
 , z_ (0.0)
 , step_ (0.0)
 , module_ ()
{
  sAppName = "Example 2";
}

bool
PGE_2::OnUserCreate ()
{
  scaleFactor_ = 20;
  columns_ = ScreenWidth () / scaleFactor_;
  rows_ = ScreenHeight () / scaleFactor_;

  x_ = 1.25;
  y_ = 0.75;
  z_ = 0.5;
  step_ = 0.01;
  return true;
}

bool
PGE_2::OnUserUpdate (float fElapsedTime)
{
  // Clear Screen
  //olc::PixelGameEngine::Clear (olc::Pixel (0, 0, 0, 255)); // black

  double value = module_.GetValue (x_, y_, z_);
  x_ += step_;

  glTranslatef (ScreenWidth () / 2, ScreenHeight () / 2, 0);
  glRotatef (static_cast<float> (M_PI) / 3.0F, 1.0F, 0.0F, 0.0F);

  for (int x = 0; x < columns_; ++x)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int y = 0; y < rows_; ++y)
    {
      glVertex3i (x * scaleFactor_, y * scaleFactor_, 0);
      glVertex3i (x * scaleFactor_, (y + 1) * scaleFactor_, 0);
      //olc::PixelGameEngine::DrawRect ({x * scaleFactor_, y * scaleFactor_},
      //                                {scaleFactor_, scaleFactor_},
      //                                olc::WHITE);
    } // end FOR
    glEnd ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
