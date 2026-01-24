#include "stdafx.h"

#include "pge_102.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_102::PGE_102 ()
 : olc::PixelGameEngine ()
 , cells_ (NULL)
 , next_ (NULL)
{
  sAppName = "Example 102";
}

PGE_102::~PGE_102 ()
{
  delete [] cells_;
  delete [] next_;
}

bool
PGE_102::OnUserCreate ()
{
  cells_ = new uint8_t[ENGINE_PGE_102_DEFAULT_N * ENGINE_PGE_102_DEFAULT_N];
  ACE_OS::memset (cells_, 0, sizeof (uint8_t) * ENGINE_PGE_102_DEFAULT_N * ENGINE_PGE_102_DEFAULT_N);
  next_ = new uint8_t[ENGINE_PGE_102_DEFAULT_N * ENGINE_PGE_102_DEFAULT_N];
  ACE_OS::memset (next_, 0, sizeof (uint8_t) * ENGINE_PGE_102_DEFAULT_N * ENGINE_PGE_102_DEFAULT_N);

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_102::OnUserUpdate (float fElapsedTime)
{
  brush ();

  if (olc::PixelGameEngine::GetKey(olc::Key::SPACE).bPressed)
    ACE_OS::memset (cells_, 0, sizeof(uint8_t) * ENGINE_PGE_102_DEFAULT_N * ENGINE_PGE_102_DEFAULT_N);

  ACE_OS::memcpy (next_, cells_, sizeof (uint8_t) * ENGINE_PGE_102_DEFAULT_N * ENGINE_PGE_102_DEFAULT_N);

  int32_t i, sum0, sum1, all, px, py;
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int32_t y = 1; y < ENGINE_PGE_102_DEFAULT_N - 1; y++)
    for (int32_t x = 1; x < ENGINE_PGE_102_DEFAULT_N - 1; x++)
    {
      i = y * ENGINE_PGE_102_DEFAULT_N + x;
      sum0 =
          cells_[i + 1]                             // right
        + cells_[i - 1]                             // left
        + cells_[i - ENGINE_PGE_102_DEFAULT_N]      // top
        + cells_[i + ENGINE_PGE_102_DEFAULT_N];     // bottom
      sum1 =
          cells_[i - 1 - ENGINE_PGE_102_DEFAULT_N]  // top left
        + cells_[i - 1 + ENGINE_PGE_102_DEFAULT_N]  // bottom left
        + cells_[i + 1 - ENGINE_PGE_102_DEFAULT_N]  // top right
        + cells_[i + 1 + ENGINE_PGE_102_DEFAULT_N]; // bottom right
      all = sum0 + sum1;

      next_[i] = cells_[i] ? ((all < 1 || all > 4) ? 1 : 0) : ((sum0 > (sum1 * 2)) ? 1 : 0);

      px =
        Common_GL_Tools::map (x, 0, ENGINE_PGE_102_DEFAULT_N - 1, 0, olc::PixelGameEngine::ScreenWidth () - 1);
      py =
        Common_GL_Tools::map (y, 0, ENGINE_PGE_102_DEFAULT_N - 1, 0, olc::PixelGameEngine::ScreenHeight () - 1);
      //p[px + py * olc::PixelGameEngine::ScreenWidth ()].a = next_[i] ? 0 : 255;
      p[py * olc::PixelGameEngine::ScreenWidth () + px].a = next_[i] ? 255 : 0;
    } // end FOR
  uint8_t* temp = cells_;
  cells_ = next_;
  next_ = temp;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_102::brush ()
{
  int32_t mx =
    Common_GL_Tools::map (olc::PixelGameEngine::GetMouseX (), 0, olc::PixelGameEngine::ScreenWidth (), 0, ENGINE_PGE_102_DEFAULT_N - 1);
  int32_t my =
    Common_GL_Tools::map (olc::PixelGameEngine::GetMouseY (), 0, olc::PixelGameEngine::ScreenHeight (), 0, ENGINE_PGE_102_DEFAULT_N - 1);

  for (int32_t dy = -ENGINE_PGE_102_DEFAULT_BRUSH_THICKNESS; dy <= ENGINE_PGE_102_DEFAULT_BRUSH_THICKNESS; dy++)
    for (int32_t dx = -ENGINE_PGE_102_DEFAULT_BRUSH_THICKNESS; dx <= ENGINE_PGE_102_DEFAULT_BRUSH_THICKNESS; dx++)
    {
      float d = static_cast<float> (std::sqrt (dx * dx + dy * dy));
      if (d > static_cast<float> (ENGINE_PGE_102_DEFAULT_BRUSH_THICKNESS))
        continue;
      int32_t x = mx + dx;
      int32_t y = my + dy;
      if (x < 0 || x >= ENGINE_PGE_102_DEFAULT_N ||
          y < 0 || y >= ENGINE_PGE_102_DEFAULT_N)
        continue;
      cells_[y * ENGINE_PGE_102_DEFAULT_N + x] =
        (olc::PixelGameEngine::GetMouse(0).bHeld ? (Common_Tools::getRandomNumber (0.0f, 2.0f) < 1.0f ? 1 : 0) : cells_[y * ENGINE_PGE_102_DEFAULT_N + x]);
    } // end FOR
}
