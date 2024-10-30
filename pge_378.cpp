#include "stdafx.h"

#include "pge_378.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_378::PGE_378 ()
 : olc::PixelGameEngine ()
 , grid_ ()
 , velocity_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 378");
}

bool
PGE_378::OnUserCreate ()
{
  //olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_378_DEFAULT_W; i++)
  {
    std::vector<float> grid_row_a, velocity_row_a;

    for (int j = 0; j < ENGINE_PGE_378_DEFAULT_H; j++)
    {
      grid_row_a.push_back (0.0f);
      velocity_row_a.push_back (0.0f);
    } // end FOR

    grid_.push_back (grid_row_a);
    velocity_.push_back (velocity_row_a);
  } // end FOR

  return true;
}

bool
PGE_378::OnUserUpdate (float fElapsedTime)
{
  static int32_t screen_width_i = olc::PixelGameEngine::ScreenWidth ();
  static int32_t screen_height_i = olc::PixelGameEngine::ScreenHeight ();

  // draw grid
  for (int i = 0; i < ENGINE_PGE_378_DEFAULT_W; i++)
    for (int j = 0; j < ENGINE_PGE_378_DEFAULT_H; j++)
    {
      uint8_t color_i = 30 + static_cast<uint8_t> (120.0f * grid_[i][j]);
      olc::Pixel color (color_i, color_i, 255,
                        static_cast<uint8_t> (grid_[i][j] * 255.0f + 127.0f));
      static int ratio_w = screen_width_i / ENGINE_PGE_378_DEFAULT_W;
      static int ratio_h = screen_height_i / ENGINE_PGE_378_DEFAULT_H;
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (i * ratio_w), static_cast<int32_t> (j * ratio_h),
                                      static_cast<int32_t> (ratio_w), static_cast<int32_t> (ratio_h),
                                      color);
    } // end FOR

  // change positions
  for (int i = 0; i < ENGINE_PGE_378_DEFAULT_W; i++)
    for (int j = 0; j < ENGINE_PGE_378_DEFAULT_H; j++)
      grid_[i][j] += ENGINE_PGE_378_DEFAULT_RATE_1 * velocity_[i][j];

  // change velocities
  for (int i = 0; i < ENGINE_PGE_378_DEFAULT_W; i++)
    for (int j = 0; j < ENGINE_PGE_378_DEFAULT_H; j++)
    {
      velocity_[i][j] += -1.0f * ENGINE_PGE_378_DEFAULT_RATE_2 * grid_[i][j];
      velocity_[i][j] *= ENGINE_PGE_378_DEFAULT_FRICTION;
    } // end FOR

  // diffuse
  for (int i = 0; i < ENGINE_PGE_378_DEFAULT_W; i++)
    for (int j = 0; j < ENGINE_PGE_378_DEFAULT_H; j++)
    {
      velocity_[i][j] *= (1.0f - 9.0f * ENGINE_PGE_378_DEFAULT_RATE_3);
      for (int k = -1; k <= 1; k++)
        for (int l = -1; l <= 1; l++)
          velocity_[i][j] += ENGINE_PGE_378_DEFAULT_RATE_3 * grid_[(i + k + ENGINE_PGE_378_DEFAULT_W) % ENGINE_PGE_378_DEFAULT_W][(j + l + ENGINE_PGE_378_DEFAULT_H) % ENGINE_PGE_378_DEFAULT_H];
    } // end FOR

  // add weight ?
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    static float ratio_w_2 = ENGINE_PGE_378_DEFAULT_W / static_cast<float> (screen_width_i);
    static float ratio_h_2 = ENGINE_PGE_378_DEFAULT_H / static_cast<float> (screen_height_i);
    int xVal = static_cast<int> (olc::PixelGameEngine::GetMouseX () * ratio_w_2);
    int yVal = static_cast<int> (olc::PixelGameEngine::GetMouseY () * ratio_h_2);
    grid_[(xVal + ENGINE_PGE_378_DEFAULT_W) % ENGINE_PGE_378_DEFAULT_W][(yVal + ENGINE_PGE_378_DEFAULT_H) % ENGINE_PGE_378_DEFAULT_H] += 2.0f;
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
