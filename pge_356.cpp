#include "stdafx.h"

#include "pge_356.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_16.h"

PGE_356::PGE_356 ()
 : olc::PixelGameEngine ()
 , c_ (2)
 , y_ (0.0f)
 , max_iterations_ (ENGINE_PGE_356_DEFAULT_MAX_ITERATIONS)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 356");
}

bool
PGE_356::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  olc::PixelGameEngine::Clear (olc::BLACK);

  return true;
}

bool
PGE_356::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_356_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_356_DEFAULT_ALPHA_DECAY : 0);

  for (int s = 0; s < 5; s++)
  {
    for (float x = 0.0f; x < static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE); x += Common_Tools::getRandomNumber (0.2f, 0.21f))
    {
      float cre =
        Common_GL_Tools::map (x, 0.0f, static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE), ENGINE_PGE_356_DEFAULT_VWX1, ENGINE_PGE_356_DEFAULT_VWX2);
      float cim =
        Common_GL_Tools::map (y_, 0.0f, static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE), ENGINE_PGE_356_DEFAULT_VWY2, ENGINE_PGE_356_DEFAULT_VWY1);
      if (isNotBounded (cre, cim, max_iterations_))
      {
        float pzre = cre;
        float pzim = cim;
        for (int i = 0; i < max_iterations_; i++)
        {
          float zre = pzre * pzre - std::abs (pzim) * pzim + cre;
          float zim = std::abs (pzre) * pzim * -2.0f + cim;
          if (ENGINE_PGE_356_DEFAULT_VWX1 < zre && ENGINE_PGE_356_DEFAULT_VWX2 > zre && ENGINE_PGE_356_DEFAULT_VWY1 < zim && ENGINE_PGE_356_DEFAULT_VWY2 > zim)
          {
            int32_t mappedX =
              static_cast<int32_t> (std::round (Common_GL_Tools::map (zre, ENGINE_PGE_356_DEFAULT_VWX1, ENGINE_PGE_356_DEFAULT_VWX2, 0.0f, static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE - 1))));
            int32_t mappedY =
              static_cast<int32_t> (std::round (Common_GL_Tools::map (zim, ENGINE_PGE_356_DEFAULT_VWY1, ENGINE_PGE_356_DEFAULT_VWY2, 0.0f, static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE - 1))));
            int32_t index_i = (mappedY * ENGINE_PGE_356_DEFAULT_SIZE + mappedX);
            int32_t amount_i = static_cast<uint8_t> (100.0f / static_cast<float> (max_iterations_));
            p[index_i].r = (static_cast<int> (p[index_i].r) + amount_i <= 255 ? p[index_i].r += amount_i : 255);
            p[index_i].g = (static_cast<int> (p[index_i].g) + amount_i <= 255 ? p[index_i].g += amount_i : 255);
            p[index_i].b = (static_cast<int> (p[index_i].b) + amount_i <= 255 ? p[index_i].b += amount_i : 255);
            p[index_i].a = (static_cast<int> (p[index_i].a) + amount_i <= 255 ? p[index_i].a += amount_i : 255);
          } // end IF
          pzre = zre;
          pzim = zim;
        } // end FOR
      } // end IF
    } // end FOR

    y_ += Common_Tools::getRandomNumber (0.2f, 0.21f);
    if (y_ > static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE) && c_ > 0)
    {
      y_ = 0.0f;
      max_iterations_ *= 2;
      c_--;
    } // end IF
    else if (y_ > static_cast<float> (ENGINE_PGE_356_DEFAULT_SIZE))
     break;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

bool
PGE_356::isNotBounded (float cx, float cy, int iterations)
{
  float zx = cx, zy = cy;
  for (int i = 0; i < iterations; i++)
  {
    float temp1 = zx * zx - std::abs (zy) * zy + cx;
    float temp2 = std::abs (zx) * zy * -2.0f + cy;
    zx = temp1;
    zy = temp2;
    if (zx * zx + zy * zy >= 4.0f)
      return true;
  } // end FOR

  return false;
}
