#include "stdafx.h"

#include "pge_78.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_78::PGE_78 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
 , v1_ (0.4f)
{
  sAppName = "Example 78";
}

bool
PGE_78::OnUserCreate ()
{
  v1_ = Common_Tools::getRandomNumber (0.0f, 0.4f) + 0.2f;

  return true;
}

bool
PGE_78::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    //t_ = 0.0f;
    v1_ = Common_Tools::getRandomNumber (0.0f, 0.4f) + 0.2f;
  } // end IF

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  for (int i = 0; i < ENGINE_PGE_78_DEFAULT_NUMBER_OF_LINES; i++)
  {
    olc::PixelGameEngine::FillCircle (half_width_i + static_cast<int32_t> (x (t_ + i)), half_height_i + static_cast<int32_t> (y (t_ + i)), 2, olc::WHITE);
    olc::PixelGameEngine::FillCircle (half_width_i + static_cast<int32_t> (x2 (t_ + i)), half_height_i + static_cast<int32_t> (y2 (t_ + i)), 2, olc::WHITE);
    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (x (t_ + i)), half_height_i + static_cast<int32_t> (y (t_ + i)),
                                    half_width_i + static_cast<int32_t> (x2 (t_ + i)), half_height_i + static_cast<int32_t> (y2 (t_ + i)),
                                    olc::WHITE, 0xFFFFFFFF);
  } // end FOR

  t_ += 0.003f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_78::x (float t_in)
{
  return std::sin (t_in / 10.0f) * 100.0f + std::cos (t_in / v1_) * 100.0f;
}

float
PGE_78::y (float t_in)
{
  return std::cos (t_in / 10.0f) * 100.0f + std::sin (t_in / v1_) * 100.0f;
}

float
PGE_78::x2 (float t_in)
{
  return std::sin (t_in / 10.0f) * 10.0f + std::cos (t_in / v1_) * 100.0f;
}

float
PGE_78::y2 (float t_in)
{
  return std::cos (t_in / 10.0f) * 10.0f + std::sin (t_in / v1_) * 100.0f;
}
