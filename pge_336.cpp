#include "stdafx.h"

#include "pge_336.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_336::PGE_336 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 336");
}

bool
PGE_336::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int r = 12; r < 200; r += 6)
    for (int i = 0; i < r; i++)
    {
      struct particle particle_s;
      particle_s.r = r;
      particle_s.theta =
        Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (r), 0.0f, 2.0f * static_cast<float> (M_PI));
      particles_.push_back (particle_s);
    } // end FOR

  return true;
}

bool
PGE_336::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static olc::vi2d center_s (olc::PixelGameEngine::ScreenWidth () / 2,
                             olc::PixelGameEngine::ScreenHeight () / 2);

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<struct particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    float t = frame_count_i / 128.0f;

    float disp = Common_GL_Tools::map (static_cast<float> ((*iterator).r), 100.0f, 1092.0f, 5.0f, 0.0f);
    if ((*iterator).r < 100)
      disp = Common_GL_Tools::map (static_cast<float> ((*iterator).r), 12.0f, 100.0f, 2.0f, 1.0f);

    static float aberr = 0.015f;
    olc::Pixel color (255U, 0, 0, 255U);
    float theta = (*iterator).theta + t * disp;
    float r =
      (*iterator).r + (1.0f + std::cos (theta * 7.0f + static_cast<float> (M_PI) + t * 4.0f)) * disp * 14.0f;
    olc::PixelGameEngine::FillCircle (center_s.x + static_cast<int32_t> (r * std::cos (theta)), center_s.y + static_cast<int32_t> (r * std::sin (theta)),
                                      3, color);

    t += aberr;
    color.r = 0U; color.g = 255U;
    theta = (*iterator).theta + t * disp;
    r =
      (*iterator).r + (1.0f + std::cos (theta * 7.0f + static_cast<float> (M_PI) + t * 4.0f)) * disp * 14.0f;
    olc::PixelGameEngine::FillCircle (center_s.x + static_cast<int32_t> (r * std::cos (theta)), center_s.y + static_cast<int32_t> (r * std::sin (theta)),
                                      3, color);

    t += aberr;
    color.g = 0U; color.b = 255U;
    theta = (*iterator).theta + t * disp;
    r =
      (*iterator).r + (1.0f + std::cos (theta * 7.0f + static_cast<float> (M_PI) + t * 4.0f)) * disp * 14.0f;
    olc::PixelGameEngine::FillCircle (center_s.x + static_cast<int32_t> (r * std::cos (theta)), center_s.y + static_cast<int32_t> (r * std::sin (theta)),
                                      3, color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
