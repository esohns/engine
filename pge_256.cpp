#include "stdafx.h"

#include "pge_256.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "engine_common.h"

PGE_256::PGE_256 ()
 : olc::PixelGameEngine ()
 , currentHue_ (0.0f)
 , layerCounter_ (0)
 , phase_ (0.0f)
 , spawnPhase_ (0.0f)
 , zaps_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 256");
}

bool
PGE_256::OnUserCreate ()
{

  return true;
}

bool
PGE_256::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  render ();

  phase_ += 1 / 32.0f;
  currentHue_ += 0.6f;
  currentHue_ = std::fmod (currentHue_, 360.0f);
  spawnPhase_ += 0.15f;

  if (phase_ > 1.0f)
  {
    phase_ -= 1;
    layerCounter_ += 1;
  } // end IF

  for (std::vector<zap>::iterator iterator = zaps_.begin ();
       iterator != zaps_.end ();
       ++iterator)
    (*iterator).update (layerCounter_);
  for (int i = static_cast<int> (zaps_.size () - 1);
       i >= 0;
       --i)
    if (!zaps_[i].alive_)
    {
      std::vector<zap>::iterator iterator = zaps_.begin ();
      std::advance (iterator, i);
      zaps_.erase (iterator);
    } // end IF

  if (spawnPhase_ > 1.0f)
  {
    spawnPhase_ -= 1.0f;

    float r, g, b;
    Common_Image_Tools::HSVToRGB (currentHue_, 56.0f / 100.0f, 96.0f / 100.0f, r, g, b);
    olc::Pixel color;
    color.r = static_cast<uint8_t> (r * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);

    zaps_.push_back (zap (color, layerCounter_));
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_256::render ()
{
  for (int i = 0; i < 24; i++)
    for (int j = 0; j < ENGINE_PGE_256_DEFAULT_MAXJ; j++)
    {
      static olc::Pixel color1 (120, 120, 120);
      static olc::Pixel color2 (40, 40, 40);
      olc::Pixel color = (j % 2 == 0 ? color1 : color2);
      float r;
      olc::vf2d pos = getGridPos (i, j, phase_, r);
      float diameter = diameterFunc (r);
      olc::PixelGameEngine::FillCircle (pos, static_cast<int32_t> (diameter / 2.0f), color);
    } // end FOR

  for (std::vector<zap>::iterator iterator = zaps_.begin ();
       iterator != zaps_.end ();
       ++iterator)
    (*iterator).show (this, layerCounter_, phase_);
}

olc::vf2d
getGridPos (int i, int j, float phase, float& r)
{
  static int half_width_i  = 400;
  static int half_height_i = 400;

  r = std::pow (1.124f, i - phase) * 30.0f;

  float theta =
    j / static_cast<float> (ENGINE_PGE_256_DEFAULT_MAXJ) * 2.0f * static_cast<float> (M_PI) + (i - phase) * 0.1563f;
  float x = std::cos (theta) * r + half_width_i;
  float y = std::sin (theta) * r + half_height_i;

  return olc::vf2d (x, y);
}

float
smoothMin (float a, float b, float k)
{
  float h = std::max (k - std::abs (a - b), 0.0f) / k;
  return std::min (a, b) - h * h * k / 4.0f;
}

float
diameterFunc (float r)
{
  return smoothMin (r / 8.0f, 12.0f, 9.0f);
}
