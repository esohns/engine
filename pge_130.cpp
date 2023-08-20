#include "stdafx.h"

#include "pge_130.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_130::PGE_130 ()
 : olc::PixelGameEngine ()
 , positions_ ()
{
  sAppName = "Example 130";
}

bool
PGE_130::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_130_DEFAULT_NUMBER_OF_SEGMENTS; i++)
    positions_.push_back ({0.0f, 0.0f});

  return true;
}

bool
PGE_130::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  olc::vf2d parent = {static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                      static_cast<float> (olc::PixelGameEngine::GetMouseY ())};
  int cnt = 0;
  olc::Pixel color;
  float r, g, b;
  for (std::vector<olc::vf2d>::iterator iterator = positions_.begin ();
       iterator != positions_.end ();
       ++iterator)
  {
    float angle = std::atan2 ((*iterator).y - parent.y, (*iterator).x - parent.x);
    (*iterator).x = parent.x + std::cos (angle) * ENGINE_PGE_130_DEFAULT_SEGMENT_LENGTH;
    (*iterator).y = parent.y + std::sin (angle) * ENGINE_PGE_130_DEFAULT_SEGMENT_LENGTH;

    Common_Image_Tools::HSVToRGB ((cnt / static_cast<float> (ENGINE_PGE_130_DEFAULT_NUMBER_OF_SEGMENTS)) * 360.0f, 1.0f, 1.0f, r, g, b);
    color.r = static_cast<uint8_t> (r * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);
    //color.a = 255;
    // *NOTE*: emulate strokeWeight(20)
    for (int i = 0; i < 12 / 2; i++)
    {
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (parent.x - i), static_cast<int32_t> (parent.y - i),
                                      static_cast<int32_t> ((*iterator).x - i), static_cast<int32_t> ((*iterator).y - i),
                                      color, 0xFFFFFFFF);
      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (parent.x + i), static_cast<int32_t> (parent.y + i),
                                      static_cast<int32_t> ((*iterator).x + i), static_cast<int32_t> ((*iterator).y + i),
                                      color, 0xFFFFFFFF);
    } // end FOR
    olc::PixelGameEngine::FillCircle (parent, 10, color);
    olc::PixelGameEngine::FillCircle (parent, 3, {0, 0, 0, static_cast<uint8_t> (0.4f * 255.0f) });
    parent = (*iterator);
    cnt++;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
