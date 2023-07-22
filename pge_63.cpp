#include "stdafx.h"

#include "pge_63.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_63::PGE_63 ()
 : olc::PixelGameEngine ()
 , s_ (0.0f)
 , t_ (0.0f)
{
  sAppName = "Example 63";
}

bool
PGE_63::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  s_ = olc::PixelGameEngine::ScreenWidth () / 15.0f;

  return true;
}

bool
PGE_63::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear ({0,0,0, ENGINE_PGE_63_DEFAULT_ALPHA_DECAY});
  int pixels = GetDrawTargetWidth() * GetDrawTargetHeight();
  olc::Pixel* p = GetDrawTarget()->GetData();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_63_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_63_DEFAULT_ALPHA_DECAY : 0);


  static float half_width_f = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  static float half_height_f = olc::PixelGameEngine::ScreenHeight () / 2.0f;

  t_ += 0.01f;

  olc::Pixel color;
  for (int i = 225; i--;)
  {
    olc::vf2d position;
    position.x = (i % 15 - 7.0f) * s_;
    position.y = (~~(i / 15) - 7.0f) * s_;
    float d = position.mag ();
    float a = std::atan2 (position.y, position.x) - static_cast<float> (M_PI / 2.0f) + t_;
    float r = (std::sin (t_) / 2.0f + 0.5f) * std::pow ((d / olc::PixelGameEngine::ScreenWidth ()), 2.0f) * olc::PixelGameEngine::ScreenWidth () * 9.0f;
    olc::vf2d position_2;
    position_2.x = position.x + std::cos (a) * r;
    position_2.y = position.y + std::sin (a) * r;
    r = std::cos (position_2.mag () / 99.0f - t_ * 3.0f) * 30.0f + 30.0f;
    position_2.x += half_width_f;
    position_2.y += half_height_f;

    float r_2, g, b;
    Common_Image_Tools::HSVToRGB (static_cast<float> (static_cast<int> (t_ * 100.0f + i) % 225), 225 / 255.0f, 225 / 255.0f,
                                  r_2, g, b);
    color.r = static_cast<uint8_t> (r_2 * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);
    olc::PixelGameEngine::FillCircle (position_2, static_cast<int32_t> (r / 2.0f), color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
