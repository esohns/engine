#include "stdafx.h"

#include "pge_68.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_68::PGE_68 ()
 : olc::PixelGameEngine ()
 , w_ (0.0)
{
  sAppName = "Example 68";
}

bool
PGE_68::OnUserCreate ()
{
  w_ = olc::PixelGameEngine::ScreenWidth () / static_cast<double> (ENGINE_PGE_68_DEFAULT_C);

  return true;
}

bool
PGE_68::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::WHITE);

  static double w_2 = w_ * 0.9;

  for (int i = 0; i < ENGINE_PGE_68_DEFAULT_C; i++)
    for (int j = 0; j < ENGINE_PGE_68_DEFAULT_C; j++)
    {
      int32_t x = static_cast<int32_t> (i * w_ + w_ / 2.0);
      int32_t y = static_cast<int32_t> (j * w_ + w_ / 2.0);

      if ((i + j) % 2 == 0)
      {
        double a = std::sin (frame_count_i * 0.005f + (y * 0.01) + (x * 0.001)) * 10.0;
        double r = std::pow (((std::cos (((frame_count_i / 60.0) * M_PI * 1.25) + (M_PI / 2.0) + ((x + y) * 0.002)) + 1.0) / 2.0), 3) * w_2 * 0.25;
        double offx = r * std::cos (a);
        double offy = r * std::sin (a);
        
        //Common_GL_Color_t col1, col2;
        //while (col1 == col2)
        //{
        //  col1 = Common_GL_Tools::randomColor ();
        //  col2 = Common_GL_Tools::randomColor ();
        //} // end WHILE
        //color1.r = col1.r;
        //color1.g = col1.g;
        //color1.b = col1.b;
        //color2.r = col2.r;
        //color2.g = col2.g;
        //color2.b = col2.b;
        olc::Pixel color1, color2;
        static olc::Pixel colors_a[9] = { {0xDE, 0x18, 0x3C, 0xFF},
                                          {0xF2, 0xB5, 0x41, 0xFF},
                                          {0x0C, 0x79, 0xBB, 0xFF},
                                          {0xEC, 0x4E, 0x20, 0xFF},
                                          {0x06, 0x7B, 0xC2, 0xFF},
                                          {0x00, 0x91, 0x6E, 0xFF},
                                          {0xFF, 0x9F, 0x1C, 0xFF},
                                          {0xA4, 0x45, 0x9F, 0xFF},
                                          {0xF6, 0x54, 0xA9, 0xFF} };
        int index1 = 0, index2 = 0;
        while (index1 == index2)
        {
          index1 = Common_Tools::getRandomNumber (0, 8);
          index2 = Common_Tools::getRandomNumber (0, 8);
        } // end WHILE
        color1 = colors_a[index1];
        color2 = colors_a[index2];

        olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x + offx), static_cast<int32_t> (y + offy), static_cast<int32_t> (w_2 * 0.4), color1);
        olc::PixelGameEngine::FillCircle (x, y, static_cast<int32_t> (w_2 * 0.4), color2);
        olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x + offx), static_cast<int32_t> (y + offy), static_cast<int32_t> (w_2 * 0.4), olc::BLACK);
      } // end IF
    } // end FOR

  frame_count_i++;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
