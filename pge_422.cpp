#include "stdafx.h"

#include "pge_422.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_19.h"

PGE_422::PGE_422 ()
 : olc::PixelGameEngine ()
 , grid_ ()
 , m_ (0)
 , mat_ ()
 , zoom_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 422");
}

bool
PGE_422::OnUserCreate ()
{
  m_ = std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ());
  zoom_ = ENGINE_PGE_422_DEFAULT_W / static_cast<float> (m_);

  grid_.assign (ENGINE_PGE_422_DEFAULT_W * ENGINE_PGE_422_DEFAULT_W, 0);
  for (int i = 0; i < ENGINE_PGE_422_DEFAULT_W * ENGINE_PGE_422_DEFAULT_W; i++)
    grid_[i] = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));

  mat_ = {1.0f, 1.0f, -5.0f};
  float sum = mat_[0] + mat_[1] * 4.0f + mat_[2] * 4.0f;
  for (std::vector<float>::iterator iterator = mat_.begin ();
       iterator != mat_.end ();
       ++iterator)
    *iterator = *iterator / sum / 9.0f;

  return true;
}

bool
PGE_422::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetKey (olc::Key::C).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::BLACK);

    for (int i = 0; i < ENGINE_PGE_422_DEFAULT_W * ENGINE_PGE_422_DEFAULT_W; i++)
      grid_[i] = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
  } // end IF
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    for (int dy = -ENGINE_PGE_422_DEFAULT_BRUSH_RADIUS; dy <= ENGINE_PGE_422_DEFAULT_BRUSH_RADIUS; dy++)
      for (int dx = -ENGINE_PGE_422_DEFAULT_BRUSH_RADIUS; dx <= ENGINE_PGE_422_DEFAULT_BRUSH_RADIUS; dx++)
      {
        if (dx * dx + dy * dy > ENGINE_PGE_422_DEFAULT_BRUSH_RADIUS * ENGINE_PGE_422_DEFAULT_BRUSH_RADIUS)
          continue;
        int x = static_cast<int> (std::floor (olc::PixelGameEngine::GetMouseX () * zoom_ + dx));
        int y = static_cast<int> (std::floor (olc::PixelGameEngine::GetMouseY () * zoom_ + dy));
        if (0 <= x && x < ENGINE_PGE_422_DEFAULT_W && 0 <= y && y < ENGINE_PGE_422_DEFAULT_W)
          grid_[x + y * ENGINE_PGE_422_DEFAULT_W] = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
      } // end FOR
  } // end IF

  std::vector<uint8_t> newGrid = grid_;

  for (int y = 1; y < ENGINE_PGE_422_DEFAULT_W - 1; y++)
    for (int x = 1; x < ENGINE_PGE_422_DEFAULT_W - 1; x++)
    {
      int i = x + y * ENGINE_PGE_422_DEFAULT_W;
      uint8_t self = grid_[i];
      int sum1 =
        grid_[i - ENGINE_PGE_422_DEFAULT_W]
      + grid_[i - 1]
      + grid_[i + 1]
      + grid_[i + ENGINE_PGE_422_DEFAULT_W];
      int sum2 =
        grid_[i - 1 - ENGINE_PGE_422_DEFAULT_W]
      + grid_[i + 1 - ENGINE_PGE_422_DEFAULT_W]
      + grid_[i - 1 + ENGINE_PGE_422_DEFAULT_W]
      + grid_[i + 1 + ENGINE_PGE_422_DEFAULT_W];
      
      float sum = 
        self * mat_[0]
      + sum1 * mat_[1]
      + sum2 * mat_[2];
      newGrid[i] = static_cast<uint8_t> (sum);

      olc::Pixel color = sum > static_cast<float> (self) ? olc::WHITE : olc::BLACK;
      olc::PixelGameEngine::Draw (x, y, color);
    } // end FOR
  grid_ = newGrid;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
