#include "stdafx.h"

#include "pge_107.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_107::PGE_107 ()
 : olc::PixelGameEngine ()
 , fields_ ()
 , particles_ ()
{
  sAppName = "Example 107";
}

PGE_107::~PGE_107 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
  for (std::vector<field*>::iterator iterator = fields_.begin ();
       iterator != fields_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_107::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  float r, g, b;
  for (int i = 0; i < ENGINE_PGE_107_DEFAULT_NUMBER_OF_FIELDS; i++)
  {
    float hue_f = Common_Tools::getRandomNumber (0.0f, 360.0f);
    float saturation_f = Common_Tools::getRandomNumber (0.4f, 1.0f);
    float brightness_f = Common_Tools::getRandomNumber (0.5f, 1.0f);
    Common_Image_Tools::HSVToRGB (hue_f, saturation_f, brightness_f, r, g, b);
    olc::Pixel color_s;
    color_s.r = static_cast<uint8_t> (r * 255.0f);
    color_s.g = static_cast<uint8_t> (g * 255.0f);
    color_s.b = static_cast<uint8_t> (b * 255.0f);
    color_s.a = static_cast<uint8_t> (0.05f * 255.0f);
    fields_.push_back (new field (static_cast<float> (Common_Tools::getRandomNumber (50, olc::PixelGameEngine::ScreenWidth () - 50)),
                                  static_cast<float> (Common_Tools::getRandomNumber (50, olc::PixelGameEngine::ScreenHeight () - 50)),
                                  (Common_Tools::testRandomProbability (0.5f) ? -1.0f : 1.0f) * Common_Tools::getRandomNumber (50.0f, 200.0f),
                                  10000.0f,
                                  color_s));
  } // end FOR

  for (int32_t y = 0; y < olc::PixelGameEngine::ScreenHeight (); y += ENGINE_PGE_107_DEFAULT_DY)
    for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth (); x += ENGINE_PGE_107_DEFAULT_DX)
      particles_.push_back (new particle (static_cast<float> (x), static_cast<float> (y), closestFieldTo (static_cast<float> (x), static_cast<float> (y))->color_));

  return true;
}

bool
PGE_107::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    for (std::vector<particle*>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      delete *iterator;
    particles_.clear ();
    for (std::vector<field*>::iterator iterator = fields_.begin ();
         iterator != fields_.end ();
         ++iterator)
      delete *iterator;
    fields_.clear ();
    OnUserCreate ();

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  olc::vf2d v;
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    v = {0.0f, 0.0f};
    for (std::vector<field*>::iterator iterator_2 = fields_.begin ();
         iterator_2 != fields_.end ();
         ++iterator_2)
      v += (*iterator_2)->value ((*iterator)->position_.x, (*iterator)->position_.y);

    olc::vf2d position_2 ((*iterator)->position_.x + v.x * 0.04f, (*iterator)->position_.y + v.y * 0.04f);
    olc::PixelGameEngine::DrawLine ((*iterator)->position_, position_2, (*iterator)->color_, 0xFFFFFFFF);
    (*iterator)->moveTo (position_2);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

PGE_107::field*
PGE_107::closestFieldTo (float x, float y)
{
  olc::vf2d position (x, y);
  
  field* result_p = NULL;
  float distance_f, min_distance_f = 1000000.0f;
  for (std::vector<field*>::iterator iterator = fields_.begin ();
       iterator != fields_.end ();
       ++iterator)
  {
    distance_f = position.dist ((*iterator)->position_);
    if (distance_f < min_distance_f)
    {
      min_distance_f = distance_f;
      result_p = *iterator;
    } // end IF
  } // end FOR

  return result_p;
}
