#include "stdafx.h"

#include "pge_80.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_80::PGE_80 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , noise_ ()
 , velocities_ ()
{
  sAppName = "Example 80";
}

bool
PGE_80::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_80_DEFAULT_N_XY * ENGINE_PGE_80_DEFAULT_N_XY; ++i)
    velocities_.push_back ({0.0f, 0.0f});

  float center_x = ENGINE_PGE_80_DEFAULT_N_XY / 2.0f;
  float center_y = ENGINE_PGE_80_DEFAULT_N_XY / 2.0f;
  float radius = std::min (ENGINE_PGE_80_DEFAULT_N_XY / 2.0f, ENGINE_PGE_80_DEFAULT_N_XY / 2.0f);
  struct particle particle_s;
  float r_angle_f, r_dist_f;
  for (int p = 0; p < ENGINE_PGE_80_DEFAULT_NUMBER_OF_PARTICLES; ++p)
  {
    r_angle_f = Common_Tools::getRandomNumber (0.0f, static_cast<float> (M_PI) * 2.0f);
    r_dist_f = Common_Tools::getRandomNumber (0.0f, 1.0f) * radius;
    particle_s.position_.x = center_x + std::cos (r_angle_f) * r_dist_f;
    particle_s.position_.y = center_y + std::sin (r_angle_f) * r_dist_f;
    particle_s.length_ = 0.0f;
    particle_s.prev_ = particle_s.position_;
    particles_.push_back (particle_s);
  } // end FOR

  return true;
}

