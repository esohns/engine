#include "stdafx.h"

#include "pge_211.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_211::PGE_211 ()
 : olc::PixelGameEngine ()
 , rcOffset_ (0.0)
 , rmOffset_ (0.0)
 , rdOffset_ (0.0)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 211");
}

bool
PGE_211::OnUserCreate ()
{
  reset ();

  return true;
}

bool
PGE_211::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;
  
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::WHITE);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    reset ();

  float rc_f =
    //3.0f + 6.0f * static_cast<float> (Common_GL_Tools::map (noise_.GetValue (rcOffset_ + (frame_count_i * 0.0005), 0.0, 0.0), -1.0, 1.0, 0.0, 1.0));
    3.0f + 6.0f * static_cast<float> (Common_GL_Tools::map (noise_.Evaluate (rcOffset_ + (frame_count_i * 0.0005), 0.0), -1.0, 1.0, 0.0, 1.0));
  float rm_f =
    //1.0f + 6.0f * static_cast<float> (Common_GL_Tools::map (noise_.GetValue (rmOffset_ + (frame_count_i * 0.0005), 0.0, 0.0), -1.0, 1.0, 0.0, 1.0));
    1.0f + 6.0f * static_cast<float> (Common_GL_Tools::map (noise_.Evaluate (rmOffset_ + (frame_count_i * 0.0005), 0.0), -1.0, 1.0, 0.0, 1.0));
  float rd_f =
    //3.0f + 6.0f * static_cast<float> (Common_GL_Tools::map (noise_.GetValue (rdOffset_ + (frame_count_i * 0.01)  , 0.0, 0.0), -1.0, 1.0, 0.0, 1.0));
    3.0f + 6.0f * static_cast<float> (Common_GL_Tools::map (noise_.Evaluate (rdOffset_ + (frame_count_i * 0.01)  , 0.0), -1.0, 1.0, 0.0, 1.0));
  float d = rc_f - rm_f;

  bool is_first_b = true;
  olc::vf2d prev_s;
  //olc::vf2d first_s;
  for (int i = 0, len = 360 * 30; i < len; i += 1)
  {
    float t = -static_cast<float> (M_PI_4) + (2.0f * static_cast<float> (M_PI)) * i / 360.0f;
    olc::vf2d position_s (d * std::cos (t) + rd_f * std::cos (t * d / rm_f),
                          d * std::sin (t) - rd_f * std::sin (t * d / rm_f));
    position_s *= 30.0f;
    if (is_first_b)
    {
      is_first_b = false;
      //first_s = position_s;
      prev_s = position_s;
    } // end IF

    olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
                                    half_width_i + static_cast<int32_t> (position_s.x), half_height_i + static_cast<int32_t> (position_s.y),
                                    olc::BLACK, 0xFFFFFFFF);

    prev_s = position_s;
  } // end FOR
  //olc::PixelGameEngine::DrawLine (half_width_i + static_cast<int32_t> (prev_s.x), half_height_i + static_cast<int32_t> (prev_s.y),
  //                                half_width_i + static_cast<int32_t> (first_s.x), half_height_i + static_cast<int32_t> (first_s.y),
  //                                olc::BLACK, 0xFFFFFFFF);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_211::reset ()
{
  rcOffset_ = Common_Tools::getRandomNumber (0.0, 1000.0);
  rmOffset_ = Common_Tools::getRandomNumber (0.0, 1000.0);
  rdOffset_ = Common_Tools::getRandomNumber (0.0, 1000.0);
}
