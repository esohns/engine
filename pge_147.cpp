#include "stdafx.h"

#include "pge_147.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_147::PGE_147 ()
 : olc::PixelGameEngine ()
 , t_ (0.0f)
 , m_ (0.0f)
 , noise_ ()
{
  sAppName = "Example 147";
}

bool
PGE_147::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_147::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  m_ = static_cast<float> (1.0f - std::pow (std::sin (t_), 88));
  t_ += 0.01f;

  olc::PixelGameEngine::Clear (olc::WHITE);

  tree (static_cast<float> (half_width_i), static_cast<float> (half_height_i), 0.0f);
  tree (static_cast<float> (half_width_i), static_cast<float> (half_height_i), static_cast<float> (M_PI));

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_147::tree (float x, float y, float r, int32_t step, int32_t l)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;

  if (step > 0)
  {
    float d = 19.0f - (step / 2.0f);
    //strokeWeight (step--);
    step--;
    x = ((x - half_width_i) * m_) + half_width_i;
    float n =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((x / static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) + t_,
                                                                 (y / static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) - t_,
                                                                 0.0)),
                            -1.0f, 1.0f, 0.0f, 1.0f) * d;

    float x_offset_f = std::cos (r) * l + std::cos (n) * d;
    float y_offset_f = std::sin (r) * l + std::sin (n) * d;
    int32_t x1 = static_cast<int32_t> (x < 0.0f ? 0 : x > olc::PixelGameEngine::ScreenWidth () - 1 ? olc::PixelGameEngine::ScreenWidth () - 1 : x);
    int32_t y1 = static_cast<int32_t> (y < 0.0f ? 0 : y > olc::PixelGameEngine::ScreenWidth () - 1 ? olc::PixelGameEngine::ScreenWidth () - 1 : y);
    int32_t x2 =
      static_cast<int32_t> (x + x_offset_f < 0.0f ? 0 : x + x_offset_f > olc::PixelGameEngine::ScreenWidth () - 1 ? olc::PixelGameEngine::ScreenWidth () - 1 : x + x_offset_f);
    int32_t y2 =
      static_cast<int32_t> (y + y_offset_f < 0.0f ? 0 : y + y_offset_f > olc::PixelGameEngine::ScreenWidth () - 1 ? olc::PixelGameEngine::ScreenWidth () - 1 : y + y_offset_f);

    olc::PixelGameEngine::DrawLine (x1, y1, x2, y2, olc::BLACK, 0xFFFFFFFF);
    x += x_offset_f;
    y += y_offset_f;

    tree (x, y, r + 0.7f, step, static_cast<int32_t> (l * 0.83f));
    tree (x, y, r - 0.7f, step, static_cast<int32_t> (l * 0.83f));
  } // end IF
}
