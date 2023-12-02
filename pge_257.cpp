#include "stdafx.h"

#include "pge_257.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_257::PGE_257 ()
 : olc::PixelGameEngine ()
 , hue_ (0.0f)
 , dots_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 257");
}

bool
PGE_257::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  hue_ = Common_Tools::getRandomNumber (0.0f, 360.0f);

  float inc =
    2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_257_DEFAULT_NUMBER_OF_DOTS);
  float delta = 0.3f * inc;

  float pLength_f =
    0.3f * (olc::PixelGameEngine::ScreenWidth () < olc::PixelGameEngine::ScreenHeight () ? olc::PixelGameEngine::ScreenWidth () : olc::PixelGameEngine::ScreenHeight ());

  for (int i = 0; i < ENGINE_PGE_257_DEFAULT_NUMBER_OF_DOTS; i++)
  {
    float theta = i * inc + Common_Tools::getRandomNumber (-delta, delta);
    float x = std::cos (theta) * pLength_f + 0.5f * olc::PixelGameEngine::ScreenWidth ();
    float y = std::sin (theta) * pLength_f + 0.5f * olc::PixelGameEngine::ScreenHeight ();
    dots_.push_back (dot (x, y, hue_, pLength_f));
  } // end FOR

  return true;
}

bool
PGE_257::OnUserUpdate (float fElapsedTime)
{
  static float elapsed_time_f = fElapsedTime;

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<dot>::iterator iterator = dots_.begin ();
       iterator != dots_.end ();
       ++iterator)
    (*iterator).update (elapsed_time_f * 1000.0f);

  // Draw ring connections
  for (int i = 0; i < static_cast<int> (dots_.size ()); i++)
    drawRing (dots_[i], dots_[(i + 1) % dots_.size ()]);

  // Draw dots
  for (std::vector<dot>::iterator iterator = dots_.begin ();
       iterator != dots_.end ();
       ++iterator)
  {
    olc::PixelGameEngine::FillCircle ((*iterator).position_, static_cast<int32_t> ((*iterator).sizeCircle_), (*iterator).colorDot_);
    olc::PixelGameEngine::FillCircle ((*iterator).position_, static_cast<int32_t> ((*iterator).sizeDot_), olc::WHITE);
  } // end FOR

  elapsed_time_f += fElapsedTime;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_257::drawRing (dot& a, dot& b)
{
  float centerX = (a.position_.x + b.position_.x) / 2.0f;
  float centerY = (a.position_.y + b.position_.y) / 2.0f;
  float diameter = a.position_.dist (b.position_);

  float r, g, b_2;
  Common_Image_Tools::HSVToRGB (hue_,
                                Common_Tools::getRandomNumber (0.0f, 1.0f),
                                Common_Tools::getRandomNumber (0.2f, 0.3f),
                                r, g, b_2);
  float alpha = Common_Tools::getRandomNumber (0.4f, 0.7f);
  olc::Pixel color (static_cast<uint8_t> (r * 255.0f),
                    static_cast<uint8_t> (g * 255.0f),
                    static_cast<uint8_t> (b_2 * 255.0f),
                    static_cast<uint8_t> (alpha * 255.0f));
  olc::PixelGameEngine::DrawCircle (static_cast<int32_t> (centerX), static_cast<int32_t> (centerY), static_cast<int32_t> (diameter / 2.0f),
                                    color, 0xFF);
}
