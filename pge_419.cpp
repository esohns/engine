#include "stdafx.h"

#include "pge_419.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_19.h"

PGE_419::PGE_419 ()
 : olc::PixelGameEngine ()
 , C_ (0.0f)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 419");
}

bool
PGE_419::OnUserCreate ()
{
  return true;
}

bool
PGE_419::OnUserUpdate (float fElapsedTime)
{
  static olc::Pixel bg_color (80, 160, 220);
  olc::PixelGameEngine::Clear (bg_color);

  C_ += 0.002f;

  static int screen_width_i = olc::PixelGameEngine::ScreenWidth ();
  static int screen_height_i = olc::PixelGameEngine::ScreenHeight ();
  for (int j = 0; j < screen_height_i; j += ENGINE_PGE_419_DEFAULT_H)
    for (int i = 0; i < screen_width_i; i += ENGINE_PGE_419_DEFAULT_H)
      D (i, j, ENGINE_PGE_419_DEFAULT_H);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_419::D (int x, int y, int s)
{
  static int screen_width_i = olc::PixelGameEngine::ScreenWidth ();
  static int screen_height_i = olc::PixelGameEngine::ScreenHeight ();

  olc::vi2d a_b (x + s / 2, y + s / 2);
  float n =
    //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (a_b.x / static_cast<double> (screen_height_i),
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (a_b.x / static_cast<double> (screen_height_i),
                                                               a_b.y / static_cast<double> (screen_height_i),
                                                               a_b.mag () / static_cast<double> (screen_height_i) - C_)),
                                  -1.0f, 1.0f, 0.0f, 1.0f);
  if (std::abs (n - 0.5f) > Common_GL_Tools::map (static_cast<float> (s), 4.0f, static_cast<float> (ENGINE_PGE_419_DEFAULT_H), 0.05f, 0.18f))
  {
    static olc::Pixel fg_color (60, 140, 60);
    olc::PixelGameEngine::FillRect (x, y, s, s, fg_color);
    olc::PixelGameEngine::DrawRect (x, y, s, s, olc::BLACK);
  } // end IF
  else
  {
    s /= 2;
    if (s >= 4)
    {
      D (x,     y,     s);
      D (x + s, y,     s);
      D (x,     y + s, s);
      D (x + s, y + s, s);
    } // end IF
  } // end ELSE
}
