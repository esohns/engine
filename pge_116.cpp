#include "stdafx.h"

#include "pge_116.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_116::PGE_116 ()
 : olc::PixelGameEngine ()
 , center_ ()
 , weeds_ ()
 , noise_ ()
 , noiseRoot_ ()
 , noiseOn_ (true)
 , touchCount_ (0)
 , mouseDist_ (0.0f)
 , targetSpeed_ ()
{
  sAppName = "Example 116";
}

PGE_116::~PGE_116 ()
{
  for (std::vector<weed*>::iterator iterator = weeds_.begin ();
       iterator != weeds_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_116::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  center_ = {olc::PixelGameEngine::ScreenWidth () / 2.0f,
             olc::PixelGameEngine::ScreenHeight () / 2.0f};

  noiseRoot_.x = Common_Tools::getRandomNumber (0.0f, 123456.0f);
  noiseRoot_.y = Common_Tools::getRandomNumber (0.0f, 123456.0f);

  for (int i = 0; i < ENGINE_PGE_116_DEFAULT_NUMBER_OF_WEEDS; i++)
    weeds_.push_back (new weed (static_cast<float> (i) * 2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_116_DEFAULT_NUMBER_OF_WEEDS),
                                1.0f * ENGINE_PGE_116_DEFAULT_RADIUS,
                                this,
                                &noise_));

  return true;
}

bool
PGE_116::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetKey (olc::Key::N).bHeld)
    noiseOn_ = !noiseOn_;

  noiseRoot_ += olc::vf2d (0.01f, 0.01f);

  static olc::vf2d prev_mouse;
  olc::vf2d mouse (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                   static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  float d = mouse.dist (prev_mouse);
  prev_mouse = mouse;
  float targetMouseDist = 3.0f * d;
  if (d > 0)
    mouseDist_ = std::min (mouseDist_ + d / 10.0f, 50.0f);
  else
    mouseDist_ = std::max (mouseDist_ - 1.0f, 0.0f);
  float tmpa = 0.0f;
  olc::vf2d targetCenter;
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    targetCenter = mouse;
    targetSpeed_ = targetCenter;
    targetSpeed_ -= center_;
    targetSpeed_ *= 0.06f;
  } // end IF
  else
  {
    int tmpTouchCount = touchCount_;
    if (tmpTouchCount < 0) tmpTouchCount = 0;
    if (tmpTouchCount > 20) tmpTouchCount = 20;
    tmpa =
      ENGINE_PGE_116_DEFAULT_A + (ENGINE_PGE_116_DEFAULT_BOTTOM - ENGINE_PGE_116_DEFAULT_A) * Common_GL_Tools::map (static_cast<float> (tmpTouchCount), 0.0f, 20.0f, 1.0f, 0.0f);
    targetSpeed_.y += (tmpa - center_.y) * 0.001f;
    targetSpeed_ *= 0.97f;
  } // end ELSE
  center_ += targetSpeed_;
  if (center_.y > ENGINE_PGE_116_DEFAULT_BOTTOM)
  {
    center_.y = ENGINE_PGE_116_DEFAULT_BOTTOM;
    targetSpeed_.y *= -1.0f;
  } // end IF

  olc::PixelGameEngine::Clear (olc::BLACK);

  touchCount_ = 0;
  for (std::vector<weed*>::iterator iterator = weeds_.begin ();
       iterator != weeds_.end ();
       ++iterator)
    (*iterator)->update (this, center_, &noise_, noiseRoot_, noiseOn_, mouseDist_, touchCount_);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
