#include "stdafx.h"

#include "pge_424.h"

#include <vector>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_19.h"

PGE_424::PGE_424 ()
 : olc::PixelGameEngine ()
 , u_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 424");
}

bool
PGE_424::OnUserCreate ()
{
  u_ = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_424_DEFAULT_N;

  return true;
}

bool
PGE_424::OnUserUpdate (float fElapsedTime)
{
  static bool render_b = true;
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::BLACK);
    render_b = true;
  } // end IF
  if (!render_b)
    return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;

  static olc::Pixel checkerboard_colors_a[2] = { olc::Pixel (0xfc, 0xf7, 0xed), olc::Pixel (0x03, 0x08, 0x12) };
  for (int i = 0; i < ENGINE_PGE_424_DEFAULT_N + 1; i++)
    for (int j = 0; j < ENGINE_PGE_424_DEFAULT_N + 1; j++)
      olc::PixelGameEngine::FillRect (static_cast<int32_t> ((i - 1 / 2.0f) * u_), static_cast<int32_t> ((j - 1 / 2.0f) * u_), u_, u_, checkerboard_colors_a[(i + j) % 2]);

  static olc::Pixel palette_a[5] = { olc::Pixel (0x2b, 0x67, 0xaf), olc::Pixel (0xf9, 0xd5, 0x31), olc::Pixel (0xef, 0x56, 0x2f),
                                     olc::Pixel (0x29, 0xac, 0x9f), olc::Pixel (0xf5, 0x89, 0xa3) };
  static std::vector<olc::Pixel> row_a (ENGINE_PGE_424_DEFAULT_N, olc::BLACK);
  std::vector<std::vector<olc::Pixel> > color_grid_a (ENGINE_PGE_424_DEFAULT_N, row_a);
  for (int i = 0; i < ENGINE_PGE_424_DEFAULT_N - 2; i++)
  {
    int32_t x = static_cast<int32_t> ((i + 3 / 2.0f) * u_);
    for (int j = 0; j < ENGINE_PGE_424_DEFAULT_N - 2; j++)
    {
      int32_t y = static_cast<int32_t> ((j + 3 / 2.0f) * u_);

      std::vector<olc::Pixel> neighbor_colors_a;
      if (i > 0)
        neighbor_colors_a.push_back (color_grid_a[i - 1][j]);
      if (j > 0)
        neighbor_colors_a.push_back (color_grid_a[i][j - 1]);
      olc::Pixel col;
      bool found_b;
      do
      {
        col = palette_a[Common_Tools::getRandomNumber (0, 4)];

        found_b = false;
        for (std::vector<olc::Pixel>::iterator iterator = neighbor_colors_a.begin ();
             iterator != neighbor_colors_a.end ();
             ++iterator)
          if (col == *iterator)
          {
            found_b = true;
            break;
          } // end IF
      } while (found_b);
      color_grid_a[i][j] = col;

      olc::PixelGameEngine::FillCircle (x, y, u_ / 2, col);
    } // end FOR
  } // end FOR

  render_b = false;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
