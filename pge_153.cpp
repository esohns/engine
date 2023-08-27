#include "stdafx.h"

#include "pge_153.h"

#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_153::PGE_153 ()
 : olc::PixelGameEngine ()
 , agents_ (ENGINE_PGE_153_DEFAULT_NUMBER_OF_AGENTS, {0.0f, 0.0f})
 , grid_ (NULL)
 , noise_ ()
{
  sAppName = "Example 153";
}

PGE_153::~PGE_153 ()
{
  delete [] grid_;
}

bool
PGE_153::OnUserCreate ()
{
  grid_ = new uint8_t[ENGINE_PGE_153_DEFAULT_N * ENGINE_PGE_153_DEFAULT_N];
  ACE_OS::memset (grid_, 0, sizeof (uint8_t) * ENGINE_PGE_153_DEFAULT_N * ENGINE_PGE_153_DEFAULT_N);

  return true;
}

bool
PGE_153::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    ACE_OS::memset (grid_, 0, sizeof (uint8_t) * ENGINE_PGE_153_DEFAULT_N * ENGINE_PGE_153_DEFAULT_N);

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = static_cast<int> (agents_.size ()); i--;)
  {
    float r =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (i + frame_count_i / 99.0, 0.0, 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 20.0f;
    agents_[i].x = std::fmod ((agents_[i].x + std::cos (r) + ENGINE_PGE_153_DEFAULT_N), static_cast<float> (ENGINE_PGE_153_DEFAULT_N));
    agents_[i].y = std::fmod ((agents_[i].y + std::sin (r) + 1 + ENGINE_PGE_153_DEFAULT_N), static_cast<float> (ENGINE_PGE_153_DEFAULT_N));
    grid_[int(agents_[i].y) * ENGINE_PGE_153_DEFAULT_N + int(agents_[i].x)] = 2;
  } // end FOR

  uint8_t* old = new uint8_t[ENGINE_PGE_153_DEFAULT_N * ENGINE_PGE_153_DEFAULT_N];
  ACE_OS::memcpy (old, grid_, sizeof (uint8_t) * ENGINE_PGE_153_DEFAULT_N * ENGINE_PGE_153_DEFAULT_N);
  for (int y = 1; y < ENGINE_PGE_153_DEFAULT_N - 1; y++)
    for (int x = 1; x < ENGINE_PGE_153_DEFAULT_N - 1; x++)
    {
      int sum =
          old[(y - 1) * ENGINE_PGE_153_DEFAULT_N + x] // up
        + old[(y + 1) * ENGINE_PGE_153_DEFAULT_N + x] // down
        + old[y * ENGINE_PGE_153_DEFAULT_N + x] * 2   // self
        + old[y * ENGINE_PGE_153_DEFAULT_N + x + 1]   // right
        + old[y * ENGINE_PGE_153_DEFAULT_N + x - 1];  // left
      grid_[y * ENGINE_PGE_153_DEFAULT_N + x] = (sum == 2) + (sum == 4);
    } // end FOR
  delete [] old; old = NULL;

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int x = ENGINE_PGE_153_DEFAULT_N; x--;)
    for (int y = ENGINE_PGE_153_DEFAULT_N; y--;)
    {
      if (grid_[y * ENGINE_PGE_153_DEFAULT_N + x] == 0)
        continue;
      p[y * olc::PixelGameEngine::ScreenWidth () + x] = olc::WHITE;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
