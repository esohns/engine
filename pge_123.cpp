#include "stdafx.h"

#include "pge_123.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_123::PGE_123 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , hIndex_ (0)
 , minH_ (100)
 , maxH_ (220)
 , h_ (minH_)
 , noise_ ()
{
  sAppName = "Example 123";
}

PGE_123::~PGE_123 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
        iterator != particles_.end ();
        ++iterator)
    delete *iterator;
}

bool
PGE_123::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_123::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    for (int i = 0; i < 10; i++)
      particles_.push_back (new particle (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                                          static_cast<float> (olc::PixelGameEngine::GetMouseY ()),
                                          static_cast<float> (i + particles_.size ()), static_cast<float> (i + particles_.size ()),
                                          h_, minH_, maxH_, frame_count_i));
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::C).bPressed)
  {
    for (std::vector<particle*>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
      delete *iterator;
    particles_.clear ();

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::H).bPressed)
    changeHue ();

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       )
  {
    if ((*iterator)->dead_)
    {
      delete *iterator;
      iterator = particles_.erase (iterator);
    } // end IF
    else
    {
      (*iterator)->update (&noise_, frame_count_i);
      (*iterator)->show (this);
      ++iterator;
    } // end ELSE
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_123::changeHue ()
{
  hIndex_++;
  if (hIndex_ > 4) hIndex_ = 0;
  switch (hIndex_)
  {
    case 0:
      minH_ = 100;
      maxH_ = 220;
      break;
    case 1:
      minH_ = 0;
      maxH_ = 55;
      break;
    case 2:
      minH_ = 20;
      maxH_ = 100;
      break;
    case 3:
      minH_ = 110;
      maxH_ = 180;
      break;
    case 4:
      minH_ = 150;
      maxH_ = 255;
      break;
  } // end SWITCH

  h_ = minH_;
}
