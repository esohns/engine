#include "stdafx.h"

#include "pge_288.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_13.h"

PGE_288::PGE_288 ()
 : olc::PixelGameEngine ()
 , arcs_ ()
 , a_ (0.0f)
 , h_ (0.0f)
 , da_ (0.0f)
 , t_ (0.0f)
 , l_ (0.0f)
 , t0_ (0.0f)
 , t1_ (0.0f)
 , rad_ (0.0f)
 , dt_ (ENGINE_PGE_288_DEFAULT_DT)
 , dt0_ (ENGINE_PGE_288_DEFAULT_DT)
 , dt1_ (ENGINE_PGE_288_DEFAULT_DT)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 288");
}

bool
PGE_288::OnUserCreate ()
{
  setAll ();

  t_ = Common_Tools::getRandomNumber (0.0f, 1000.0f);
  t0_ = Common_Tools::getRandomNumber (0.0f, 1000.0f);
  t1_ = Common_Tools::getRandomNumber (0.0f, 1000.0f);

  return true;
}

bool
PGE_288::OnUserUpdate (float fElapsedTime)
{
  static olc::vi2d pmouse_s (olc::PixelGameEngine::GetMouseX (),
                             olc::PixelGameEngine::GetMouseY ());
  olc::vi2d mouse_s (olc::PixelGameEngine::GetMouseX (),
                     olc::PixelGameEngine::GetMouseY ());
  addArcs (mouse_s.x, mouse_s.y, pmouse_s.x, pmouse_s.y);

  for (int i = static_cast<int> (arcs_.size ()) - 1; i >= 0; i--)
    arcs_[i].show (Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (arcs_.size () - 1) + 0.001f, 0.0f, 1.0f), this);

  for (int i = 0; i < static_cast<int> (arcs_.size ()) - ENGINE_PGE_288_DEFAULT_MAX_NUMBER_OF_ARCS; i++)
    arcs_.erase (arcs_.begin ());

  pmouse_s = mouse_s;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_288::addArcs (int x, int y, int xPrev, int yPrev)
{
  olc::vf2d v0  (static_cast<float> (xPrev), static_cast<float> (yPrev));
  olc::vf2d v1 (static_cast<float> (x), static_cast<float> (y));
  int d = static_cast<int> (v0.dist (v1));
  if (d != 0)
  {
    for (int i = 0; i <= d; i++)
    {
      olc::vf2d v2 = v0.lerp (v1, i / static_cast<double> (d));
      arcs_.push_back (arc (v2.x, v2.y, l_ / 2.0f, a_, a_ + rad_, h_));

      setAll ();
    } // end FOR
  } // end IF
  else
  {
    arcs_.push_back (arc (v1.x, v1.y, l_ / 2.0f, a_, a_ + rad_, h_));

    setAll ();
  } // end ELSE
}
