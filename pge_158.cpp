#include "stdafx.h"

#include "pge_158.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

PGE_158::PGE_158 ()
 : olc::PixelGameEngine ()
 , balls_ ()
{
  sAppName = "Example 158";
}

bool
PGE_158::OnUserCreate ()
{
  for (int i = ENGINE_PGE_158_DEFAULT_NUMBER_OF_BALLS; i--;)
    balls_.push_back (ball (this));

  return true;
}

bool
PGE_158::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  balls_.push_back (ball (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                          static_cast<float> (olc::PixelGameEngine::GetMouseY ())));

  for (std::list<ball>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
  {
    olc::vf2d acceleration;
    for (std::list<ball>::iterator iterator_2 = balls_.begin ();
         iterator_2 != balls_.end ();
         ++iterator_2)
      if (iterator != iterator_2)
      {
        olc::vf2d difference = (*iterator_2).position_ - (*iterator).position_;
        float distance = std::max (1.0f, std::sqrt (difference.x * difference.x + difference.y * difference.y));
        float force = (distance - ENGINE_PGE_158_DEFAULT_RADIUS) * (*iterator_2).mass_ / distance;
        difference *= force;
        acceleration += difference;
      } // end IF
    acceleration *= (*iterator).mass_;
    (*iterator).velocity_ *= ENGINE_PGE_158_DEFAULT_INFLUENCE;
    (*iterator).velocity_ += acceleration;
  } // end FOR
  balls_.pop_back ();

  for (std::list<ball>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
    (*iterator).position_ += (*iterator).velocity_;

  for (std::list<ball>::iterator iterator = balls_.begin ();
       iterator != balls_.end ();
       ++iterator)
  {
    float r = Common_GL_Tools::map ((*iterator).mass_, ENGINE_PGE_158_DEFAULT_MASS_MIN, ENGINE_PGE_158_DEFAULT_MASS_MAX, 0.0f, 255.0f);
    olc::Pixel color;
    color.r = static_cast<uint8_t> (r);
    color.g = static_cast<uint8_t> (r * 3.0f > 255.0f ? 255.0f : r * 3.0f);
    color.b = static_cast<uint8_t> (255);
    olc::PixelGameEngine::FillCircle ((*iterator).position_, static_cast<int32_t> ((*iterator).mass_ * 300.0f), color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
