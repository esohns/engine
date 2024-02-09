#include "stdafx.h"

#include "pge_353.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_16.h"

PGE_353::PGE_353 ()
 : olc::PixelGameEngine ()
 , input_ ()
 , output_ ()
 , showIdentity_ (false)
 , t_ (0.0f)
 , xFactor_ (0.0f)
 , yFactor_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 353");
}

bool
PGE_353::OnUserCreate ()
{
  xFactor_ =
    olc::PixelGameEngine::ScreenWidth() / static_cast<float> (ENGINE_PGE_353_DEFAULT_RESOLUTION);
  yFactor_ =
    olc::PixelGameEngine::ScreenHeight() / static_cast<float> (ENGINE_PGE_353_DEFAULT_RESOLUTION);

  createInput ();
  initOutput ();

  return true;
}

bool
PGE_353::OnUserUpdate (float fElapsedTime)
{
  computeOutput (t_);
  updateScreen ();

  t_ += ENGINE_PGE_353_DEFAULT_DT;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_353::createInput ()
{
  input_.clear ();

  for (int i = 0; i < ENGINE_PGE_353_DEFAULT_RESOLUTION; i++)
  {
    std::vector<std::complex<float> > temp_a;
    input_.push_back (temp_a);
    float a =
      (2.0f * i / static_cast<float> (ENGINE_PGE_353_DEFAULT_RESOLUTION) - 1.0f) * ENGINE_PGE_353_DEFAULT_L1SIZE;
    for (int j = 0; j < ENGINE_PGE_353_DEFAULT_RESOLUTION; j++)
    {
      float b = (2.0f * j / static_cast<float> (ENGINE_PGE_353_DEFAULT_RESOLUTION) - 1.0f) * ENGINE_PGE_353_DEFAULT_L1SIZE;
      input_[i].push_back (std::complex<float> (a, b));
    } // end FOR
  } // end FOR
}

void
PGE_353::initOutput ()
{
  output_.clear ();

  for (int i = 0; i < ENGINE_PGE_353_DEFAULT_RESOLUTION; i++)
  {
    std::vector<std::complex<float> > temp_a;
    output_.push_back (temp_a);
    for (int j = 0; j < ENGINE_PGE_353_DEFAULT_RESOLUTION; j++)
      output_[i].push_back (std::complex<float> (0.0f, 0.0f));
  } // end FOR
}

std::complex<float>
PGE_353::f (std::complex<float>& z, float t)
{
  if (showIdentity_)
    return z;

  return (z * z) + std::complex<float> (std::sin (t), 0.0f);  // z^2 + sin(t)
}

void
PGE_353::computeOutput (float t)
{
  for (int i = 0; i < ENGINE_PGE_353_DEFAULT_RESOLUTION; i++)
    for (int j = 0; j < ENGINE_PGE_353_DEFAULT_RESOLUTION; j++)
      output_[i][j] = f (input_[i][j], t);
}

olc::Pixel
PGE_353::complexToColor (std::complex<float>& z)
{
  float a = z.real ();
  float b = z.imag ();
  // polar coordinates
  float theta = std::atan2 (b, a);
  float r = (a * a + b * b) * 10.0f;

  float hue_f =
    Common_GL_Tools::map (theta, static_cast<float> (-M_PI), static_cast<float> (M_PI), 0.0f, 360.0f);
  float brightness_f =
    Common_GL_Tools::map (std::atan (r), 0.0f, static_cast<float> (M_PI_2), 0.0f, 1.0f);
  float r_2, g, b_2;
  Common_Image_Tools::HSVToRGB (hue_f, 1.0f, brightness_f, r_2, g, b_2);

  return olc::Pixel (static_cast<uint8_t> (r_2 * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b_2 * 255.0f), 255);
}

void
PGE_353::updateScreen ()
{
  for (int i = 0; i < ENGINE_PGE_353_DEFAULT_RESOLUTION; i++)
    for (int j = 0; j < ENGINE_PGE_353_DEFAULT_RESOLUTION; j++)
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (i * xFactor_),
                                      static_cast<int32_t> (j * yFactor_),
                                      ENGINE_PGE_353_DEFAULT_SQUARE_SIZE, ENGINE_PGE_353_DEFAULT_SQUARE_SIZE,
                                      complexToColor (output_[i][j]));
}
