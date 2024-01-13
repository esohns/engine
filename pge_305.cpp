#include "stdafx.h"

#include "pge_305.h"

#include <random>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_305::PGE_305 ()
 : olc::PixelGameEngine ()
 , noise_ ()
 , noiseCoeffX_ ()
 , noiseCoeffY_ ()
 , particles_ ()
 , scaleFactor_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 305");
}

bool
PGE_305::OnUserCreate ()
{
  float r = ENGINE_PGE_305_DEFAULT_W / 1.8f;
  for (int i = 0; i < ENGINE_PGE_305_DEFAULT_N; i++)
  {
    float theta = 5.0f * 2.0f * static_cast<float> (M_PI) * i / static_cast<float> (ENGINE_PGE_305_DEFAULT_N);
    float ra = r * i / static_cast<float> (ENGINE_PGE_305_DEFAULT_N);
    float x1 = ENGINE_PGE_305_DEFAULT_WIDTH / 2 + ra * std::sin (theta);
    float y1 = ENGINE_PGE_305_DEFAULT_HEIGHT / 2 + ra * std::cos (theta);
    particles_.push_back ({x1, y1});
  } // end FOR

  bool flag;
  for (int i = 0; i < ENGINE_PGE_305_DEFAULT_N; i++)
  {
    flag = true;
    olc::vf2d tempA;
    do
    {
      olc::vf2d noiseCoeffx;
      noiseCoeffx.x = 1e6f * Common_Tools::getRandomNumber (0.0f, 1.0f);
      noiseCoeffx.y = 1e6f * Common_Tools::getRandomNumber (0.0f, 1.0f);
      //if (std::abs (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseCoeffx.x, noiseCoeffx.y, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) < ENGINE_PGE_305_DEFAULT_ACCEPTANCE)
      if (std::abs (Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (noiseCoeffx.x, noiseCoeffx.y)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) < ENGINE_PGE_305_DEFAULT_ACCEPTANCE)
      {
        tempA = noiseCoeffx;
        flag = false;
      } // end IF
    } while (flag);
    olc::vf2d tempB;
    flag = true;
    do
    {
      olc::vf2d noiseCoeffy;
      noiseCoeffy.x = 1e6f * Common_Tools::getRandomNumber (0.0f, 1.0f);
      noiseCoeffy.y = 1e6f * Common_Tools::getRandomNumber (0.0f, 1.0f);
      //if (std::abs (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseCoeffy.x, noiseCoeffy.y, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) < ENGINE_PGE_305_DEFAULT_ACCEPTANCE)
      if (std::abs (Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (noiseCoeffy.x, noiseCoeffy.y)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) < ENGINE_PGE_305_DEFAULT_ACCEPTANCE)
      {
        tempB = noiseCoeffy;
        flag = false;
      } // end IF
    } while (flag);
    noiseCoeffX_.push_back (tempA);
    noiseCoeffY_.push_back (tempB);
  } // end FOR

  scaleFactor_ = ENGINE_PGE_305_DEFAULT_W / 2.0f;

  return true;
}

bool
PGE_305::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 0; i < ENGINE_PGE_305_DEFAULT_N; i++)
  {
    float nX = nn (noiseCoeffX_[i].x, noiseCoeffX_[i].y, frame_count_i);
    float nY = nn (noiseCoeffY_[i].x, noiseCoeffY_[i].y, frame_count_i);
    float x2 = particles_[i].x + scaleFactor_ * nX;
    float y2 = particles_[i].y + scaleFactor_ * nY;

    float r, g, b;
    Common_Image_Tools::HSVToRGB (360.0f * i / static_cast<float> (ENGINE_PGE_305_DEFAULT_N), 0.8f, 0.8f, r, g, b);
    olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
    float r_2 =
      //1.0f + 3.5f * Common_GL_Tools::map (static_cast<float> (noise_.GetValue (static_cast<float> (i), 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      1.0f + 3.5f * Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (static_cast<float> (i), 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
    olc::PixelGameEngine::FillCircle (static_cast<int32_t> (x2), static_cast<int32_t> (y2), static_cast<int32_t> (r_2), color);
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_305::nn (float x, float y, int f)
{
  f = f - 1;
  float radi = ENGINE_PGE_305_DEFAULT_STEPS / 2000.0f;
  float cx = x - radi;
  float cy = y;
  float X = cx + radi * std::cos (2.0f * static_cast<float> (M_PI) * f / static_cast<float> (ENGINE_PGE_305_DEFAULT_STEPS));
  float Y = cy + radi * std::sin (2.0f * static_cast<float> (M_PI) * f / static_cast<float> (ENGINE_PGE_305_DEFAULT_STEPS));
  //return Common_GL_Tools::map (static_cast<float> (noise_.GetValue (X, Y, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f;
  return Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (X, Y)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f;
}
