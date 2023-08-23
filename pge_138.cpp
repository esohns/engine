#include "stdafx.h"

#include "pge_138.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_138::PGE_138 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , maxx_ (0.0f)
 , maxy_ (0.0f)
 , y_ (0)
{
  sAppName = "Example 138";
}

bool
PGE_138::OnUserCreate ()
{
  float average =
    static_cast<float> (std::sqrt (olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ()));
  maxx_ =
    (average / static_cast<float> (olc::PixelGameEngine::ScreenHeight ())) * 2.0f;
  maxy_ =
    (average / static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) * 2.0f;

  return true;
}

bool
PGE_138::OnUserUpdate (float fElapsedTime)
{
  if (y_ < olc::PixelGameEngine::ScreenHeight ())
  {
    for (int32_t x = olc::PixelGameEngine::ScreenWidth (); x--; )
    {
      olc::Pixel color = getColor (static_cast<float> (x), static_cast<float> (y_));
      olc::PixelGameEngine::DrawLine (x, y_, x, y_ + 100, color, 0xFFFFFFFF);
    } // end FOR
    y_++;
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::Pixel
PGE_138::getColor (float x, float y)
{
  olc::Pixel result;

  x = Common_GL_Tools::map (x, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), -maxx_, maxx_);
  y = Common_GL_Tools::map (y, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), -maxy_, maxy_);
  float newx =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (x), static_cast<double> (y), 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * 24.0f;
  float newy =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (y), static_cast<double> (x), 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * 24.0f;
  x = newx;
  y = newy;
  result.r = static_cast<uint8_t> (std::sin (x - y) * std::sin (x - y) * 255.0f);

  newx =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (x), static_cast<double> (y), 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * 24.0f;
  newy =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (y), static_cast<double> (x), 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * 24.0f;
  x = newx;
  y = newy;
  result.g = static_cast<uint8_t> (std::sin (x - y) * std::sin (x - y) * 255.0f);

  newx =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (x), static_cast<double> (y), 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * 24.0f;
  newy =
    Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (y), static_cast<double> (x), 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f) * 24.0f;
  x = newx;
  y = newy;
  result.b = static_cast<uint8_t> (std::sin (x - y) * std::sin (x - y) * 255.0f);

  return result;
}
