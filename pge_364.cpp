#include "stdafx.h"

#include "pge_364.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_17.h"

PGE_364::PGE_364 ()
 : olc::PixelGameEngine ()
 , minSize_ (0.0f)
 , maxSize_ (0.0f)
 , hueFactor_ (360.0f / static_cast<float> (ENGINE_PGE_364_DEFAULT_ITERATIONS))
 , theta_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 364");
}

bool
PGE_364::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  minSize_ = olc::PixelGameEngine::ScreenWidth () / 15.0f;
  maxSize_ = olc::PixelGameEngine::ScreenWidth () / 10.0f;

  return true;
}

bool
PGE_364::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_364_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_364_DEFAULT_ALPHA_DECAY : 0);

  float r, g, b;//, alpha;
  olc::Pixel color;
  for (int i = 0; i < ENGINE_PGE_364_DEFAULT_ITERATIONS; i++)
  {
    Common_Image_Tools::HSVToRGB (i * hueFactor_, 0.9f, 1.0f, r, g, b);
    //alpha = (255.0f / static_cast<float> (ENGINE_PGE_364_DEFAULT_ITERATIONS)) * i;
    color =
      {static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), static_cast<uint8_t> (0.25f * 255.0f)};

    float offSet =
      ((2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_364_DEFAULT_ITERATIONS)) * i;
    float x =
      Common_GL_Tools::map (std::sin (theta_ - offSet), -1.0f, 1.0f, olc::PixelGameEngine::ScreenWidth () * 0.2f, olc::PixelGameEngine::ScreenWidth () * 0.8f);
    float y =
      Common_GL_Tools::map (std::sin (theta_ + offSet), -1.0f, 1.0f, olc::PixelGameEngine::ScreenHeight () * 0.2f, olc::PixelGameEngine::ScreenHeight () * 0.8f);
    float sz =
      Common_GL_Tools::map (std::sin (theta_ + offSet * 3.0f), -1.0f, 1.0f, minSize_, maxSize_);
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y), static_cast<int32_t> (sz),
                                      color);
  } // end FOR

  theta_ += (2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_364_DEFAULT_FRAMES_PER_TURN);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
