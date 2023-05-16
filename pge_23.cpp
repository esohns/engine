#include "stdafx.h"

#include "pge_23.h"

#include "MagickWand/MagickWand.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

//////////////////////////////////////////

PGE_23::PGE_23 ()
 : olc::PixelGameEngine ()
 , grid_ ()
 , next_ ()
{
  sAppName = "Example 23";
}

bool
PGE_23::OnUserCreate ()
{
  for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); ++y)
    for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); ++x)
    {
      grid_[x][y] = { 1.0f, 0.0f };
      next_[x][y] = { 1.0f, 0.0f };
    } // end FOR

  // initialize grid
  for (int y = olc::PixelGameEngine::ScreenHeight () / 2 - 5; y < olc::PixelGameEngine::ScreenHeight () / 2 + 5; ++y)
    for (int x = olc::PixelGameEngine::ScreenWidth () / 2 - 5; x < olc::PixelGameEngine::ScreenWidth () / 2 + 5; ++x)
      grid_[x][y].b = 1.0f;

  return true;
}

bool
PGE_23::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int y = 1; y < olc::PixelGameEngine::ScreenHeight () - 1; ++y)
    for (int x = 1; x < olc::PixelGameEngine::ScreenWidth () - 1; ++x)
    {
      next_[x][y].a = grid_[x][y].a +
        (dA_ * laplaceA (x, y)) -
        (grid_[x][y].a * grid_[x][y].b * grid_[x][y].b) +
        (feed_ * (1.0f - grid_[x][y].a));
      next_[x][y].b = grid_[x][y].b +
        (dB_ * laplaceB (x, y)) +
        (grid_[x][y].a * grid_[x][y].b * grid_[x][y].b) -
        ((k_ + feed_) * grid_[x][y].b);

      next_[x][y].a = (next_[x][y].a < 0.0f ? 0.0f 
                                            : next_[x][y].a > 1.0f ? 1.0f : next_[x][y].a);
      next_[x][y].b = (next_[x][y].b < 0.0f ? 0.0f 
                                            : next_[x][y].b > 1.0f ? 1.0f : next_[x][y].b);
    } // end FOR

  for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); ++y)
    for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); ++x)
    {
      int32_t c =
        static_cast<int32_t> (std::floor ((next_[x][y].a - next_[x][y].b) * 255.0f));
      c = c < 0 ? 0 : c > 255 ? 255 : c;
      olc::PixelGameEngine::Draw ({x, y}, {static_cast<uint8_t> (c), static_cast<uint8_t> (c), static_cast<uint8_t> (c), 255});
    } // end FOR

  // swap grids
  grid_t temp = grid_;
  grid_ = next_;
  next_ = grid_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_23::laplaceA (int x,
                  int y)
{
  float sum = 0.0f;
  sum += grid_[x][y].a * -1.0f;
  sum += grid_[x - 1][y].a * 0.2f;
  sum += grid_[x + 1][y].a * 0.2f;
  sum += grid_[x][y + 1].a * 0.2f;
  sum += grid_[x][y - 1].a * 0.2f;
  sum += grid_[x - 1][y - 1].a * 0.05f;
  sum += grid_[x + 1][y - 1].a * 0.05f;
  sum += grid_[x + 1][y + 1].a * 0.05f;
  sum += grid_[x - 1][y + 1].a * 0.05f;
  return sum;
}

float
PGE_23::laplaceB (int x,
                  int y)
{
  float sum = 0.0f;
  sum += grid_[x][y].b * -1.0f;
  sum += grid_[x - 1][y].b * 0.2f;
  sum += grid_[x + 1][y].b * 0.2f;
  sum += grid_[x][y + 1].b * 0.2f;
  sum += grid_[x][y - 1].b * 0.2f;
  sum += grid_[x - 1][y - 1].b * 0.05f;
  sum += grid_[x + 1][y - 1].b * 0.05f;
  sum += grid_[x + 1][y + 1].b * 0.05f;
  sum += grid_[x - 1][y + 1].b * 0.05f;
  return sum;
}
