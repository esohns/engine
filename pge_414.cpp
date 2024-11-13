#include "stdafx.h"

#include "pge_414.h"

#if defined (IMAGEMAGICK_SUPPORT)
#if defined (IMAGEMAGICK_IS_GRAPHICSMAGICK)
#include "wand/wand_api.h"
#else
#include "MagickWand/MagickWand.h"
#endif // IMAGEMAGICK_IS_GRAPHICSMAGICK
#endif // IMAGEMAGICK_SUPPORT

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "engine_common.h"

PGE_414::PGE_414 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = "Example 414";
}

bool
PGE_414::OnUserCreate ()
{
  Common_Image_Resolution_t resolution_s;
  uint8_t* data_p = NULL;
  if (!Common_Image_Tools::load (ACE_TEXT_ALWAYS_CHAR ("Apple_Computer_Logo_rainbow.png"),
                                 ACE_TEXT_ALWAYS_CHAR ("RGBA"),
                                 resolution_s,
                                 data_p))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to load image (was: \"%s\"), aborting\n"),
                ACE_TEXT ("Apple_Computer_Logo_rainbow.png")));
    return false;
  } // end IF
  ACE_ASSERT (data_p);

  int pixelIndex = 0;
  for (int y = 0; y < resolution_s.cy; y++)
    for (int x = 0; x < resolution_s.cx; x++)
    {
      uint8_t pixelR = data_p[pixelIndex];
      uint8_t pixelG = data_p[pixelIndex + 1];
      uint8_t pixelB = data_p[pixelIndex + 2];
      uint8_t pixelA = data_p[pixelIndex + 3];

      pixelIndex += 4;

      // assign a particle to this pixel ?
      if (!Common_Tools::testRandomProbability (ENGINE_PGE_414_DEFAULT_LOAD_PERCENTAGE))
        continue; // no --> proceed

      static int screen_width_i = olc::PixelGameEngine::ScreenWidth ();
      static int screen_height_i = olc::PixelGameEngine::ScreenHeight ();
      static olc::vf2d center_s (screen_width_i / 2.0f, screen_height_i / 2.0f);

      olc::Pixel pixelColor (pixelR, pixelG, pixelB, 255);

      particle particle_s (center_s.x, center_s.y);
      particle_s.target.x = center_s.x - (resolution_s.cx / 2) + x;
      particle_s.target.y = center_s.y - (resolution_s.cy / 2) + y;
      particle_s.targetColor = pixelColor;

      particles_.push_back (particle_s);
    } // end FOR

  MagickRelinquishMemory (data_p); data_p = NULL;

  return true;
}

bool
PGE_414::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_414_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_414_DEFAULT_ALPHA_DECAY : 0);

  olc::vf2d mouse_position_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                              static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  bool left_mouse_button_pressed_b = olc::PixelGameEngine::GetMouse (0).bHeld;
  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       )
  {
    (*iterator).update (mouse_position_s,
                        left_mouse_button_pressed_b);
    (*iterator).draw (this);

    if (unlikely ((*iterator).isDead && (*iterator).isOutOfBounds (this)))
    {
      iterator = particles_.erase (iterator);
      continue;
    } // end IF

    ++iterator;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
