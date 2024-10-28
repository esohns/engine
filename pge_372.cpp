#include "stdafx.h"

#include "pge_372.h"

#include <vector>

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_372::PGE_372 ()
 : olc::PixelGameEngine ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 372");
}

bool
PGE_372::OnUserCreate ()
{
  return true;
}

bool
PGE_372::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::WHITE);

  std::vector<std::vector<struct rectangle> > rows_a;
  float row_sum = 0.0f;
  for (int i = 0; i < ENGINE_PGE_372_DEFAULT_ROWS_COUNT; i++)
  {
    std::vector<struct rectangle> columns_a;

    float m =
      //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (i * 99.0, frame_count_i / 400.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) *
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (i * 99.0, frame_count_i / 400.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) *
      Common_GL_Tools::map (std::sin (((i / static_cast<float> (ENGINE_PGE_372_DEFAULT_ROWS_COUNT)) * 2.0f * static_cast<float> (M_PI) * 2.0f) / 4.0f +
                                      std::sin ((static_cast<float> (M_PI) / 180.0f) * (frame_count_i / 2.0f)) *
                                      std::fmod ((static_cast<float> (M_PI) / 180.0f) * (frame_count_i / 2.0f), 2.0f * static_cast<float> (M_PI))),
                            -1.0f, 1.0f, 0.0f, 0.1f);
    row_sum += m;

    struct rectangle rectangle_s;
    for (int j = 0; j < ENGINE_PGE_372_DEFAULT_COLUMNS_COUNT; j++)
    {
      float n =
        //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (i * 99.0, j * 99.0, frame_count_i / 400.0)), -1.0f, 1.0f, 0.0f, 1.0f) *
        Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (i * 99.0, j * 99.0, frame_count_i / 400.0)), -1.0f, 1.0f, 0.0f, 1.0f) *
        Common_GL_Tools::map (std::sin (((i / static_cast<float> (ENGINE_PGE_372_DEFAULT_ROWS_COUNT)) * 2.0f * static_cast<float> (M_PI))    +
                                        ((j / static_cast<float> (ENGINE_PGE_372_DEFAULT_COLUMNS_COUNT)) * 2.0f * static_cast<float> (M_PI)) +
                                        (frame_count_i / 55.0f)),
                              -1.0f, 1.0f, 0.0f, 0.1f);
      rectangle_s.h = m;
      rectangle_s.w = n;
      columns_a.push_back (rectangle_s);
    } // end FOR

    rows_a.push_back (columns_a);
  } // end FOR

  float y = 0.0f;
  float h = 0.0f;
  for (int i = 0; i < ENGINE_PGE_372_DEFAULT_ROWS_COUNT; i++)
  {
    float col_sum = 0.0f;
    for (int j = 0; j < ENGINE_PGE_372_DEFAULT_COLUMNS_COUNT; j++)
    {
      struct rectangle& obj = rows_a[i][j];
      h = (obj.h / row_sum) * olc::PixelGameEngine::ScreenHeight ();
      col_sum += obj.w;
      obj.h = h;
      obj.position.y = y;
    } // end FOR

    float x = 0.0f;
    for (int j = 0; j < ENGINE_PGE_372_DEFAULT_COLUMNS_COUNT; j++)
    {
      struct rectangle& obj = rows_a[i][j];
      float w = (obj.w / col_sum) * olc::PixelGameEngine::ScreenWidth ();
      obj.position.x = x;
      obj.w = w;
      x += obj.w;
    } // end FOR

    y += h;
  } // end FOR

  for (std::vector<std::vector<struct rectangle> >::iterator iterator = rows_a.begin ();
       iterator != rows_a.end ();
       ++iterator)
    for (std::vector<struct rectangle>::iterator iterator_2 = (*iterator).begin ();
         iterator_2 != (*iterator).end ();
         ++iterator_2)
    {
      //olc::PixelGameEngine::DrawRect (static_cast<int32_t> ((*iterator_2).position.x),
      //                                static_cast<int32_t> ((*iterator_2).position.y),
      //                                static_cast<int32_t> ((*iterator_2).w),
      //                                static_cast<int32_t> ((*iterator_2).h),
      //                                olc::BLACK);
      olc::PixelGameEngine::DrawCircle (static_cast<int32_t> ((*iterator_2).position.x + (*iterator_2).w / 2.0f),
                                        static_cast<int32_t> ((*iterator_2).position.y + (*iterator_2).h / 2.0f),
                                        static_cast<int32_t> (std::min ((*iterator_2).w, (*iterator_2).h) / 2.0f),
                                        olc::BLACK,
                                        0xFF);
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
