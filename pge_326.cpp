#include "stdafx.h"

#include "pge_326.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_326::PGE_326 ()
 : olc::PixelGameEngine ()
 , turtle_ (ENGINE_PGE_326_DEFAULT_WIDTH / 2.0f, ENGINE_PGE_326_DEFAULT_HEIGHT / 2.0f)
 , hue_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 326");
}

bool
PGE_326::OnUserCreate ()
{
  hue_ = Common_Tools::getRandomNumber (0.0f, 360.0f);

  return true;
}

bool
PGE_326::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  turtle_.penUp ();
  turtle_.moveTo (450.0f, 125.0f, this, olc::BLACK);
  turtle_.turnTo (0.0f);
  turtle_.penDown ();

  static float rotateAng = (360.0f / static_cast<float> (ENGINE_PGE_326_DEFAULT_SIDES)) * 2.0f;
  float r, g, b;
  for (int side = 0; side < ENGINE_PGE_326_DEFAULT_SIDES; side++)
  {
    for (int step = 0; step < ENGINE_PGE_326_DEFAULT_STEPS; step++)
    {
      turtle_.turnRight (1.0f * static_cast<float> (M_PI) / 180.0f);
      Common_Image_Tools::HSVToRGB (std::fmod (hue_ + (step / 100.0f) * 360.0f, 360.0f), 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
      turtle_.moveForward (3.5f, this, color);
    } // end FOR

    for (int step = 0; step < ENGINE_PGE_326_DEFAULT_STEPS; step++)
    {
      turtle_.turnLeft (1.0f * static_cast<float> (M_PI) / 180.0f);
      Common_Image_Tools::HSVToRGB (std::fmod (hue_ + (step / 100.0f) * 360.0f, 360.0f), 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
      turtle_.moveForward (3.5f, this, color);
    } // end FOR

    turtle_.turnRight (rotateAng * static_cast<float> (M_PI) / 180.0f);
    hue_ += 1.0f;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

