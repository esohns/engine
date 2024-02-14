#include "stdafx.h"

#include "pge_361.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_17.h"

PGE_361::PGE_361 ()
 : olc::PixelGameEngine ()
 , stars_ ()
 , holes_ ()
 , color_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 361");
}

bool
PGE_361::OnUserCreate ()
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  stars_.clear ();
  for (int i = 0; i < ENGINE_PGE_361_DEFAULT_NUMBER_OF_STARS; i++)
    stars_.push_back (star (std::cos (static_cast<float> (i)) * ENGINE_PGE_361_DEFAULT_R,
                            std::sin (static_cast<float> (i)) * ENGINE_PGE_361_DEFAULT_R));

  holes_.clear ();
  for (int i = 0; i < ENGINE_PGE_361_DEFAULT_NUMBER_OF_HOLES; i++)
    holes_.push_back (hole (static_cast<float> (Common_Tools::getRandomNumber (-ENGINE_PGE_361_DEFAULT_R, ENGINE_PGE_361_DEFAULT_R)),
                            static_cast<float> (Common_Tools::getRandomNumber (-ENGINE_PGE_361_DEFAULT_R, ENGINE_PGE_361_DEFAULT_R))));

  return true;
}

bool
PGE_361::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    OnUserCreate ();

  //olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_361_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_361_DEFAULT_ALPHA_DECAY : 0);

  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  color_ = (color_ + 3) % 360;
  float r, g, b;
  Common_Image_Tools::HSVToRGB (static_cast<float> (color_), 1.0f, 1.0f, r, g, b);
  olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 255);

  for (int s = 0; s < static_cast<int> (stars_.size ()); s++)
  {
    olc::vf2d origin_s = stars_[s].position_;

    float direction = 0.0f;
    for (int bh = 0; bh < static_cast<int> (holes_.size ()); bh++)
      direction += std::atan2 (holes_[bh].position_.y - origin_s.y, holes_[bh].position_.x - origin_s.x) * 2.0f;

    olc::vf2d MOVE (5.0f, 0.0f);
    float x_rotated = std::cos (direction) * MOVE.x - std::sin (direction) * MOVE.y;
    float y_rotated = std::sin (direction) * MOVE.x + std::cos (direction) * MOVE.y;
    MOVE.x = x_rotated; MOVE.y = y_rotated;
    stars_[s].position_ += MOVE;

    olc::PixelGameEngine::DrawLine (center_s + origin_s, center_s + stars_[s].position_, color, 0xFFFFFFFF);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
