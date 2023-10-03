#include "stdafx.h"

#include "pge_192.h"

#include <algorithm>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_192::PGE_192 ()
 : olc::PixelGameEngine ()
 , m_ (0)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 192");
}

bool
PGE_192::OnUserCreate ()
{
  return true;
}

bool
PGE_192::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();

  for (int32_t x = 0; ++x < ENGINE_PGE_192_DEFAULT_W * ENGINE_PGE_192_DEFAULT_W;)
  {
    int32_t X =
      std::max (std::min ((x % ENGINE_PGE_192_DEFAULT_W) + noise (x, frame_count_i, m_), ENGINE_PGE_192_DEFAULT_W - 1), 0);
    int32_t Y =
      std::max (std::min ((x / ENGINE_PGE_192_DEFAULT_W) + noise (x, frame_count_i, m_), ENGINE_PGE_192_DEFAULT_W - 1), 0);

    uint8_t c = 50 + std::min (p[X + Y * ENGINE_PGE_192_DEFAULT_W].g, static_cast<uint8_t> (155));
    p[X + Y * ENGINE_PGE_192_DEFAULT_W] =
      {c, c, static_cast<uint8_t> (std::min (c * 2, 255)), 255};
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

int
PGE_192::noise (int32_t x, int32_t frameCount, int32_t& m_inout)
{
  static double half_size_d = ENGINE_PGE_192_DEFAULT_W / 2.0;

  ++m_inout;
  //return static_cast<int> (Common_GL_Tools::map (std::min (std::max (noise_.GetValue (x % ENGINE_PGE_192_DEFAULT_W / 99.0,
  return static_cast<int> (Common_GL_Tools::map (std::min (std::max (noise_.Evaluate (x % ENGINE_PGE_192_DEFAULT_W / 99.0,
                                                                                      x / ENGINE_PGE_192_DEFAULT_W / 99.0,
                                                                                      frameCount / 99.0 + (m_inout % 2)), -1.0), 1.0),
                                                 -1.0, 1.0, -half_size_d, half_size_d));
}
