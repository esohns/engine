#include "stdafx.h"

#include "pge_196.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_196::PGE_196 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , t_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 196");
}

bool
PGE_196::OnUserCreate ()
{
  return true;
}

float
PGE_196::mesh (float x, float y, int n)
{
  return (n ? mesh (static_cast<float> (Common_GL_Tools::map (noise_.GetValue (x / 3.0, y / 3.0 + t_ / 3.0, 0.0),
  //return (n ? mesh (static_cast<float> (Common_GL_Tools::map (noise_.Evaluate (x / 3.0, y / 3.0 + t_ / 3.0, 0.0),
                                                              -1.0, 1.0, 0.0, 1.0)) * x * y,
                    std::sin (x + t_) + std::sin (y),
                    n - 1)
            : (x - y) * (x - y) * 128.0f);
}

bool
PGE_196::OnUserUpdate (float fElapsedTime)
{
  t_ += 0.1f;

  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int y = ENGINE_PGE_196_DEFAULT_SIZE; y--;)
    for (int x = ENGINE_PGE_196_DEFAULT_SIZE; x--;)
    {
      float c =
        mesh (Common_GL_Tools::map (static_cast<float> (x), 0.0f, static_cast<float> (ENGINE_PGE_196_DEFAULT_SIZE), -5.0f, 5.0f),
              Common_GL_Tools::map (static_cast<float> (y), 0.0f, static_cast<float> (ENGINE_PGE_196_DEFAULT_SIZE), -5.0f, 5.0f));
      float r = c * 1.0f;
      float g = c * 1.2f;
      float b = c * 1.3f;
      olc::Pixel color = { r > 255.0f ? static_cast<uint8_t> (0) : static_cast<uint8_t> (255 - r),
                           g > 255.0f ? static_cast<uint8_t> (0) : static_cast<uint8_t> (255 - g),
                           b > 255.0f ? static_cast<uint8_t> (0) : static_cast<uint8_t> (255 - b),
                           255 };
      p[x + y * olc::PixelGameEngine::ScreenWidth ()] = color;
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