bool
PGE_80::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    velocities_.clear ();
    particles_.clear ();
    OnUserCreate ();

    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_80_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_80_DEFAULT_ALPHA_DECAY : 0);

  static int frame_count_i = 1;

  vel_from_pot (velocities_, ENGINE_PGE_80_DEFAULT_N_XY, ENGINE_PGE_80_DEFAULT_N_XY, &noise_, frame_count_i);

  olc::Pixel color;
  for (int p = 0; p < ENGINE_PGE_80_DEFAULT_NUMBER_OF_PARTICLES; ++p)
  {
    advect_particle (particles_[p], velocities_, ENGINE_PGE_80_DEFAULT_N_XY, ENGINE_PGE_80_DEFAULT_N_XY);
    color.r = 255 - std::min (int(std::abs (particles_[p].position_.x - particles_[p].prev_.x) * 100.0f), 255);
    color.g = 255 - std::min (int(std::abs (particles_[p].position_.y - particles_[p].prev_.y) * 100.0f), 255);
    color.b = std::min (int(particles_[p].length_ * 500.0f), 255);

    olc::PixelGameEngine::DrawLine (static_cast<int32_t> (particles_[p].prev_.x * ENGINE_PGE_80_DEFAULT_VIEW_SCALE),
                                    static_cast<int32_t> (particles_[p].prev_.y * ENGINE_PGE_80_DEFAULT_VIEW_SCALE),
                                    static_cast<int32_t> (particles_[p].position_.x * ENGINE_PGE_80_DEFAULT_VIEW_SCALE),
                                    static_cast<int32_t> (particles_[p].position_.y * ENGINE_PGE_80_DEFAULT_VIEW_SCALE),
                                    color, 0xFFFFFFFF);

    particles_[p].prev_ = particles_[p].position_;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_80::get_potential (float x, float y, int32_t res_x, int32_t res_y, float time, noise::module::Perlin* noise)
{
  float distmult = static_cast<float> (std::pow (1.0f / (std::min (res_x, res_y) / 2.0f), 2));
  float dist_center =
    std::max (1.0f - static_cast<float> (std::pow (x - res_x / 2.0f, 2) + std::pow (y - res_y / 2.0f, 2)) * distmult, 0.0f);
  return Common_GL_Tools::map (static_cast<float> (noise->GetValue (x * ENGINE_PGE_80_DEFAULT_DETAIL, y * ENGINE_PGE_80_DEFAULT_DETAIL, time)), -1.0f, 1.0f, 0.0f, 1.0f) * ENGINE_PGE_80_DEFAULT_SCALE * dist_center;
}

void
PGE_80::vel_from_pot (std::vector<olc::vf2d>& vel_arr, int32_t res_x, int32_t res_y, noise::module::Perlin* noise, int frameCount_in)
{
  float time = frameCount_in * 0.005f;
  for (int32_t y = 1; y < res_y - 1; ++y)
    for (int32_t x = 1; x < res_x - 1; ++x)
      vel_arr[IX (x, y)] = {-(get_potential (static_cast<float> (x), static_cast<float> (y + ENGINE_PGE_80_DEFAULT_DX), res_x, res_y, time, noise) - get_potential (static_cast<float> (x), static_cast<float> (y - ENGINE_PGE_80_DEFAULT_DX), res_x, res_y, time, noise)) / (2.0f * ENGINE_PGE_80_DEFAULT_DX),
                             (get_potential (static_cast<float> (x + ENGINE_PGE_80_DEFAULT_DX), static_cast<float> (y), res_x, res_y, time, noise) - get_potential (static_cast<float> (x - ENGINE_PGE_80_DEFAULT_DX), static_cast<float> (y), res_x, res_y, time, noise)) / (2.0f * ENGINE_PGE_80_DEFAULT_DX)};
}

olc::vf2d
PGE_80::sample_2d (const std::vector<olc::vf2d>& arr, float x, float y, int32_t res_x, int32_t res_y)
{
  x = std::min (std::max (x, 0.0f), static_cast<float> (res_x - 2.0f));
  y = std::min (std::max (y, 0.0f), static_cast<float> (res_y - 2.0f));
  int32_t ix = static_cast<int32_t> (std::floor (x));
  int32_t iy = static_cast<int32_t> (std::floor (y));
  olc::vf2d s0 = arr[IX (ix, iy)];
  olc::vf2d s1 = arr[IX (ix + 1, iy)];
  olc::vf2d s2 = arr[IX (ix, iy + 1)];
  olc::vf2d s3 = arr[IX (ix + 1, iy + 1)];
  float tx = x - ix;
  float ty = y - iy;
  olc::vf2d sx0 = {s0.x + (s1.x - s0.x) * tx, s0.y + (s1.y - s0.y) * tx};
  olc::vf2d sx1 = {s2.x + (s3.x - s2.x) * tx, s2.y + (s3.y - s2.y) * tx};
  return {sx0.x + (sx1.x - sx0.x) * ty, sx0.y + (sx1.y - sx0.y) * ty};
}

void
PGE_80::advect_particle (struct particle& p, const std::vector<olc::vf2d>& vel_arr, int32_t res_x, int32_t res_y)
{
  float h = 0.2f;
  int32_t steps = static_cast<int32_t> (1.0f / h);
  float ox = p.position_.x;
  float oy = p.position_.y;

  // RK3
  olc::vf2d k1, k2, k3;
  float tx, ty;
  for (int32_t s = 0; s < steps; ++s)
  {
    k1 = sample_2d (vel_arr, p.position_.x, p.position_.y, res_x, res_y);
    tx = p.position_.x + k1.x * 0.5f * h;
    ty = p.position_.y + k1.y * 0.5f * h;
    k2 = sample_2d (vel_arr, tx, ty, res_x, res_y);
    tx = tx + k2.x * 2.0f * h;
    ty = ty + k2.y * 2.0f * h;
    k3 = sample_2d (vel_arr, tx, ty, res_x, res_y);
    p.position_.x += k3.x * h;
    p.position_.y += k3.y * h;
  } // end FOR

  p.length_ =
    static_cast<float> (std::sqrt (std::pow (p.position_.x - ox, 2) + std::pow (p.position_.y - oy, 2)));

  if (p.position_.x < 0.0f)
    p.position_.x = 0.0f;
  if (p.position_.x > static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1))
    p.position_.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1);
  if (p.position_.y < 0.0f)
    p.position_.y = 0.0f;
  if (p.position_.y > static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))
    p.position_.y = static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1);
}
