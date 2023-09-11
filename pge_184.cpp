#include "stdafx.h"

#include "pge_184.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_184::PGE_184 ()
 : olc::PixelGameEngine ()
 , m_ (0)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 184");
}
 
bool
PGE_184::OnUserCreate ()
{
  m_ = ENGINE_PGE_184_DEFAULT_W / 2;

  return true;
}

bool
PGE_184::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int32_t y = -m_; y < m_; y++)
    for (int32_t x = -m_; x < m_; x++)
    {
      olc::vf2d p (static_cast<float> (x), static_cast<float> (y));
      float n =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((x + m_ / 2.0) / static_cast<float> (m_) * 2.0, (y + frame_count_i) / static_cast<float> (m_), p.mag () * 0.02)),
                              -1.0f, 1.0f, 0.0f, 1.0f);
      n = std::abs (n - 0.5f);
      if (n < 0.03f)
      {
        olc::Pixel color = olc::BLACK;
        color.r = n < 0.01f ? 255 : 128;
        olc::PixelGameEngine::Draw (x + m_, y + m_, color);
      } // end IF
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
