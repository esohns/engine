#include "stdafx.h"

#include "pge_2.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines.h"
#include "engine_common.h"

PGE_2::PGE_2 (float dt, float diffusion, float viscosity)
 : olc::PixelGameEngine ()
 , resolution_ (0)
 , dt_ (dt)
 , diffusion_ (diffusion)
 , viscosity_ (viscosity)
 , x_ (NULL)
 , x0_ (NULL)
 , v_x_ (NULL)
 , v_y_ (NULL)
 , v_x0_ (NULL)
 , v_y0_ (NULL)
 , z_ (ENGINE_GLUT_3_DEFAULT_NOISE_Z)
 , module_ ()
{
  sAppName = "Example 2";
}

void
PGE_2::set_bounds (int b, float x[])
{
  for (int i = 1; i < resolution_ - 1; i++)
  {
    x[IX(0,i)]                   = b == 1 ? -x[IX(1,i)] : x[IX(1,i)];
    x[IX(resolution_ - 2 + 1,i)] = b == 1 ? -x[IX(resolution_ - 2,i)] : x[IX(resolution_ - 2,i)];
    x[IX(i,0 )]                  = b == 2 ? -x[IX(i,1)] : x[IX(i,1)];
    x[IX(i,resolution_ - 2 + 1)] = b == 2 ? -x[IX(i,resolution_ - 2)] : x[IX(i,resolution_ - 2)];
  }

  x[IX(0, 0)]                                     = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
  x[IX(0, resolution_ - 2 + 1)]                   = 0.5f * (x[IX(1, resolution_ - 2 + 1)] + x[IX(0, resolution_ - 2)]);
  x[IX(resolution_ - 2 + 1, 0)]                   = 0.5f * (x[IX(resolution_ - 2, 0)] + x[IX(resolution_ - 2 + 1, 1)]);
  x[IX(resolution_ - 2 + 1, resolution_ - 2 + 1)] = 0.5f * (x[IX(resolution_ - 2, resolution_ - 2 + 1)] + x[IX(resolution_ - 2 + 1, resolution_ - 2)]);
}

void
PGE_2::diffuse (int b, float x[], float x0[], float diff, float dt)
{
  float a = dt * diff * (resolution_ - 2) * (resolution_ - 2);

  for (int k = 0; k < ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS; k++)
  {
    for (int i = 1; i < resolution_ - 1; i++)
      for (int j = 1; j < resolution_ - 1; j++)
        x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                                           x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
    set_bounds (b, x);
  }
}

