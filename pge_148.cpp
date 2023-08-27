#include "stdafx.h"

#include "pge_148.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_148::PGE_148 ()
 : olc::PixelGameEngine ()
 , t_ (0)
 , x_ (0.0f)
 , y_ (0.0f)
 , z_ (0)
 , r_ (1000.0f)
 , noise_ ()
{
  sAppName = "Example 148";
}

bool
PGE_148::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_148::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  t_ += 3;
  for (x_ = 1.0f, y_ = 1.0f, z_ = 0, r_ = 500.0f; z_++ <= 500;)
  {
    r_ *= 0.99f;
    //drawingContext.setLineDash([(a = z + t) & 16 ? 240 : 80])
    olc::PixelGameEngine::DrawCircle (half_width_i + static_cast<int32_t> (x_),
                                      half_height_i + static_cast<int32_t> (y_),
                                      static_cast<int32_t> (r_),
                                      {255, 255, 255, static_cast<uint8_t> ((z_ / 2.0f) + 5)}, 0xFF);
    float d =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((z_ + t_) / static_cast<double> (ENGINE_PGE_148_DEFAULT_W), 0.0, 0.0)),
      //Common_GL_Tools::map (static_cast<float> (noise_.Evaluate ((z_ + t_) / static_cast<double> (ENGINE_PGE_148_DEFAULT_W))),
                            -1.0f, 1.0f, 0.0f, 1.0f) * 20.0f;
    x_ += std::cos (d) * 1.5f;
    y_ += std::sin (d) * 1.5f;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
