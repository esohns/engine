#include "stdafx.h"

#include "pge_292.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_292::PGE_292 ()
 : olc::PixelGameEngine ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 292");
}

bool
PGE_292::OnUserCreate ()
{
  return true;
}

bool
PGE_292::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static float elapsed_time_f = 0.0f;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x += 15)
    segment (x, true, elapsed_time_f);
  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth(); x += 15)
    segment (x, false, elapsed_time_f);

  ++frame_count_i;
  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_292::segment (int32_t x, bool positive, float fElapsedTime)
{
  float yOffset =
    smoothWave (fElapsedTime + x, 5.0f, 50.0f + (olc::PixelGameEngine::GetMouseY () / -6.0f));
  if (positive)
    yOffset = -yOffset;

  olc::Pixel color (static_cast<uint8_t> ((x * 0.8f) / static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) * 255.0f),
                    255 - static_cast<uint8_t> ((x / static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) * 255.0f),
                    255,
                    255);

  olc::PixelGameEngine::FillCircle (x, static_cast<int32_t> (yOffset + olc::PixelGameEngine::ScreenHeight () / 2.0f), 10, color);
}

float
PGE_292::smoothWave (float time, float speed, float amp)
{
  return std::sin (time * speed) * amp;
}
