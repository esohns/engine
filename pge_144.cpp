#include "stdafx.h"

#include "pge_144.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_144::PGE_144 ()
 : olc::PixelGameEngine ()
 , circles_ ()
{
  sAppName = "Example 144";
}

bool
PGE_144::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = ENGINE_PGE_144_DEFAULT_NUMBER_OF_CIRCLES; i--;)
  {
    float degree = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    circles_.push_back (circle (this, degree, i));
  } // end FOR

  return true;
}

bool
PGE_144::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_144_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_144_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<circle>::iterator iterator = circles_.begin ();
       iterator != circles_.end ();
       ++iterator)
  {
    // update
    (*iterator).position_ += (*iterator).velocity_;
    if ((*iterator).position_.x < 0 || (*iterator).position_.x >= olc::PixelGameEngine::ScreenWidth ()) (*iterator).velocity_.x = -(*iterator).velocity_.x;
    if ((*iterator).position_.y < 0 || (*iterator).position_.y >= olc::PixelGameEngine::ScreenHeight ()) (*iterator).velocity_.y = -(*iterator).velocity_.y;

    // show
    for (int i = 360; i > 0; i -= 4)
      //olc::PixelGameEngine::FillCircle ((*iterator).position_, i / 2, (*iterator).color_);
      olc::PixelGameEngine::DrawCircle ((*iterator).position_, i / 2, (*iterator).color_, 0xFF);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
