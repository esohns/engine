#include "stdafx.h"

#include "pge_110.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_110::PGE_110 ()
 : olc::PixelGameEngine ()
 , center_ ()
 , weeds_ ()
 , noise_ ()
 , noiseRoot_ ()
 , noiseOn_ (true)
 , mode_ (1)
{
  sAppName = "Example 110";
}

PGE_110::~PGE_110 ()
{
  for (std::vector<weed*>::iterator iterator = weeds_.begin ();
       iterator != weeds_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_110::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  center_ = {olc::PixelGameEngine::ScreenWidth () / 2.0f,
             olc::PixelGameEngine::ScreenHeight () / 2.0f};

  noiseRoot_.x = Common_Tools::getRandomNumber (0.0f, 123456.0f);
  noiseRoot_.y = Common_Tools::getRandomNumber (0.0f, 123456.0f);

  for (int i = 0; i < ENGINE_PGE_110_DEFAULT_NUMBER_OF_WEEDS; i++)
    weeds_.push_back (new weed (static_cast<float> (i) * 2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_110_DEFAULT_NUMBER_OF_WEEDS),
                                3.0f * ENGINE_PGE_110_DEFAULT_RADIUS,
                                this));

  return true;
}

bool
PGE_110::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    noiseOn_ = !noiseOn_;
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
    mode_ = (mode_ + 1) % 2;

  noiseRoot_ += olc::vf2d (0.01f, 0.01f);

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<weed*>::iterator iterator = weeds_.begin ();
       iterator != weeds_.end ();
       ++iterator)
    (*iterator)->update (this, center_, &noise_, noiseRoot_, noiseOn_, mode_);

  olc::PixelGameEngine::DrawCircle (center_, static_cast<int32_t> (ENGINE_PGE_110_DEFAULT_RADIUS), {120, 0, 0, 220}, 0xFF);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
