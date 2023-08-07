#include "stdafx.h"

#include "pge_78.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_78::PGE_78 ()
 : olc::PixelGameEngine ()
 , angle_ (0.0f)
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
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_77_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_77_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    //t_ = 0.0f;
    v1_ = Common_Tools::getRandomNumber (0.0f, 0.4f) + 0.2f;
  } // end IF

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static olc::Pixel color_s = {255, 255, 255, 200};

  for (int i = 1; i < ENGINE_PGE_78_DEFAULT_NUMBER_OF_LINES; i++)
  {
    olc::vi2d pos_1 = {static_cast<int32_t> (x (t_ + i)), static_cast<int32_t> (y (t_ + i))};
    olc::vi2d pos_1_rot;
    pos_1_rot.x = static_cast<int32_t> (std::cos (angle_) * pos_1.x - std::sin (angle_) * pos_1.y);
    pos_1_rot.y = static_cast<int32_t> (std::sin (angle_) * pos_1.x + std::cos (angle_) * pos_1.y);
    olc::PixelGameEngine::FillCircle (pos_1_rot.x + half_width_i, pos_1_rot.y + half_height_i, 2, color_s);
    olc::vi2d pos_2 = {static_cast<int32_t> (x2 (t_ + i)), static_cast<int32_t> (y2 (t_ + i))};
    olc::vi2d pos_2_rot;
    pos_2_rot.x = static_cast<int32_t> (std::cos (angle_) * pos_2.x - std::sin (angle_) * pos_2.y);
    pos_2_rot.y = static_cast<int32_t> (std::sin (angle_) * pos_2.x + std::cos (angle_) * pos_2.y);
    olc::PixelGameEngine::FillCircle (pos_2_rot.x + half_width_i, pos_2_rot.y + half_height_i, 2, color_s);
    olc::PixelGameEngine::DrawLine (pos_1_rot.x + half_width_i, pos_1_rot.y + half_height_i,
                                    pos_2_rot.x + half_width_i, pos_2_rot.y + half_height_i,
                                    color_s, 0xFFFFFFFF);
  } // end FOR

  angle_ += 0.001f;
  t_ += 0.002f;

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
  return std::sin (t_in / 10.0f) * 10.0f  + std::cos (t_in / v1_) * 100.0f;
}

float
PGE_78::y2 (float t_in)
{
  return std::cos (t_in / 10.0f) * 10.0f  + std::sin (t_in / v1_) * 100.0f;
}
