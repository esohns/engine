#include "stdafx.h"

#include "pge_179.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_179::PGE_179 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 179");
}

bool
PGE_179::OnUserCreate ()
{
  return true;
}

bool
PGE_179::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);

  float mx = static_cast<float> (olc::PixelGameEngine::GetMouseX () - ENGINE_PGE_179_DEFAULT_W / 2);
  float my = static_cast<float> (olc::PixelGameEngine::GetMouseY () - ENGINE_PGE_179_DEFAULT_W / 2);
  float r, g, b;
  olc::Pixel color;
  for (int i = 3000; i--;)
  {
    float X = Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_179_DEFAULT_W - 1));
    float Y = Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_179_DEFAULT_W - 1));
    float x = Common_GL_Tools::map (X - mx, 0.0f, static_cast<float> (ENGINE_PGE_179_DEFAULT_W - 1), -2.0f, 2.0f);
    float y = Common_GL_Tools::map (Y - my, 0.0f, static_cast<float> (ENGINE_PGE_179_DEFAULT_W - 1), -2.0f, 2.0f);
    float D = std::sin (x * x + y * y) - std::sin (2.0f * std::sin (x) * y + t_);
    Common_Image_Tools::HSVToRGB (std::fmod (D, 1.0f) * 360.0f, 1.0f, D > -0.3f ? 1.0f : 0.0f, r, g, b);
    color.r = static_cast<uint8_t> (r * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (X), static_cast<int32_t> (Y),
                                    ENGINE_PGE_179_DEFAULT_U, ENGINE_PGE_179_DEFAULT_U,
                                    color);
  } // end FOR

  t_ += 0.05f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
