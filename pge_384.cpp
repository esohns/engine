#include "stdafx.h"

#include "pge_384.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_384::PGE_384 ()
 : olc::PixelGameEngine ()
 , parameters_ ()
 , points_ ()
 , fields_ ()
 , stepsPerFrame_ (3)
 , randomColor_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 384");
}

bool
PGE_384::OnUserCreate ()
{
  points_ = initialize (points_);
  fields_ = struct fields ();
  stepsPerFrame_ = Common_Tools::getRandomNumber (3, 12);
  randomColor_ = Common_Tools::getRandomNumber (0.0f, 1.0f);

  return true;
}

bool
PGE_384::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_384_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_384_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    parameters_.mu_k = Common_Tools::getRandomNumber (1.0f, 5.0f);
    parameters_.sigma_k = Common_Tools::getRandomNumber (0.1f, 2.0f);
    parameters_.w_k = Common_Tools::getRandomNumber (0.01f, 0.07f);
    parameters_.mu_g = Common_Tools::getRandomNumber (0.1f, 1.0f);
    parameters_.sigma_g = Common_Tools::getRandomNumber (0.05f, 0.3f);
    parameters_.c_rep = Common_Tools::getRandomNumber (0.1f, 2.0f);
  } // end IF

  for (int i = 0; i < stepsPerFrame_; ++i)
    step ();

  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static float scale_f = olc::PixelGameEngine::ScreenWidth () / 50.0f;

  olc::Pixel color;
  for (int i = 0; i < ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS; ++i)
  {
    int32_t x = half_width_i + static_cast<int32_t> (points_[i * 2] * scale_f);
    int32_t y = half_height_i + static_cast<int32_t> (points_[i * 2 + 1] * scale_f);
    int32_t r = static_cast<int32_t> ((parameters_.c_rep / (fields_.R_val[i] * 5.0f)) * scale_f);

    if (randomColor_ < 0.3f)
      color = olc::PixelF (i / static_cast<float> (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS),
                           70 / 200.0f,
                           100 / 200.0f,
                           1.0f);
    else if (0.3f <= randomColor_ && randomColor_ < 0.6f)
      color = olc::PixelF (90 / 200.0f,
                           80 / 200.0f,
                           i / static_cast<float> (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS),
                           1.0f);
    else
      color = olc::PixelF (20 / 200.0f,
                           i / static_cast<float> (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS),
                           180 / 200.0f,
                           1.0f);

    olc::PixelGameEngine::FillCircle (x, y, r, color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<float>
PGE_384::initialize (std::vector<float>& points)
{
  points.clear ();

  for (int i = 0; i < ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS; ++i)
  {
    points.push_back ((Common_Tools::getRandomNumber (0.0f, 1.0f) - 0.5f) * 12.0f);
    points.push_back ((Common_Tools::getRandomNumber (0.0f, 1.0f) - 0.5f) * 12.0f);
  } // end FOR

  return points;
}

void
PGE_384::add_xy (std::vector<float>& a, int i, float x, float y, float c)
{
  a[i * 2] += x * c;
  a[i * 2 + 1] += y * c;
}

void
PGE_384::compute_fields ()
{
  fields_.R_val.assign (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS, repulsion_f (0.0f, parameters_.c_rep).first);
  fields_.U_val.assign (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS, peak_f (0.0f, parameters_.mu_k, parameters_.sigma_k, parameters_.w_k).first);
  fields_.R_grad.assign (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS * 2, 0.0f);
  fields_.U_grad.assign (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS * 2, 0.0f);

  for (int i = 0; i < ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS - 1; ++i)
    for (int j = i + 1; j < ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS; ++j)
    {
      float rx = points_[i * 2] - points_[j * 2];
      float ry = points_[i * 2 + 1] - points_[j * 2 + 1];
      float r = std::sqrt (rx * rx + ry * ry) + 1e-20f;
      rx /= r;
      ry /= r;

      if (r < 1.0f)
      {
        std::pair<float, float> r_dr = repulsion_f (r, parameters_.c_rep);
        add_xy (fields_.R_grad, i, rx, ry, r_dr.second);
        add_xy (fields_.R_grad, j, rx, ry, -r_dr.second);
        fields_.R_val[i] += r_dr.first;
        fields_.R_val[j] += r_dr.first;
      } // end IF

      std::pair<float, float> k_dk = peak_f (r, parameters_.mu_k, parameters_.sigma_k, parameters_.w_k);
      add_xy (fields_.U_grad, i, rx, ry, k_dk.second);
      add_xy (fields_.U_grad, j, rx, ry, -k_dk.second);
      fields_.U_val[i] += k_dk.first;
      fields_.U_val[j] += k_dk.first;
    } // end FOR
}

std::pair<float, float>
PGE_384::repulsion_f (float x, float c_rep)
{
  float t = std::max (1.0f - x, 0.0f);

  return std::make_pair (0.5f * c_rep * t * t, -c_rep * t);
}

float
PGE_384::fast_exp (float x)
{
  float t = 1.0f + x / 32.0f;
  t *= t;
  t *= t;
  t *= t;
  t *= t;
  t *= t;

  return t;
}

std::pair<float, float>
PGE_384::peak_f (float x, float mu, float sigma, float w)
{
  float t = (x - mu) / sigma;
  float y = w / fast_exp (t * t);

  return std::make_pair (y, -2.0f * t * y / sigma);
}

float
PGE_384::step ()
{
  compute_fields ();

  float total_E = 0.0f;
  for (int i = 0; i < ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS; ++i)
  {
    std::pair<float, float> g_dg =
      peak_f (fields_.U_val[i], parameters_.mu_g, parameters_.sigma_g);
    float vx = g_dg.second * fields_.U_grad[i * 2] - fields_.R_grad[i * 2];
    float vy = g_dg.second * fields_.U_grad[i * 2 + 1] - fields_.R_grad[i * 2 + 1];
    add_xy (points_, i, vx, vy, parameters_.dt);
    total_E += fields_.R_val[i] - g_dg.first;
  } // end FOR

  return total_E / static_cast<float> (ENGINE_PGE_384_DEFAULT_NUMBER_OF_POINTS);
}
