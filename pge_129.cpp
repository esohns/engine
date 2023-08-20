#include "stdafx.h"

#include "pge_129.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_129::PGE_129 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , w_ (0)
{
  sAppName = "Example 129";
}

bool
PGE_129::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  olc::PixelGameEngine::Clear (olc::WHITE);

  w_ = olc::PixelGameEngine::ScreenWidth () / 2;

  return true;
}

bool
PGE_129::OnUserUpdate (float fElapsedTime)
{
  static int frame_rate_i = 1;
  static double a2r_d = M_PI / 180.0;
  static olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();

  int i = 0;
  for (int y = -w_; y < w_; y++)
    for (int x = -w_; x < w_; x++)
    {
      p[i].a =
        //static_cast<uint8_t> (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (a2r_d * static_cast<double> (x + frame_rate_i),
        //                                                                                 a2r_d * static_cast<double> (y - frame_rate_i),
        //                                                                                 std::sin (a2r_d * (frame_rate_i * 0.4)) * a2r_d * static_cast<double> (x * x + y * y + x * y + frame_rate_i) * 0.1)),
        //                                            -1.0f, 1.0f, 0.0f, 255.0f));
        static_cast<uint8_t> (Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (a2r_d * static_cast<double> (x + frame_rate_i),
                                                                                         a2r_d * static_cast<double> (y - frame_rate_i),
                                                                                         std::sin (a2r_d * (frame_rate_i * 0.4)) * a2r_d * static_cast<double> (x * x + y * y + x * y + frame_rate_i) * 0.1)),
                                                    -1.0f, 1.0f, 0.0f, 255.0f));
      ++i;
    } // end FOR

  ++frame_rate_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
