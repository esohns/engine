#include "stdafx.h"

#include "pge_265.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_265::PGE_265 ()
 : olc::PixelGameEngine ()
 , cache_ ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 265");
}

bool
PGE_265::OnUserCreate ()
{
  return true;
}

bool
PGE_265::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float t = frame_count_i / 100.0f;
  int counter_i = 0;

  for (int y = ENGINE_PGE_265_DEFAULT_BORDER; y < olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_265_DEFAULT_BORDER; y += ENGINE_PGE_265_DEFAULT_SPACE)
    for (int x = ENGINE_PGE_265_DEFAULT_BORDER; x < olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_265_DEFAULT_BORDER; x += ENGINE_PGE_265_DEFAULT_SPACE)
    {
      float n =
        Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (x / 100.0 * ENGINE_PGE_265_DEFAULT_FREQUENCY + t, y / 100.0 * ENGINE_PGE_265_DEFAULT_FREQUENCY, t * 2)), -1.0f, 1.0f, 0.0f, 1.0f) + 0.14f;
      //float n2 = n;//noise(x/300*freq + t, y/400*freq, t*2)+0.14;

      if (counter_i == static_cast<int> (cache_.size ()))
      {
        struct cache_node cache_node_s;
        cache_node_s.color = olc::WHITE;
        cache_node_s.nx =
          (Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (x), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * 30.0f;
        cache_node_s.ny =
          (Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<double> (y), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * 30.0f;
        cache_.push_back (cache_node_s);
      } // end IF

      float a = n * 2.0f * static_cast<float> (M_PI);
      float xxx = x + cache_[counter_i].ny;
      float yyy = y + cache_[counter_i].nx;

      olc::vf2d mouse_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                         static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
      float d =
        (xxx - mouse_s.x) * (xxx - mouse_s.x) + (yyy - mouse_s.y) * (yyy - mouse_s.y);
      float l = static_cast<float> (ENGINE_PGE_265_DEFAULT_L);
      //if(d < interact*interact){
      l *=
        (1.0f - std::sqrt (d) / static_cast<float> (ENGINE_PGE_265_DEFAULT_INTERACT)) * 3.0f;
      // var a2 = atan2(mouseY-yyy, mouseX-xxx) + PI;
      // var d2 = (100-sqrt(d));
      // xxx = cos(a2) * d2 + mouseX;
      // yyy = sin(a2) * d2 + mouseY;
    //}

      float xx = std::cos (a) * l + xxx;
      float yy = std::sin (a) * l + yyy;

      olc::PixelGameEngine::DrawLine (static_cast<int32_t> (xxx), static_cast<int32_t> (yyy),
                                      static_cast<int32_t> (xx), static_cast<int32_t> (yy),
                                      cache_[counter_i].color, 0xFFFFFFFF);

      counter_i++;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
