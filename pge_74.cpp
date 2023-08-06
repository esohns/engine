#include "stdafx.h"

#include "pge_74.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_74::PGE_74 ()
 : olc::PixelGameEngine ()
 , a_ (10.0f)
 , b_ (10.0f)
 //, c_ (20.0f)
 //, d_ (20.0f)
 , t_ (0)
{
  sAppName = "Example 74";
}

bool
PGE_74::OnUserCreate ()
{
  return true;
}

bool
PGE_74::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    a_ = Common_Tools::getRandomNumber (6.0f, 20.0f);
    b_ = Common_Tools::getRandomNumber (6.0f, 20.0f);
    //c_ = Common_Tools::getRandomNumber (6.0f, 20.0f);
    //d_ = Common_Tools::getRandomNumber (6.0f, 20.0f);
  } // end IF

  olc::Pixel color_s = {static_cast<uint8_t> (t_ % 256), static_cast<uint8_t> (200), static_cast<uint8_t> (255 - (t_ % 256)), static_cast<uint8_t> (255)};
  for (int i = 0; i < ENGINE_PGE_74_DEFAULT_NUMBER_OF_LINES; i++)
  {
    olc::PixelGameEngine::DrawLine (x (t_ + i), y (t_ + i), static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_74_DEFAULT_NUMBER_OF_LINES) * (i + 1)), 0, color_s, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x (t_ + i), y (t_ + i), static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_74_DEFAULT_NUMBER_OF_LINES) * i),       olc::PixelGameEngine::ScreenHeight () - 1, color_s, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x (t_ + i), y (t_ + i), 0,                                                                                                                                  static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / static_cast<float> (ENGINE_PGE_74_DEFAULT_NUMBER_OF_LINES) * i), color_s, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x (t_ + i), y (t_ + i), olc::PixelGameEngine::ScreenWidth () - 1,                                                                                           static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / static_cast<float> (ENGINE_PGE_74_DEFAULT_NUMBER_OF_LINES) * (i + 1)), color_s, 0xFFFFFFFF);
  } // end FOR
  t_++;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

int32_t
PGE_74::x (int32_t t_in)
{
  return static_cast<int32_t> ((std::cos (t_in / a_) * 200.0f) + (olc::PixelGameEngine::ScreenWidth () / 2.0f));
}

int32_t
PGE_74::y (int32_t t_in)
{
  return static_cast<int32_t> ((std::sin (t_in / b_) * 200.0f) + (olc::PixelGameEngine::ScreenHeight () / 2.0f));
}

//int32_t
//PGE_74::x2 (int32_t t_in)
//{
//  return static_cast<int32_t> (std::cos (t_in / c_) * 400.0f);
//}

//int32_t
//PGE_74::y2 (int32_t t_in)
//{
//  return static_cast<int32_t> (std::sin (t_in / d_) * 400.0f);
//}
