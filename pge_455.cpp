#include "stdafx.h"

#include "pge_455.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_21.h"

PGE_455::PGE_455 ()
 : olc::PixelGameEngine ()
 , grid_ (NULL)
 , zoom_ (0.0f)
{
  sAppName = "Example 455";
}

PGE_455::~PGE_455 ()
{
  delete [] grid_;
}

bool
PGE_455::OnUserCreate ()
{
  grid_ = new uint8_t[ENGINE_PGE_455_DEFAULT_W * ENGINE_PGE_455_DEFAULT_W];
  int32_t m = std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ());
  zoom_ = ENGINE_PGE_455_DEFAULT_W / static_cast<float> (m);

  for (int i = 0; i < ENGINE_PGE_455_DEFAULT_W * ENGINE_PGE_455_DEFAULT_W; i++)
    grid_[i] = Common_Tools::getRandomNumber (0, 255);

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_455::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    for (int dy = -ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS; dy <= ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS; dy++)
      for (int dx = -ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS; dx <= ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS; dx++)
      {
        if (dx * dx + dy * dy > ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS * ENGINE_PGE_455_DEFAULT_BRUSH_RADIUS) continue;
        int x = static_cast<int> (std::floor (olc::PixelGameEngine::GetMouseX () * zoom_ + dx));
        int y = static_cast<int> (std::floor (olc::PixelGameEngine::GetMouseY () * zoom_ + dy));
        if (0 <= x && x < ENGINE_PGE_455_DEFAULT_W && 0 <= y && y < ENGINE_PGE_455_DEFAULT_W)
          grid_[x + y * ENGINE_PGE_455_DEFAULT_W] = Common_Tools::getRandomNumber (0, 255);
      } // end FOR
  } // end IF

  uint8_t* newGrid = new uint8_t[ENGINE_PGE_455_DEFAULT_W * ENGINE_PGE_455_DEFAULT_W];
  ACE_OS::memcpy (newGrid, grid_, sizeof (uint8_t) * ENGINE_PGE_455_DEFAULT_W * ENGINE_PGE_455_DEFAULT_W);

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int y = 1; y < ENGINE_PGE_455_DEFAULT_W - 1; y++)
    for (int x = 1; x < ENGINE_PGE_455_DEFAULT_W - 1; x++)
    {
      int i = x + y * ENGINE_PGE_455_DEFAULT_W;
      uint8_t oldValue = grid_[i];
      int sum =   grid_[i - ENGINE_PGE_455_DEFAULT_W]
                + grid_[i - 1]
                + grid_[i + 1]
                + grid_[i + ENGINE_PGE_455_DEFAULT_W]
                + grid_[i - 1 - ENGINE_PGE_455_DEFAULT_W]
                + grid_[i + 1 - ENGINE_PGE_455_DEFAULT_W]
                + grid_[i - 1 + ENGINE_PGE_455_DEFAULT_W]
                + grid_[i + 1 + ENGINE_PGE_455_DEFAULT_W];
      int average = static_cast<int> (std::floor (sum / 8.0f));
      uint8_t newValue = (average + 1) & 0xff;
      newGrid[i] = newValue;
      olc::Pixel color =
        std::sin (1.0f + newValue / 4.0f) > std::sin (oldValue / 4.0f) ? olc::WHITE : olc::BLACK;
      p[i] = color;
    } // end FOR

  delete [] grid_;
  grid_ = newGrid;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
