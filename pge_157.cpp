#include "stdafx.h"

#include "pge_157.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_157::PGE_157 ()
 : olc::PixelGameEngine ()
 , flowers_ ()
{
  sAppName = "Example 157";
}

bool
PGE_157::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = ENGINE_PGE_157_DEFAULT_NUMBER_OF_FLOWERS; i--;)
    flowers_.push_back ({Common_Tools::getRandomNumber (static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) * 0.1f, static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) * 0.9f),
                         Common_Tools::getRandomNumber (static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) * 0.1f, static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) * 0.9f)});

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_157::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    flowers_.clear ();
    OnUserCreate ();
  } // end IF

  float sec = frame_count_i / 30.0f;
  olc::Pixel color;
  float r, g, b;
  Common_Image_Tools::HSVToRGB (Common_GL_Tools::map (std::sin (sec), -1.0f, 1.0f, 0.0f, 360.0f), 1.0f, 1.0f, r, g, b);
  color.r = static_cast<uint8_t> (r * 255.0f);
  color.g = static_cast<uint8_t> (g * 255.0f);
  color.b = static_cast<uint8_t> (b * 255.0f);
  color.a = 20;

  static int32_t prev_x, prev_y;
  bool is_first_b;
  for (std::vector<olc::vf2d>::iterator iterator = flowers_.begin ();
       iterator != flowers_.end ();
       ++iterator)
  {
    float x = (*iterator).x;
    float y = (*iterator).y;

    is_first_b = true;
    for (int i = 200; i--;)
    {
      float degree = sec;
      for (std::vector<olc::vf2d>::iterator iterator_2 = flowers_.begin ();
           iterator_2 != flowers_.end ();
           ++iterator_2)
        degree += std::atan2 ((*iterator_2).y - y, (*iterator_2).x - x) * 2.0f;

      if (is_first_b)
      {
        is_first_b = false;
        prev_x = static_cast<int32_t> (x);
        prev_y = static_cast<int32_t> (y);
      } // end IF
      x += std::cos (degree) * ENGINE_PGE_157_DEFAULT_STEP;
      y += std::sin (degree) * ENGINE_PGE_157_DEFAULT_STEP;
      olc::PixelGameEngine::DrawLine (prev_x, prev_y, static_cast<int32_t> (x), static_cast<int32_t> (y),
                                      color, 0xFFFFFFFF);

      bool condition_b = false;
      for (std::vector<olc::vf2d>::iterator iterator_2 = flowers_.begin ();
           iterator_2 != flowers_.end ();
           ++iterator_2)
        if ((((*iterator_2).x - x) * ((*iterator_2).x - x) +
             ((*iterator_2).y - y) * ((*iterator_2).y - y)) < ENGINE_PGE_157_DEFAULT_STEP)
        {
          condition_b = true;
          break;
        } // end IF
      if (condition_b)
        break;

      prev_x = static_cast<int32_t> (x);
      prev_y = static_cast<int32_t> (y);
    } // end FOR
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
