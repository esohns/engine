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

int
PGE_2::IX (int x, int y)
{
  if (x < 0) { x = 0; }
  if (x > resolution_ - 1) { x = resolution_ - 1; }

  if (y < 0) { y = 0; }
  if (y > resolution_ - 1) { y = resolution_ - 1; }

  return (y * resolution_) + x;
}

void
PGE_2::set_bounds (int b, float x[])
{
  for (int i = 1; i < resolution_ - 1; i++)
  {
    x[IX(0,i)]                   = b == 1 ? -x[IX(1,i)] : x[IX(1,i)];
    x[IX(resolution_ - 1,i)]     = b == 1 ? -x[IX(resolution_ - 2,i)] : x[IX(resolution_ - 2,i)];
    x[IX(i,0 )]                  = b == 2 ? -x[IX(i,1)] : x[IX(i,1)];
    x[IX(i,resolution_ - 1)]     = b == 2 ? -x[IX(i,resolution_ - 2)] : x[IX(i,resolution_ - 2)];
  }

  //x[IX(0, 0)]                                     = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
  //x[IX(0, resolution_ - 2 + 1)]                   = 0.5f * (x[IX(1, resolution_ - 2 + 1)] + x[IX(0, resolution_ - 2)]);
  //x[IX(resolution_ - 2 + 1, 0)]                   = 0.5f * (x[IX(resolution_ - 2, 0)] + x[IX(resolution_ - 2 + 1, 1)]);
  //x[IX(resolution_ - 2 + 1, resolution_ - 2 + 1)] = 0.5f * (x[IX(resolution_ - 2, resolution_ - 2 + 1)] + x[IX(resolution_ - 2 + 1, resolution_ - 2)]);
  x[IX(0, 0)]                                     = 0.33f * (x[IX(1, 0)] + x[IX(0, 1)] + x[IX(0, 0)]);
  x[IX(0, resolution_ - 1)]                       = 0.33f * (x[IX(1, resolution_ - 2 + 1)] + x[IX(0, resolution_ - 2)] + x[IX(0, resolution_ - 2 + 1)]);
  x[IX(resolution_ - 1, 0)]                       = 0.33f * (x[IX(resolution_ - 2, 0)] + x[IX(resolution_ - 1, 1)] + x[IX(resolution_ - 1, 0)]);
  x[IX(resolution_ - 1, resolution_ - 1)]         = 0.33f * (x[IX(resolution_ - 2, resolution_ - 1)] + x[IX(resolution_ - 1, resolution_ - 2)] + x[IX(resolution_ - 1, resolution_ - 1)]);
}

void
PGE_2::solve (int b, float x[], float x0[], float a, float c)
{
  float cRecip = 1.0f / c;

  for (int k = 0; k < ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS; k++)
  {
    for (int j = 1; j < resolution_ - 1; j++)
      for (int i = 1; i < resolution_ - 1; i++)
        x[IX(i, j)] = (x0[IX(i, j)] + a
          * (x[IX(i + 1, j)]
            + x[IX(i - 1, j)]
            + x[IX(i, j + 1)]
            + x[IX(i, j - 1)]
            + x[IX(i, j)]
            + x[IX(i, j)]
            )) * cRecip;
    set_bounds (b, x);
  }
}

void
PGE_2::diffuse (int b, float x[], float x0[], float diff, float dt)
{
  float a = dt * diff * (resolution_ - 2) * (resolution_ - 2);

  solve (b, x, x0, a, 1 + 6 * a);
  //for (int k = 0; k < ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS; k++)
  //{
  //  for (int i = 1; i < resolution_ - 1; i++)
  //    for (int j = 1; j < resolution_ - 1; j++)
  //      x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
  //                                         x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
  //  set_bounds (b, x);
  //}
}

void
PGE_2::project (float v_x[], float v_y[], float p[], float div[])
{
  float h = 1.0f / resolution_;

  for (int i = 1; i < resolution_ - 1; i++)
    for (int j = 1; j < resolution_ - 1; j++)
    {
      div[IX(i, j)] = -0.5f * h * (v_x[IX(i + 1, j)] - v_x[IX(i - 1, j)] +
                                   v_y[IX(i, j + 1)] - v_y[IX(i, j - 1)]);
      p[IX(i, j)] = 0.0f;
    }
  set_bounds (0, div); set_bounds (0, p);

  //for (int k = 0; k < ENGINE_PGE_2_DEFAULT_NUM_ITERATIONS; k++)
  //{
  //  for (int i = 1; i < resolution_ - 1; i++)
  //    for (int j = 1; j < resolution_ - 1; j++)
  //      p[IX(i, j)] = (div[IX(i, j)] + ((p[IX(i - 1, j)] + p[IX(i + 1, j)] +
  //                                       p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4));
  //  set_bounds (0, p);
  //}
  solve (0, p, div, 1, 6);

  for (int i = 1; i < resolution_ - 1; i++)
    for (int j = 1; j < resolution_ - 1; j++)
    {
      //v_x[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
      //v_y[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
      v_x[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)] - p[IX(i - 1, j)]) * static_cast<float> (resolution_);
      v_y[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)] - p[IX(i, j - 1)]) * static_cast<float> (resolution_);
    }
  set_bounds (1, v_x); set_bounds (2, v_y);
}

