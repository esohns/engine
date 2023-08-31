#include "stdafx.h"

#include "pge_161.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_161::PGE_161 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , referencePoints_ (4, struct reference_point ())
{
  sAppName = "Example 161";
}

bool
PGE_161::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_161_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back ({0.0f, Common_Tools::getRandomNumber (-280.0f, 280.0f)});

  struct reference_point reference_point_s;
  for (int i = 3; i--; )
  {
    if (i == 0)
    {
      reference_point_s.position_.x = 0.0f;
      reference_point_s.position_.y = 360.0f;
      reference_point_s.ratio_ = 1.0f;
    } // end IF
    else
    {
      reference_point_s.position_.x = 0.0f;
      reference_point_s.position_.y = 50.0f;
      reference_point_s.ratio_ = 0.4f;
    } // end ELSE
    referencePoints_[i] = reference_point_s;
  } // end FOR

  return true;
}

bool
PGE_161::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<olc::vf2d>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    float angle = 0.0f;
    for (std::vector<struct reference_point>::iterator iterator_2 = referencePoints_.begin();
         iterator_2 != referencePoints_.end ();
         ++iterator_2)
      if ((*iterator_2).ratio_ > Common_Tools::getRandomNumber (0.0f, 1.0f))
        angle += std::atan2 ((*iterator_2).position_.y - (*iterator).y, (*iterator_2).position_.x - (*iterator).x);
    float offset_x = std::cos (angle) * 4.0f;
    float offset_y = std::sin (angle) * 4.0f;
    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> ((*iterator).x), half_height_i + static_cast<int32_t> ((*iterator).y),
                                    half_width_i + static_cast<int32_t> ((*iterator).x + offset_x), half_height_i + static_cast<int32_t> ((*iterator).y + offset_y),
                                    olc::BLACK, 0xFFFFFFFF);
    (*iterator).x += offset_x;
    (*iterator).y += offset_y;
    if ((*iterator).y > 280.0f)
    {
      (*iterator).x = 0.0f;
      (*iterator).y = -280.0f;
    } // end IF
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
