#include "stdafx.h"

#include "pge_66.h"

#include <algorithm>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_66::PGE_66 ()
 : olc::PixelGameEngine ()
 , points_ ()
{
  sAppName = "Example 66";
}

bool
PGE_66::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_POINTS; ++i)
    points_.push_back (glm::vec3 (static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1)),
                                  static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1)),
                                  static_cast<float> (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1))));

  return true;
}

bool
PGE_66::OnUserUpdate (float fElapsedTime)
{
  olc::Pixel* p = GetDrawTarget ()->GetData ();

  static int frame_count_i = 1;
  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    {
      std::vector<float> distances_a;
      for (int i = 0; i < static_cast<int> (points_.size ()); i++)
      {
        glm::vec3 v (x, y, static_cast<float> (frame_count_i % olc::PixelGameEngine::ScreenWidth ()));
        distances_a.push_back (glm::distance (v, points_[i]));
      } // end FOR
      std::sort (distances_a.begin (), distances_a.end ());

      int index = (x + y * olc::PixelGameEngine::ScreenWidth ());
      p[index].r =
        static_cast<uint8_t> (Common_GL_Tools::map (distances_a[0], 0.0f, 150.0f, 0.0f, 255.0f));
      p[index].g =
        static_cast<uint8_t> (Common_GL_Tools::map (distances_a[1], 0.0f, 50.0f, 255.0f, 0.0f));
      p[index].b =
        static_cast<uint8_t> (Common_GL_Tools::map (distances_a[2], 0.0f, 200.0f, 255.0f, 0.0f));
      p[index].a = 255;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
