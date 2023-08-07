#include "stdafx.h"

#include "pge_85.h"

#include "ace/Log_Msg.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_85::PGE_85 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 85";
}

bool
PGE_85::OnUserCreate ()
{
  return true;
}

bool
PGE_85::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2 - 40;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2 - 40;

  std::vector<float> t =
    linspace (0.0f, 2.0f * static_cast<float> (M_PI), ENGINE_PGE_85_DEFAULT_NUMBER_OF_POINTS);
  for (int i = 0; i < ENGINE_PGE_85_DEFAULT_NUMBER_OF_POINTS; i++)
  {
    complex ft = swirly (t[i], 1, 6, -14, 0);
    olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (ft.x_ * 200.0f), half_height_i + static_cast<int32_t> (ft.y_ * 200.0f), 5, olc::WHITE, 0xFF);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<float>
PGE_85::linspace (float min, float max, int points)
{
  std::vector<float> result;
  for (int i = 0; i < points; i++)
    result.push_back (min + i * (max - min) / static_cast<float> (points - 1));
  return result;
}

PGE_85::complex
PGE_85::swirly (float t, float a, float b, float c, float d)
{
  complex j1 = complex (0.0f, 1.0f);

  complex objComplexTemp = j1.times (t).times (a);
  complex r1 = objComplexTemp.exp ();

  objComplexTemp = j1.times (t).times (b);
  complex r2 = objComplexTemp.exp ().divides (2.0f);

  objComplexTemp = j1.times (t).times (c);
  objComplexTemp = j1.mult (objComplexTemp.exp ());
  complex r3 = objComplexTemp.divides (3);

  objComplexTemp = j1.times (t).times (d);
  complex r4 = objComplexTemp.exp ().divides (4.0f);

  complex objComplex = r1.minus (r2).add (r3).add (r4);
  return objComplex;
}
