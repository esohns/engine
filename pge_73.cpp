#include "stdafx.h"

#include "pge_73.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

olc::Pixel PGE_73::object::colors_a[6] = {{0xDE, 0x18, 0x3C, 0xFF},
                                          {0xF2, 0xB5, 0x41, 0xFF},
                                          {0x0C, 0x79, 0xBB, 0xFF},
                                          {0xEC, 0x4E, 0x20, 0xFF},
                                          {0x00, 0x91, 0x6E, 0xFF},
                                          {0xF6, 0x54, 0xA9, 0xFF}};

PGE_73::PGE_73 ()
 : olc::PixelGameEngine ()
 , objects_()
 , array_1_ ()
 , array_2_ ()
{
  sAppName = "Example 73";
}

PGE_73::~PGE_73 ()
{
  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_73::OnUserCreate ()
{
  int32_t number_of_objects_i = ENGINE_PGE_73_DEFAULT_NUMBER_OF_OBJECTS;
  recRect (olc::PixelGameEngine::ScreenWidth () / 2, olc::PixelGameEngine::ScreenHeight () / 2,
           static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () * 0.9f), static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () * 0.9f),
           number_of_objects_i, array_1_);
  number_of_objects_i = ENGINE_PGE_73_DEFAULT_NUMBER_OF_OBJECTS;
  recRect (olc::PixelGameEngine::ScreenWidth () / 2, olc::PixelGameEngine::ScreenHeight () / 2,
           static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () * 0.6f), static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () * 0.6f),
           number_of_objects_i, array_2_);
  for (size_t i = 0; i < array_1_.size (); i++)
  {
    olc::vf2d pos1 = {static_cast<float> (array_1_[i].x), static_cast<float> (array_1_[i].y)};
    olc::vf2d pos2 = {static_cast<float> (array_2_[i].x), static_cast<float> (array_2_[i].y)};
    float dst = pos1.dist ({olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f});
    objects_.push_back (new object (pos1, array_1_[i].width, array_1_[i].height, pos2, array_2_[i].width, array_2_[i].height, int(dst / 10.0f)));
  } // end FOR

  return true;
}

bool
PGE_73::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_73_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_73_DEFAULT_ALPHA_DECAY : 0);
  //olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<object*>::iterator iterator = objects_.begin ();
       iterator != objects_.end ();
       ++iterator)
  {
    (*iterator)->show (this);
    (*iterator)->update ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_73::recRect (int32_t x_in, int32_t y_in, int32_t width_in, int32_t height_in, int32_t num_in, std::vector<Common_UI_Rectangle>& array_in)
{
  int32_t ww =
    static_cast<int32_t> (Common_Tools::getRandomNumber (0.1f, 0.9f) * width_in);
  int32_t hh =
    static_cast<int32_t> (Common_Tools::getRandomNumber (0.1f, 0.9f) * height_in);
  num_in--;

  if (num_in >= 0)
  {
    if (width_in < height_in)
    {
      recRect (x_in, y_in - (height_in / 2) + (hh / 2), width_in, hh, num_in, array_in);
      recRect (x_in, y_in + (height_in / 2) - (height_in - hh) / 2, width_in, (height_in - hh), num_in, array_in);
    } // end IF
    else
    {
      recRect (x_in - (width_in / 2) + (ww / 2), y_in, ww, height_in, num_in, array_in);
      recRect (x_in + (width_in / 2) - (width_in - ww) / 2, y_in, (width_in - ww), height_in, num_in, array_in);
    } // end ELSE
  } // end IF
  else
  {
    Common_UI_Rectangle rectangle_s;
    rectangle_s.x = static_cast<short> (x_in);
    rectangle_s.y = static_cast<short> (y_in);
    rectangle_s.width = static_cast<unsigned short> (width_in);
    rectangle_s.height = static_cast<unsigned short> (height_in);
    array_in.push_back (rectangle_s);
  } // end ELSE
}
