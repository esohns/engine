#include "stdafx.h"

#include "pge_170.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_170::PGE_170 ()
 : olc::PixelGameEngine ()
 , index_ (0)
 , particles_ (ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES, {0.0f, 0.0f})
 , lines_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 170");
}

bool
PGE_170::OnUserCreate ()
{
  for (int i = ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES; i--;)
  {
    float d = static_cast<float> (std::pow (i, 1.0351f));
    particles_[i] = {std::cos (d) * (i / static_cast<float> (ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES)),
                     std::sin (d) * (i / static_cast<float> (ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES))};
  } // end FOR

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_170::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  if (index_ < ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES)
  {
    for (int i = index_; i < index_ + 8; i++)
      for (int j = i + 1; j < ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES; j++)
      {
        float d = (particles_[i].x - particles_[j].x) * (particles_[i].x - particles_[j].x) + (particles_[i].y - particles_[j].y) * (particles_[i].y - particles_[j].y);
        float condition_b = true;
        for (std::vector<olc::vf2d>::iterator iterator = particles_.begin ();
             iterator != particles_.end ();
             ++iterator)
          if (d > (particles_[i].x - (*iterator).x) * (particles_[i].x - (*iterator).x) + (particles_[i].y - (*iterator).y) * (particles_[i].y - (*iterator).y) &&
              d > (particles_[j].x - (*iterator).x) * (particles_[j].x - (*iterator).x) + (particles_[j].y - (*iterator).y) * (particles_[j].y - (*iterator).y))
          {
            condition_b = false;
            break;
          } // end IF
        if (condition_b)
          lines_.push_back (std::make_pair (i, j));
      } // end FOR

    index_ += 8;
  } // end IF

  float s = half_width_i * (ENGINE_PGE_170_DEFAULT_NUMBER_OF_PARTICLES / static_cast<float> (index_));
  for (int k = 2; k--;)
  {
    //strokeWeight (k ? 4 : 3)
    uint32_t stroke = (k ? 0xFFFFFFFF : 0xFEFEFEFE);
    olc::Pixel color = (k ? olc::BLACK : olc::WHITE);
    for (std::vector<std::pair<int, int> >::iterator iterator = lines_.begin ();
         iterator != lines_.end ();
         ++iterator)
      olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (particles_[(*iterator).first].x * s), half_height_i + static_cast<int32_t> (particles_[(*iterator).first].y * s),
                                      half_width_i + static_cast<int32_t> (particles_[(*iterator).second].x * s), half_height_i + static_cast<int32_t> (particles_[(*iterator).second].y * s),
                                      color, stroke);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
