#include "stdafx.h"

#include "pge_447.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_20.h"

PGE_447::PGE_447 ()
 : olc::PixelGameEngine ()
 , colors_ ()
 , noise_ ()
{
  sAppName = "Example 447";
}

bool
PGE_447::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  initializeColors ();

  return true;
}

bool
PGE_447::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_447_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_447_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < ENGINE_PGE_447_DEFAULT_NUMBER_OF_PLOTTERS; i++)
  {
    int32_t x = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1);
    int32_t y = Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1);
    int32_t row = static_cast<int32_t> (std::floor (y / (ENGINE_PGE_447_DEFAULT_SIZE / 2.0f)));
    int32_t column = static_cast<int32_t> (std::floor (x / (ENGINE_PGE_447_DEFAULT_SIZE / 2.0f)));
    olc::vf2d center ((column * 2 + 1) * ENGINE_PGE_447_DEFAULT_SIZE / 2.0f / 2.0f,
                      (row    * 2 + 1) * ENGINE_PGE_447_DEFAULT_SIZE / 2.0f / 2.0f);
    float distanceToCenter = std::hypot (x - center.x, y - center.y);
    float directionToCenter = std::atan2 (x - center.x, y - center.y);
    float nearestBranchAngle =
      (std::round ((directionToCenter * ENGINE_PGE_447_DEFAULT_SYMMETRIES) / (2.0f * static_cast<float> (M_PI))) * (2.0f * static_cast<float> (M_PI)) / ENGINE_PGE_447_DEFAULT_SYMMETRIES);
    float angleToNearestBranch = directionToCenter - nearestBranchAngle;
    distanceToCenter *= std::cos (angleToNearestBranch);
    double distanceLike = std::log (distanceToCenter / 3.0);

    float distanceToNearestBranch = std::sin (angleToNearestBranch);
    double angleLike = std::round (distanceToNearestBranch * 5.0) / 5.0;

    double areaSeed = row * 13.0 + column * 41.0;
    double timeLike = frame_count_i * ENGINE_PGE_447_DEFAULT_SPEED + areaSeed;

    float val =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (distanceLike, angleLike, timeLike)), -1.0f, 1.0f, 0.0f, 1.0f);

    static float maxDrawingRadius = ENGINE_PGE_447_DEFAULT_SIZE / 2.0f / 2.0f * 0.6f;
    float drawingRadius =
      maxDrawingRadius * std::min (1.0f, (frame_count_i % ENGINE_PGE_447_DEFAULT_CYCLE_DURATION) / (ENGINE_PGE_447_DEFAULT_CYCLE_DURATION / 3.0f));

    float bloomingFactor = std::min (1.0f, 2.0f * distanceToCenter / drawingRadius);
    val *= bloomingFactor;

    struct plotter plotter = getPlotter (val);

    static float branchLimit = 0.02f;
    if (std::abs (distanceToNearestBranch) < branchLimit)
    {
      plotter.color = colors_.color3;
      plotter.radius = 2;
    } // end IF
    if (distanceToCenter > drawingRadius)
      plotter.color.a = static_cast<uint8_t> (255.0f - 5.0f * (distanceToCenter - drawingRadius));

    // At last, actually plot the dot.
    olc::PixelGameEngine::FillCircle (x, y, plotter.radius, plotter.color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_447::initializeColors ()
{
  static olc::Pixel pink (255, 143, 248);
  static olc::Pixel blue1 (70, 198, 255);
  static olc::Pixel blue2 (45, 121, 253);
  static olc::Pixel blue3 (1, 43, 210);

  colors_.color1 = blue3;
  colors_.color2 = blue2;
  colors_.color3 = pink;

  colors_.background = olc::BLACK;
}

struct PGE_447::plotter
PGE_447::getPlotter (float value_in)
{
  static olc::Pixel palette_a[ENGINE_PGE_447_DEFAULT_PALETTE_SIZE] = {
    colors_.background,
    colors_.background,
    colors_.background,
    colors_.color1,
    colors_.background,
    colors_.background,
    colors_.color2,
    colors_.background,
    colors_.background,
    colors_.color3,
    colors_.background,
    colors_.background,
    colors_.color1,
    colors_.background,
    colors_.background,
    colors_.color2,
    colors_.background,
    colors_.background,
    colors_.color3,
    colors_.background,
    colors_.background,
    colors_.color1,
    colors_.background,
    colors_.background,
    colors_.background,
  };
  static float size = 1.0f / ENGINE_PGE_447_DEFAULT_PALETTE_SIZE;

  int index = static_cast<int> (std::floor (value_in / size));
  if (index < 0)
    index = 0;
  else if (index > ENGINE_PGE_447_DEFAULT_PALETTE_SIZE - 1)
    index = ENGINE_PGE_447_DEFAULT_PALETTE_SIZE - 1;

  float valueInInterval = (value_in - (index * size)) / size;
  static float center_f = 0.5f;
  float radiusScale = 2.0f * (center_f - std::abs (valueInInterval - center_f));

  struct plotter plotter_s;
  plotter_s.color = palette_a[index];
  plotter_s.radius = static_cast<int32_t> (ENGINE_PGE_447_DEFAULT_MAX_RADIUS * radiusScale / 2.0f);

  return plotter_s;
}
