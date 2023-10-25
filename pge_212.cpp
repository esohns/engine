#include "stdafx.h"

#include "pge_212.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_212::PGE_212 ()
 : olc::PixelGameEngine ()
 , position1_ ()
 , position2_ ()
 , position3_ ()
 , position4_ ()
 , r_ (0.0f)
 , inc_ (0.0f)
 , a_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 212");
}

bool
PGE_212::OnUserCreate ()
{
  position1_.x = static_cast<float> (-(olc::PixelGameEngine::ScreenHeight () / 2) + ENGINE_PGE_212_DEFAULT_FRAME);
  position1_.y = static_cast<float> (-(olc::PixelGameEngine::ScreenHeight () / 2) + ENGINE_PGE_212_DEFAULT_FRAME);
  position2_.x = static_cast<float> ((olc::PixelGameEngine::ScreenHeight () / 2) - ENGINE_PGE_212_DEFAULT_FRAME);
  position2_.y = position1_.y;
  position3_.x = position2_.x;
  position3_.y = static_cast<float> ((olc::PixelGameEngine::ScreenHeight () / 2) - ENGINE_PGE_212_DEFAULT_FRAME);
  position4_.x = position1_.x;
  position4_.y = position3_.y;

  r_ = ((position2_.x - position1_.x) - ENGINE_PGE_212_DEFAULT_D) / (position2_.x - position1_.x);
  inc_ = std::atan (ENGINE_PGE_212_DEFAULT_D / (position2_.x - position1_.x));

  return true;
}

bool
PGE_212::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    a_ = 0.0f;
    OnUserCreate ();
    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  if (std::abs (position2_.x - position1_.x) > 2.0f)
  {
    float rx1, rx2, rx3, rx4, ry1, ry2, ry3, ry4;
    rx1 = position1_.x * std::cos (a_) - position1_.y * std::sin (a_) + static_cast<float> (half_width_i);
    ry1 = position1_.x * std::sin (a_) + position1_.y * std::cos (a_) + static_cast<float> (half_height_i);
    rx2 = position2_.x * std::cos (a_) - position2_.y * std::sin (a_) + static_cast<float> (half_width_i);
    ry2 = position2_.x * std::sin (a_) + position2_.y * std::cos (a_) + static_cast<float> (half_height_i);
    rx3 = position3_.x * std::cos (a_) - position3_.y * std::sin (a_) + static_cast<float> (half_width_i);
    ry3 = position3_.x * std::sin (a_) + position3_.y * std::cos (a_) + static_cast<float> (half_height_i);
    rx4 = position4_.x * std::cos (a_) - position4_.y * std::sin (a_) + static_cast<float> (half_width_i);
    ry4 = position4_.x * std::sin (a_) + position4_.y * std::cos (a_) + static_cast<float> (half_height_i);

    float hue_f = std::fmod (a_ * 65.0f, 255.0f) / 255.0f * 360.0f;
    float r, g, b;
    Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
    olc::Pixel color_s;
    color_s.r = static_cast<uint8_t> (r * 255.0f);
    color_s.g = static_cast<uint8_t> (g * 255.0f);
    color_s.b = static_cast<uint8_t> (b * 255.0f);

    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (rx1), static_cast<int32_t> (ry1),
                                    static_cast<int32_t> (rx2), static_cast<int32_t> (ry2),
                                    color_s, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (rx2), static_cast<int32_t> (ry2),
                                    static_cast<int32_t> (rx3), static_cast<int32_t> (ry3),
                                    color_s, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (rx3), static_cast<int32_t> (ry3),
                                    static_cast<int32_t> (rx4), static_cast<int32_t> (ry4),
                                    color_s, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (rx4), static_cast<int32_t> (ry4),
                                    static_cast<int32_t> (rx1), static_cast<int32_t> (ry1),
                                    color_s, 0xFFFFFFFF);
    a_ = a_ + inc_;

    position1_.x = position1_.x * r_;
    position1_.y = position1_.y * r_;
    position2_.x = position2_.x * r_;
    position2_.y = position2_.y * r_;
    position3_.x = position3_.x * r_;
    position3_.y = position3_.y * r_;
    position4_.x = position4_.x * r_;
    position4_.y = position4_.y * r_;
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
