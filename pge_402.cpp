#include "stdafx.h"

#include "pge_402.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_18.h"

PGE_402::PGE_402 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = "Example 402";
}

bool
PGE_402::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_402::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_402_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_402_DEFAULT_ALPHA_DECAY : 0);
  if (frame_count_i % ENGINE_PGE_402_DEFAULT_CYCLE_DURATION == 0)
    olc::PixelGameEngine::Clear (olc::WHITE);

  static olc::Pixel bg_color (255, 255, 255, 10);
  olc::PixelGameEngine::FillRect (0, 0, olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight (), bg_color);

  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);
  for (int i = 0; i < ENGINE_PGE_402_DEFAULT_NUMBER_OF_PLOTTERS; i++)
  {
    olc::vf2d position_s (Common_Tools::getRandomNumber (0.0f, 1.0f) * static_cast<float> (ENGINE_PGE_402_DEFAULT_SIZE) - (ENGINE_PGE_402_DEFAULT_SIZE / 2.0f),
                          Common_Tools::getRandomNumber (0.0f, 1.0f) * static_cast<float> (ENGINE_PGE_402_DEFAULT_SIZE) - (ENGINE_PGE_402_DEFAULT_SIZE / 2.0f));
    float distanceTo_0_0 = std::hypot (position_s.x, position_s.y);
    float angleTo_0_0 = std::atan2 (position_s.x, position_s.y);
    //// *EDIT*: to make atan2() 'look' more symmetric
    //if (angleTo_0_0 < 0.0f)
    //  angleTo_0_0 += 2.0f * static_cast<float> (M_PI);

    float distanceLike = std::log (distanceTo_0_0 / 3.0f);
    static float petals_divisor_f = ENGINE_PGE_402_DEFAULT_NUMBER_OF_PETALS / 2.0f;
    float angleLike = 0.5f * std::sin (angleTo_0_0 * petals_divisor_f);

    // The radius of the flower. Very small at the beginning of
    // the cycle, growing for a short time, then a fix value
    static float maxRadius = ENGINE_PGE_402_DEFAULT_SIZE * 0.3f;
    static float cycle_divisor_f = ENGINE_PGE_402_DEFAULT_CYCLE_DURATION / 3.0f;
    float radius =
      maxRadius * std::min (1.0f, (frame_count_i % ENGINE_PGE_402_DEFAULT_CYCLE_DURATION) / cycle_divisor_f);

    // Get plotter properties randommly based on its distance
    // and direction to the center. In fact, those lines is
    // the part that has the most impact in the final rendering
    float val =
      //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (distanceLike, angleLike, static_cast<float> (frame_count_i) * ENGINE_PGE_402_DEFAULT_SPEED)), -1.0f, 1.0f, 0.0f, 1.0f);
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (distanceLike, angleLike, static_cast<float> (frame_count_i) * ENGINE_PGE_402_DEFAULT_SPEED)), -1.0f, 1.0f, 0.0f, 1.0f);
    // *TODO*: this is the original code, but the results don't look like they should...why ?
    // see also: https://openprocessing.org/sketch/2268138
    //float bloomingFactor = std::min (1.0f, 2.0f * (distanceTo_0_0 / radius));
    //struct plotter plotter_s = getPlotter (val * bloomingFactor);
    // *TODO*: this looks a little more like it should...
    float bloomingFactor = 2.0f * (distanceTo_0_0 / radius);
    struct plotter plotter_s = getPlotter (std::min (1.0f, val * bloomingFactor));

    if (distanceTo_0_0 > radius)
    {
      // Fade out the plotter outside the radius
      plotter_s.color.a = 255 - static_cast<uint8_t> (5.0f * (distanceTo_0_0 - radius));
    } // end IF

    int32_t radius_i = static_cast<int32_t> (plotter_s.radius);
    if (radius_i > 0)
      olc::PixelGameEngine::FillCircle (center_s + position_s, radius_i, plotter_s.color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

PGE_402::plotter
PGE_402::getPlotter (float value)
{
  struct plotter plotter_s;

  static olc::Pixel blue1 (150, 194, 225, ENGINE_PGE_402_DEFAULT_ALPHA);
  static olc::Pixel blue2 (118, 151, 189, ENGINE_PGE_402_DEFAULT_ALPHA);
  static olc::Pixel pink  (252, 142, 172, ENGINE_PGE_402_DEFAULT_ALPHA);
  static olc::Pixel palette_a[ENGINE_PGE_402_DEFAULT_PALETTE_SIZE] = { olc::WHITE,
                                                                       pink,
                                                                       blue2,
                                                                       blue1,
                                                                       blue2,
                                                                       blue1,
                                                                       pink,
                                                                       blue2,
                                                                       blue1,
                                                                       olc::WHITE,
                                                                       olc::WHITE };

  // Size of an interval
  static float size = 1.0f / ENGINE_PGE_402_DEFAULT_PALETTE_SIZE;
  // Corresponding color index for the current value
  int index = static_cast<int> (std::floor (value / size));
  //int index =
  //  static_cast<int> (Common_GL_Tools::map (std::floor (value / size), 0.0f, static_cast<float> (ENGINE_PGE_402_DEFAULT_PALETTE_SIZE), 0.0f, static_cast<float> (ENGINE_PGE_402_DEFAULT_PALETTE_SIZE - 1)));
  // Corresponding [0,1] in the current color interval.
  // I.e. if there are 2 colors, and value is 0.6, this will
  // be 0.2. I.e size is 0.5, and 0.6 is at 20 % of the current
  // color interval (last color)
  float valueInInterval = (value - (index * size)) / size;

  // To get the radius, with fading ones on color transition
  static float center = 0.5f;
  float radiusScale = 2.0f * (center - std::abs (valueInInterval - center));

  plotter_s.color = palette_a[index];
  plotter_s.radius = ENGINE_PGE_402_DEFAULT_MAX_RADIUS * radiusScale;

  return plotter_s;
}
