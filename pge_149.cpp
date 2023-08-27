#include "stdafx.h"

#include "pge_149.h"

#include "ace/Log_Msg.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_149::PGE_149 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
{
  sAppName = "Example 149";
}

bool
PGE_149::OnUserCreate ()
{
  //olc::PixelGameEngine::SetPixelBlend (0.8f);
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_149::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  //for (int i = 500; i--;)
  //  for (int j = 300; j--;)
  //    FillRect (i, j, (int32_t)(pow(sin(j * j + i / (float)j - t_ * 7) + cos(pow(j, 5) - i / (float)j * 6 + t_), 3) * j / (float)500), 1, olc::BLACK);
  for (int32_t i = 0; i < 500; i++)
    for (int32_t j = 1; j < 300; j++)
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (i), static_cast<int32_t> (j),
                                      /*1 + */static_cast<int32_t> (std::pow (std::sin ((j * j) + (i / static_cast<float> (j)) - t_ * 7.0f) +
                                                                              std::cos (std::pow (j, 5) - (i / static_cast<float> (j)) * 6.0f + t_), 3) * 3.0f * j / 500.0f),
                                      1, //static_cast<int32_t> (j / 300.0f * 10.0f),
                                      {0, 0, 0, /*255});*/static_cast<uint8_t> (j / 300.0f * 255.0f)});

  t_ += 0.01f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
