#include "stdafx.h"

#include "pge_173.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_173::PGE_173 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 173");
}

bool
PGE_173::OnUserCreate ()
{
  return true;
}

bool
PGE_173::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();

  float f = static_cast<float> (frame_count_i);
  float l =
    //(std::sin (static_cast<float> (M_PI) / 180.0f * (f + Common_GL_Tools::map (static_cast<float> (noise_.GetValue (Common_Tools::getRandomNumber (0.8, 1.2), 0.0, 0.0)),
    (std::sin (static_cast<float> (M_PI) / 180.0f * (f + Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (Common_Tools::getRandomNumber (0.8, 1.2), 0.0, 0.0)),
                                                                               -1.0f, 1.0f, 0.0f, 1.0f))) + 1.0f) / 2.0f;
  int ind = 0;
  for (int i = 0; i < half_height_i; i++)
  {
    for (int j = 0; j < half_width_i; j++)
    {
      int x = j - olc::PixelGameEngine::ScreenWidth () / 4;
      int y = i - olc::PixelGameEngine::ScreenHeight () / 4;
      float k = std::sin (static_cast<float> (M_PI) / 180.0f * (f + x * l + y * (1.0f - l)));
      float n =
        //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (static_cast<float> (M_PI) / 180.0f * (x + f), static_cast<float> (M_PI) / 180.0f * (y - f), static_cast<float> (M_PI) / 180.0f * x * static_cast<float> (M_PI) / 180.0f * y * k)),
        Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<float> (M_PI) / 180.0f * (x + f), static_cast<float> (M_PI) / 180.0f * (y - f), static_cast<float> (M_PI) / 180.0f * x * static_cast<float> (M_PI) / 180.0f * y * k)),
                              -1.0f, 1.0f, 0.0f, 1.0f);
      uint8_t col = uint8_t(Common_GL_Tools::map (n, 0.0f, 1.0f, 0.0f, 255.0f));
      olc::Pixel color;
      color.r = col; color.g = col; color.b = col;
      p[ind] = color;
      p[ind + olc::PixelGameEngine::ScreenWidth () + 1] = color;
      ind += 2;
    } // end FOR
    ind += olc::PixelGameEngine::ScreenWidth ();
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
