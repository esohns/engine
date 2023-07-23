#include "stdafx.h"

#include "pge_65.h"

#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_65::PGE_65 ()
 : olc::PixelGameEngine ()
 , cells_ (NULL)
 , zoom_ (0.0f)
{
  sAppName = "Example 65";
}

PGE_65::~PGE_65 ()
{
  delete [] cells_;
}

bool
PGE_65::OnUserCreate ()
{
  int32_t m = std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ());
  zoom_ = ENGINE_PGE_65_DEFAULT_N / static_cast<float> (m);
  cells_ = new bool[ENGINE_PGE_65_DEFAULT_N * ENGINE_PGE_65_DEFAULT_N];
  ACE_OS::memset (cells_, 0, sizeof (bool) * ENGINE_PGE_65_DEFAULT_N * ENGINE_PGE_65_DEFAULT_N);

  olc::PixelGameEngine::Clear (olc::BLACK);

  return true;
}

bool
PGE_65::OnUserUpdate (float fElapsedTime)
{
  olc::Pixel* p = GetDrawTarget ()->GetData ();

  // mouse (brush)
  int32_t mx = static_cast<int32_t> (olc::PixelGameEngine::GetMouseX () * zoom_);
  if (mx < 0) mx = 0;
  else if (mx > ENGINE_PGE_65_DEFAULT_N - 1) mx = ENGINE_PGE_65_DEFAULT_N - 1;
  int32_t my = static_cast<int32_t> (olc::PixelGameEngine::GetMouseY () * zoom_);
  if (my < 0) my = 0;
  else if (my > ENGINE_PGE_65_DEFAULT_N - 1) my = ENGINE_PGE_65_DEFAULT_N - 1;
  int32_t b = ENGINE_PGE_65_DEFAULT_BRUSH_THICKNESS;
  for (int dy = -b; dy <= b; dy++)
    for (int dx = -b; dx <= b; dx++)
    {
      float d = static_cast<float> (std::sqrt (dx * dx + dy * dy));
      if (d > static_cast<float> (b)) continue;
      int32_t x = mx + dx;
      int32_t y = my + dy;
      if (x < 0 || x >= ENGINE_PGE_65_DEFAULT_N || y < 0 || y >= ENGINE_PGE_65_DEFAULT_N) continue;
      cells_[x + y * ENGINE_PGE_65_DEFAULT_N] =
        Common_Tools::getRandomNumber (0, 5000) < 1 && b - d < 1 ? true : false; // most likely dead
    } // end FOR

  bool* next_p = new bool[ENGINE_PGE_65_DEFAULT_N * ENGINE_PGE_65_DEFAULT_N];
  ACE_OS::memset (next_p, 0, sizeof (bool) * ENGINE_PGE_65_DEFAULT_N * ENGINE_PGE_65_DEFAULT_N);
  for (int y = 4; y < ENGINE_PGE_65_DEFAULT_N - 4; y++)
    for (int x = 4; x < ENGINE_PGE_65_DEFAULT_N - 4; x++)
    {
      int i = x + y * ENGINE_PGE_65_DEFAULT_N;

      int hood0 =
        cells_[i + 1] +
        cells_[i - 1] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 1 - ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 1 + ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 1 - ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 1 + ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 2] +
        cells_[i + 2 + ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 2 - ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 2] +
        cells_[i - 2 + ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 2 - ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N - 1];

      int hood1 =
        cells_[i + 1] +
        cells_[i + 2] +
        cells_[i + 3] +
        cells_[i + 4] +
        cells_[i - 1] +
        cells_[i - 2] +
        cells_[i - 3] +
        cells_[i - 4] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 3 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - 4 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 3 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i + 4 * ENGINE_PGE_65_DEFAULT_N] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N + 2] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N + 3] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N + 4] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N + 2] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N + 3] +
        cells_[i - 3 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i - 3 * ENGINE_PGE_65_DEFAULT_N + 2] +
        cells_[i - 4 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N + 2] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N + 3] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N + 4] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N + 2] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N + 3] +
        cells_[i + 3 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i + 3 * ENGINE_PGE_65_DEFAULT_N + 2] +
        cells_[i + 4 * ENGINE_PGE_65_DEFAULT_N + 1] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N - 2] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N - 3] +
        cells_[i - ENGINE_PGE_65_DEFAULT_N - 4] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N - 2] +
        cells_[i - 2 * ENGINE_PGE_65_DEFAULT_N - 3] +
        cells_[i - 3 * ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i - 3 * ENGINE_PGE_65_DEFAULT_N - 2] +
        cells_[i - 4 * ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N - 2] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N - 3] +
        cells_[i + ENGINE_PGE_65_DEFAULT_N - 4] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N - 2] +
        cells_[i + 2 * ENGINE_PGE_65_DEFAULT_N - 3] +
        cells_[i + 3 * ENGINE_PGE_65_DEFAULT_N - 1] +
        cells_[i + 3 * ENGINE_PGE_65_DEFAULT_N - 2] +
        cells_[i + 4 * ENGINE_PGE_65_DEFAULT_N - 1];

      bool state_b = getNextState (cells_[i], hood0, hood1);
      next_p[i] = state_b;
      p[i] = state_b ? olc::WHITE : olc::BLACK;
    } // end FOR

  delete [] cells_; cells_ = next_p;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

bool
PGE_65::getNextState (bool state_in, int hood0_in, int hood1_in)
{
  return state_in ? hood0_in < 19 : hood0_in * 2.6f > static_cast<float> (hood1_in);
  //return state_in ? hood0_in < 19 && hood1_in < 36 : hood0_in * 2.6f > static_cast<float> (hood1_in);
  //return state_in ? hood0_in != hood1_in : hood0_in * 2.6f > static_cast<float> (hood1_in);
}