void
PGE_2::advect (int b, float d[], float d0[], float v_x[], float v_y[], float dt)
{
  float i0, i1, j0, j1;

  float dtx = dt * (resolution_ - 2);
  float dty = dt * (resolution_ - 2);

  float s0, s1, t0, t1;
  float tmp1, tmp2, x, y;

  float Nfloat = static_cast<float> (resolution_);
  float ifloat, jfloat;

  int i, j;

  for (j = 1, jfloat = 1; j < resolution_ - 1; j++, jfloat++) {
    for (i = 1, ifloat = 1; i < resolution_ - 1; i++, ifloat++) {
      tmp1 = dtx * v_x[IX(i, j)];
      tmp2 = dty * v_y[IX(i, j)];
      x = ifloat - tmp1;
      y = jfloat - tmp2;

      if (x < 0.5f) x = 0.5f;
      if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
      i0 = ::floorf(x);
      i1 = i0 + 1.0f;
      if (y < 0.5f) y = 0.5f;
      if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
      j0 = ::floorf(y);
      j1 = j0 + 1.0f;

      s1 = x - i0;
      s0 = 1.0f - s1;
      t1 = y - j0;
      t0 = 1.0f - t1;

      int i0i = static_cast<int> (i0);
      int i1i = static_cast<int> (i1);
      int j0i = static_cast<int> (j0);
      int j1i = static_cast<int> (j1);

      d[IX(i, j)] =
        s0 * (t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)]) +
        s1 * (t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]);
    }
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
            olc::Pixel (255, 255, 255, int(x_[IX(i, j)]) > 255 ? 255 : int (x_[IX(i, j)])));
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
      x_[IX(i, j)] -= 0.05f;
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

  static int32_t mouse_x_prev = GetMouseX ();
  static int32_t mouse_y_prev = GetMouseY ();

  int32_t mouse_x = GetMouseX ();
  int32_t mouse_y = GetMouseY ();
  struct olc::HWButton button_s = GetMouse (olc::Mouse::LEFT);
  if (button_s.bPressed || button_s.bHeld)
    for (int i = -7; i <= 7; i++)
      for (int j = -7; j <= 7; j++)
        addDensity (mouse_x + i, mouse_y + j, Common_Tools::getRandomNumber (150.0f, 250.0f));
  //Draw (mouse_x, mouse_y,
  //      olc::Pixel (255, 255, 255, 255));

  //for (int i = -15; i <= 15; i++)
  //  for (int j = -15; j <= 15; j++)
  //    addDensity (cx + i, cy + j, Common_Tools::getRandomNumber (150.0f, 250.0f));

  //for (int i = 0; i < 2; i++)
  //{
    //float value_f =
    //  static_cast<float> (module_.GetValue (ENGINE_GLUT_3_DEFAULT_NOISE_X,
    //                                        ENGINE_GLUT_3_DEFAULT_NOISE_Y,
    //                                        z_));
    //z_ += 0.01;

    //addDensity (cx, cy, value_f * 255.0f);

    //float angle_f = value_f * static_cast<float> (M_PI) * 2.0f;
    //addVelocity (mouse_x, mouse_y,
    //             std::abs (std::cos (angle_f) * 0.05f), std::abs (std::sin (angle_f) * 0.05f));
  //}

  float amountX = mouse_x - static_cast<float> (mouse_x_prev);
  float amountY = mouse_y - static_cast<float> (mouse_y_prev);
  addVelocity (mouse_x, mouse_y,
               amountX, amountY);
  mouse_x_prev = mouse_x;
  mouse_y_prev = mouse_y;

  ////////////////////////////////////////

  // velocity step
  //SWAP (v_x0_, v_x_);
  diffuse (1, v_x0_, v_x_, viscosity_, dt_);
  //SWAP (v_y0_, v_y_);
  diffuse (2, v_y0_, v_y_, viscosity_, dt_);

  project (v_x0_, v_y0_, v_x_, v_y_);

  //SWAP (v_x0_, v_x_); SWAP (v_y0_, v_y_);
  advect (1, v_x_, v_x0_, v_x0_, v_y0_, dt_);
  advect (2, v_y_, v_y0_, v_x0_, v_y0_, dt_);
  project (v_x_, v_y_, v_x0_, v_y0_);

  // density step
  //SWAP (x0_, x_);
  diffuse (0, x0_, x_, diffusion_, dt_);
  //SWAP (x0_, x_);
  advect (0, x_, x0_, v_x_, v_y_, dt_);

  render_density ();
  fade_density ();

  //render_velocity ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
