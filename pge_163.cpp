#include "stdafx.h"

#include "pge_163.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_7.h"
#include "engine_common.h"

PGE_163::PGE_163 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 163";
}

bool
PGE_163::OnUserCreate ()
{
  return true;
}

bool
PGE_163::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float r1 = 0.0f;
  float r2 = 0.0f;
  float t1 = Common_GL_Tools::map (std::cos ((static_cast<float> (M_PI) / 180.0f) * (frame_count_i)), -1.0f, 1.0f, 0.03f, 0.2f);
  float t2 = 1.0f;
  while (r1 < 360.0f)
  {
    float a = sin (r2) * half_height_i;
    olc::PixelGameEngine::Draw (half_width_i  + static_cast<int32_t> (std::cos (r1) * a),
                                half_height_i + static_cast<int32_t> (std::sin (r1) * a),
                                olc::WHITE);
    r1 += t1;
    r2 += t2;
  } // end WHILE

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
