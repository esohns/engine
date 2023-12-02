#include "stdafx.h"

#include "pge_30.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_30::PGE_30 ()
 : olc::PixelGameEngine ()
 , red_ ()
 , green_ ()
 , blue_ ()
 , angle_ (0.0f)
{
  sAppName = "Example 30";
}

bool
PGE_30::OnUserCreate ()
{
  for (int n = 0; n < ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS; n++)
  {
    float hue_f =
      static_cast<float> (std::sqrt (n / static_cast<double> (ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS)));
    hue_f = Common_GL_Tools::map (hue_f, 0.0f, 1.0f, 0.0f, 360.0f);
    float r, g, b;
    Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f,
                                  r, g, b);
    r = Common_GL_Tools::map (r, 0.0f, 1.0f, 0.0f, 255.0f);
    g = Common_GL_Tools::map (g, 0.0f, 1.0f, 0.0f, 255.0f);
    b = Common_GL_Tools::map (b, 0.0f, 1.0f, 0.0f, 255.0f);
    olc::Pixel color (static_cast<uint8_t> (r), static_cast<uint8_t> (g), static_cast<uint8_t> (b), 255);
    red_[n] = color.r;
    green_[n] = color.g;
    blue_[n] = color.b;
  } // end FOR

  return true;
}

bool
PGE_30::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  float ca =
    Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseX ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), -1.0f, 1.0f); //-0.70176;
  float cb =
    Common_GL_Tools::map (static_cast<float> (olc::PixelGameEngine::GetMouseY ()), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1), -1.0f, 1.0f); //-0.3842;
  //float ca = std::cosf (angle_ * 3.213f); //sin(angle);
  //float cb = std::sinf (angle_);

  angle_ += 0.02f;

  float w = 5.0f;
  float h =
    (w * olc::PixelGameEngine::ScreenHeight ()) / static_cast<float> (olc::PixelGameEngine::ScreenWidth ());

  // Start at negative half the width and height
  float xmin = -w / 2.0f;
  float ymin = -h / 2.0f;

  // x goes from xmin to xmax
  float xmax = xmin + w;
  // y goes from ymin to ymax
  float ymax = ymin + h;

  // Calculate amount we increment x,y for each pixel
  float dx = (xmax - xmin) / static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
  float dy = (ymax - ymin) / static_cast<float> (olc::PixelGameEngine::ScreenHeight ());

  float y = ymin;
  for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); j++)
  {
    float x = xmin;
    for (int i = 0; i < olc::PixelGameEngine::ScreenHeight (); i++)
    {
      // iterate z = z^2 + cm does z tend towards infinity?
      float a = x;
      float b = y;
      int n = 0;
      while (n < ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS)
      {
        float aa = a * a;
        float bb = b * b;
        if (aa + bb > 4.0f) // "infinity"
          break;
        float twoab = 2.0f * a * b;
        a = aa - bb + ca;
        b = twoab + cb;
        n++;
      } // end WHILE

      if (n == ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS)
        olc::PixelGameEngine::Draw (i, j, olc::BLACK);
      else
        olc::PixelGameEngine::Draw (i, j, olc::Pixel (red_[n], green_[n], blue_[n], 255));
      x += dx;
    } // end FOR
    y += dy;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