void
PGE_2::project (float v_x[], float v_y[], float p[], float div[])
{
  float h = 1.0f / resolution_ - 2;

  for (int i = 1; i < resolution_ - 1; i++)
    for (int j = 1; j < resolution_ - 1; j++)
    {
      div[IX(i, j)] = -0.5f * h * (v_x[IX(i + 1, j)] - v_x[IX(i - 1, j)] +
                                   v_y[IX(i, j + 1)] - v_y[IX(i, j - 1)]);
      p[IX(i, j)] = 0.0f;
    }
  set_bounds (0, div); set_bounds (0, p);

  for (int k = 0; k < ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS; k++)
  {
    for (int i = 1; i < resolution_ - 1; i++)
      for (int j = 1; j < resolution_ - 1; j++)
        p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
                                       p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
    set_bounds (0, p);
  }

  for (int i = 1; i < resolution_ - 1; i++)
    for (int j = 1; j < resolution_ - 1; j++)
    {
      v_x[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
      v_y[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
    }
  set_bounds (1, v_x); set_bounds (2, v_y);
}

void
PGE_2::advect (int b, float d[], float d0[], float v_x[], float v_y[], float dt)
{
  int i0, i1, j0, j1;
  float dt0 = dt * (resolution_ - 2);
  float s0, s1, t0, t1;
  float x, y;

  for (int j = 1; j < resolution_ - 1; j++)
    for (int i = 1; i < resolution_ - 1; i++)
    {
      x = i - dt0 * v_x[IX(i, j)];
      y = j - dt0 * v_y[IX(i, j)];

      if (isnan (x) || x < 0.5f)
        x = 0.5f;
      if (x > resolution_ - 2 + 0.5f)
        x = resolution_ - 2 + 0.5f;
      i0 = int (x); i1 = i0 + 1;
      if (isnan (y) || y < 0.5f)
        y = 0.5f;
      if (y > resolution_ - 2 + 0.5f)
        y = resolution_ - 2 + 0.5f;
      j0 = int (y); j1 = j0 + 1;

      s1 = x - i0; s0 = 1 - s1;
      t1 = y - j0; t0 = 1 - t1;

      d[IX(i, j)] =
        s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
        s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
    }

  set_bounds (b, d);
}

bool
PGE_2::OnUserCreate ()
{
  // sanity check(s)
  ACE_ASSERT (ScreenWidth () == ScreenHeight ());

  resolution_ = ScreenWidth ();

  x_ = new float[resolution_ * resolution_];
  ACE_OS::memset (x_, 0, sizeof (float) * resolution_ * resolution_);
  x0_ = new float[resolution_ * resolution_];
  ACE_OS::memset (x0_, 0, sizeof (float) * resolution_ * resolution_);
  v_x_ = new float[resolution_ * resolution_];
  ACE_OS::memset (v_x_, 0, sizeof (float) * resolution_ * resolution_);
  v_y_ = new float[resolution_ * resolution_];
  ACE_OS::memset (v_y_, 0, sizeof (float) * resolution_ * resolution_);
  v_x0_ = new float[resolution_ * resolution_];
  ACE_OS::memset (v_x0_, 0, sizeof (float) * resolution_ * resolution_);
  v_y0_ = new float[resolution_ * resolution_];
  ACE_OS::memset (v_y0_, 0, sizeof (float) * resolution_ * resolution_);

  return true;
}

void
PGE_2::addVelocity (int x, int y, float amountX, float amountY)
{
  v_x_[IX(x, y)] += amountX;
  v_y_[IX(x, y)] += amountY;
}

void
PGE_2::render_density ()
{
  for (int i = 0; i < resolution_; i++)
    for (int j = 0; j < resolution_; j++)
      Draw (i, j,
            olc::Pixel (255, 255, 255, static_cast<int> (x0_[IX(i, j)]) % 256));
}

void
PGE_2::render_velocity ()
{
  for (int j = 0; j < resolution_; j++)
    for (int i = 0; i < resolution_; i++)
    {
      int vx = isnan (v_x_[IX(i, j)]) ? 0 : static_cast<int> (v_x_[IX(i, j)]);
      int vy = isnan (v_y_[IX(i, j)]) ? 0 : static_cast<int> (v_y_[IX(i, j)]);

      if (vx || vy)
        DrawLine (i, j,
                  i + vx, j + vy,
                  olc::WHITE,
                  0xFFFFFFFF);
    }
}

void
PGE_2::fade_density ()
{
  for (int i = 0; i < resolution_; i++)
    for (int j = 0; j < resolution_; j++)
    {
      x_[IX(i, j)] -= 0.02f;
      if (x_[IX(i, j)] < 0.0f)
        x_[IX(i, j)] = 0.0f;
    }
}

bool
PGE_2::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::Pixel (0, 0, 0, 255)); // black

  int cx = int(0.5f * resolution_);
  int cy = int(0.5f * resolution_);

  int32_t mouse_x = GetMouseX ();
  int32_t mouse_y = GetMouseY ();
  addDensity (mouse_x, mouse_y, Common_Tools::getRandomNumber (50.0f, 250.0f));
  //Draw (mouse_x, mouse_y,
  //      olc::Pixel(255, 255, 255, 255));

  for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++)
      addDensity (cx + i, cy + j, Common_Tools::getRandomNumber (50.0f, 250.0f));

  //for (int i = 0; i < 2; i++)
  //{
    float angle_f =
      static_cast<float> (module_.GetValue (ENGINE_GLUT_3_DEFAULT_NOISE_X,
                                            ENGINE_GLUT_3_DEFAULT_NOISE_Y,
                                            z_) * M_PI * 2.0);
    z_ += 0.01;
    addVelocity (mouse_x, mouse_y,
                 std::abs (std::cos (angle_f * 0.1f)), std::abs (std::sin (angle_f * 0.1f)));
  //}

  // velocity step
  SWAP (v_x0_, v_x_);
  diffuse (1, v_x_, v_x0_, viscosity_, dt_);
  SWAP (v_y0_, v_y_);
  diffuse (2, v_y_, v_y0_, viscosity_, dt_);

  project (v_x_, v_y_, v_x0_, v_y0_);

  SWAP (v_x0_, v_x_); SWAP (v_y0_, v_y_);
  advect (1, v_x_, v_x0_, v_x0_, v_y0_, dt_);
  advect (2, v_y_, v_y0_, v_x0_, v_y0_, dt_);
  project (v_x_, v_y_, v_x0_, v_y0_);

  // density step
  SWAP (x0_, x_);
  diffuse (0, x_, x0_, diffusion_, dt_);
  SWAP (x0_, x_);
  advect (0, x_, x0_, v_x_, v_y_, dt_);

  render_density ();
  //fade_density ();

  //render_velocity ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
