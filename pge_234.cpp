#include "stdafx.h"

#include "pge_234.h"

#include <vector>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_10.h"

PGE_234::PGE_234 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 234");
}

bool
PGE_234::OnUserCreate ()
{
  return true;
}

bool
PGE_234::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int num = 0; num < 16; num++)
  {
    //Parameter
    std::vector<float> a;
    std::vector<float> f;
    std::vector<float> p;
    std::vector<float> d;

    for (int i = 0; i < 4; i++)
    {
      a.push_back (10.0f);
      f.push_back (Common_Tools::getRandomNumber (-1.0f, 1.0f) * 4.0f);
      p.push_back (Common_Tools::getRandomNumber (0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI));
      d.push_back (0.0f);
    } // end FOR

    for (int i = 0; i < ENGINE_PGE_234_DEFAULT_N; i++)
    {
      float t = i * 0.002f;
      std::vector<float> s;

      for (int j = 0; j < 4; j++)
        s.push_back (harmonograph (a[j], f[j], p[j], d[j], t));

      olc::vf2d position (s[0] + s[1], s[2] + s[3]);
      static olc::vf2d zero_s (0.0f, 0.0f);
      float r = zero_s.dist (position);
      float draw_scale = 0.3f * olc::PixelGameEngine::ScreenHeight () / (1.0f + r);
      olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (draw_scale * position.x),
                                        half_height_i + static_cast<int32_t> (draw_scale * position.y),
                                        static_cast<int32_t> (std::sin (t)),
                                        olc::WHITE,
                                        0xFF);
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_234::harmonograph (float a, float f, float p, float d, float t)
{
  return a * std::sin (t * f + p) * std::exp (-d * t);
}
