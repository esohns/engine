#include "stdafx.h"

#include "pge_90.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "engine_common.h"

PGE_90::PGE_90 ()
 : olc::PixelGameEngine ()
 , field_ ()
 , bg_color_ (olc::BLACK)
 , color_ (olc::WHITE)
{
  sAppName = "Example 90";
}

bool
PGE_90::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  field_.initialize_points (ENGINE_PGE_90_DEFAULT_NUMBER_OF_PARTICLES);

  float hue_f = Common_Tools::getRandomNumber (0.0f, 1.0f);
  float f_max = 0.1f;
  float f =
    ENGINE_PGE_90_DEFAULT_COLORS ? f_max * (Common_Tools::getRandomNumber (0.0f, 1.0f) + 1.0f) / 2.0f
                                 : 1.0f - f_max * (Common_Tools::getRandomNumber (0.0f, 1.0f) + 1.0f) / 2.0f;
  float r, g, b;
  Common_Image_Tools::HSVToRGB (hue_f * 360.0f, 1.0f - f, f, r, g, b);
  bg_color_.r = static_cast<uint8_t> (r * 255.0f);
  bg_color_.g = static_cast<uint8_t> (g * 255.0f);
  bg_color_.b = static_cast<uint8_t> (b * 255.0f);

  Common_Image_Tools::HSVToRGB (std::fmod (hue_f + f, 1.0f) * 360.0f, 1.0f - f, 1.0f - f, r, g, b);
  color_.r = static_cast<uint8_t> (r * 255.0f);
  color_.g = static_cast<uint8_t> (g * 255.0f);
  color_.b = static_cast<uint8_t> (b * 255.0f);
  color_.a = static_cast<uint8_t> (ENGINE_PGE_90_DEFAULT_ALPHA * 255.0f);

  olc::PixelGameEngine::Clear (bg_color_);

  return true;
}

bool
PGE_90::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    field_.initialize_points (ENGINE_PGE_90_DEFAULT_NUMBER_OF_PARTICLES);
    field_.reset ();

    olc::PixelGameEngine::Clear (bg_color_);
  } // end IF

  field_.update (this, color_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
