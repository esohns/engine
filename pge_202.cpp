#include "stdafx.h"

#include "pge_202.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_202::PGE_202 ()
 : olc::PixelGameEngine ()
 , color_ (olc::BLACK)
 , noise_ ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 202");
}

bool
PGE_202::OnUserCreate ()
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_202::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;
  if (frame_count_i % 60)
    goto continue_;

  //t_ += 0.0015f;
  t_ += 0.003f;

  for (int32_t Y = 4; Y--;)
    for (int32_t X = 4; X--;)
      for (int32_t y = 18; y--;)
        for (int32_t x = 18; x--;)
        {
          olc::PixelGameEngine::FillCircle (110 + X * 26 * 6 + x * 6, 12 + Y * 26 * 6 + y * 6, 3, color_);
          olc::vf2d position (static_cast<float> (x - 9), static_cast<float> (y - 9));
          float mag_f = position.mag ();
          int32_t i = 2;
          color_.b =
            //static_cast<uint8_t> (255.0f * std::sin (t_ - std::pow (mag_f, i + 2.0f * Common_GL_Tools::map (static_cast<float> (noise_.GetValue (static_cast<double> (t_ + X), static_cast<double> (Y), 0.0)),
            static_cast<uint8_t> (255.0f * std::sin (t_ - std::pow (mag_f, i + 2.0f * Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (t_ + X), static_cast<double> (Y), 0.0)),
                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f))));
          --i;
          color_.g =
            //static_cast<uint8_t> (255.0f * std::sin (t_ - std::pow (mag_f, i + 2.0f * Common_GL_Tools::map (static_cast<float> (noise_.GetValue (static_cast<double> (X), static_cast<double> (t_ - Y), 0.0)),
            static_cast<uint8_t> (255.0f * std::sin (t_ - std::pow (mag_f, i + 2.0f * Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (X), static_cast<double> (t_ - Y), 0.0)),
                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f))));
          --i;
          color_.r =
            //static_cast<uint8_t> (255.0f * std::sin (t_ - std::pow (mag_f, i + 2.0f * Common_GL_Tools::map (static_cast<float> (noise_.GetValue (static_cast<double> (t_ - X), static_cast<double> (t_ + Y), 0.0)),
            static_cast<uint8_t> (255.0f * std::sin (t_ - std::pow (mag_f, i + 2.0f * Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (t_ - X), static_cast<double> (t_ + Y), 0.0)),
                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f))));
        } // end FOR

continue_:
  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
