#include "stdafx.h"

#include "pge_259.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_259::PGE_259 ()
 : olc::PixelGameEngine ()
 , columns_ ()
 , rows_ ()
 , main_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 259");
}

bool
PGE_259::OnUserCreate ()
{
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i += ENGINE_PGE_259_DEFAULT_BLOCK_SIZE)
    for (int j = 0; j < olc::PixelGameEngine::ScreenHeight (); j += ENGINE_PGE_259_DEFAULT_BLOCK_SIZE)
      main_.push_back (point (static_cast<float> (i), static_cast<float> (j)));

  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth (); i += ENGINE_PGE_259_DEFAULT_BLOCK_SIZE)
  {
    std::vector<point*> column_a;
    for (std::vector<point>::iterator iterator = main_.begin ();
         iterator != main_.end ();
         ++iterator)
      if ((*iterator).origin_.x == static_cast<float> (i))
        column_a.push_back (&(*iterator));
    columns_.push_back (column_a);
  } // end FOR

  for (int i = 0; i < olc::PixelGameEngine::ScreenHeight (); i += ENGINE_PGE_259_DEFAULT_BLOCK_SIZE)
  {
    std::vector<point*> row_a;
    for (std::vector<point>::iterator iterator = main_.begin ();
         iterator != main_.end ();
         ++iterator)
      if ((*iterator).origin_.y == static_cast<float> (i))
        row_a.push_back (&(*iterator));
    rows_.push_back (row_a);
  } // end FOR

  return true;
}

bool
PGE_259::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  olc::HWButton mouse_0_s = olc::PixelGameEngine::GetMouse (0);
  if (mouse_0_s.bPressed || mouse_0_s.bHeld)
    burst ();

  for (std::vector<point>::iterator iterator = main_.begin ();
        iterator != main_.end ();
        ++iterator)
    (*iterator).update ();

  olc::vf2d prev_s;
  bool is_first_b = true;
  for (std::vector< std::vector<point*> >::iterator iterator = rows_.begin ();
       iterator != rows_.end ();
       ++iterator)
  {
    is_first_b = true;
    for (std::vector<point*>::iterator iterator_2 = (*iterator).begin ();
         iterator_2 != (*iterator).end ();
         ++iterator_2)
    {
      if (is_first_b)
      {
        is_first_b = false;
        prev_s = (*iterator_2)->position_;
      } // end IF
      olc::PixelGameEngine::DrawLine (prev_s, (*iterator_2)->position_, olc::BLACK, 0xFFFFFFFF);
      prev_s = (*iterator_2)->position_;
    } // end FOR
  } // end FOR

  for (std::vector< std::vector<point*> >::iterator iterator = columns_.begin ();
       iterator != columns_.end ();
       ++iterator)
  {
    is_first_b = true;
    for (std::vector<point*>::iterator iterator_2 = (*iterator).begin ();
         iterator_2 != (*iterator).end ();
         ++iterator_2)
    {
      if (is_first_b)
      {
        is_first_b = false;
        prev_s = (*iterator_2)->position_;
      } // end IF
      olc::PixelGameEngine::DrawLine (prev_s, (*iterator_2)->position_, olc::BLACK, 0xFFFFFFFF);
      prev_s = (*iterator_2)->position_;
    } // end FOR
  } // end FOR

  olc::PixelGameEngine::FillCircle (olc::PixelGameEngine::GetMouseX (),
                                    olc::PixelGameEngine::GetMouseY (),
                                    10, olc::BLACK);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_259::burst ()
{
  olc::vf2d mouse_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                     static_cast<float> (olc::PixelGameEngine::GetMouseY ()));

  for (std::vector<point>::iterator iterator = main_.begin ();
       iterator != main_.end ();
       ++iterator)
  {
    olc::vf2d diff = (*iterator).position_ -  mouse_s;
    float distance_f = diff.mag ();
    float multi =
      static_cast<float> (std::pow (2, -(distance_f * ENGINE_PGE_259_DEFAULT_MOUSE_POWER)));
    olc::vf2d f = diff * multi;
    (*iterator).acceleration_ += f;
  } // end FOR
}
