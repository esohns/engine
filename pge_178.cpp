#include "stdafx.h"

#include "pge_178.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_178::PGE_178 ()
 : olc::PixelGameEngine ()
 , m_ (0)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 178");
}

bool
PGE_178::OnUserCreate ()
{
  m_ = ENGINE_PGE_178_DEFAULT_WIDTH / ENGINE_PGE_178_DEFAULT_U;

  return true;
}

bool
PGE_178::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (int32_t y = 0; y < m_; y++)
    for (int32_t x = 0; x < m_; x++)
    {
      float d = frame_count_i / static_cast<float> (m_) + x * x * y / static_cast<float> (m_) / static_cast<float> (m_) / static_cast<float> (m_);
      float c = std::cos (d);
      float s = std::sin (d + 1.0f);
      float n =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((x - frame_count_i) / static_cast<double> (m_), y / static_cast<double> (m_), 0.0)),
                              -1.0f, 1.0f, 0.0f, 1.0f);
      float r, g, b;
      Common_Image_Tools::HSVToRGB (n * 360.0f, 1.0f, 1.0f, r, g, b);
      olc::Pixel color;
      color.r = static_cast<uint8_t> (r * 255.0f);
      color.g = static_cast<uint8_t> (g * 255.0f);
      color.b = static_cast<uint8_t> (b * 255.0f);
      olc::PixelGameEngine::FillRect (static_cast<int32_t> ((x * ENGINE_PGE_178_DEFAULT_U - half_width_i) * c + half_width_i), static_cast<int32_t> ((y * ENGINE_PGE_178_DEFAULT_U - half_width_i) * s + half_width_i),
                                      ENGINE_PGE_178_DEFAULT_U, ENGINE_PGE_178_DEFAULT_U,
                                      color);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
