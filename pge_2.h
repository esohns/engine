#ifndef PGE_2_H
#define PGE_2_H

#include "ace/Basic_Types.h"

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "defines.h"

// *NOTE*: see also: https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/GDC03.pdf
class PGE_2
 : public olc::PixelGameEngine
{
 public:
  PGE_2 (float, float, float);

  inline void addDensity (int x, int y, float amount) { x0_[IX(x, y)] += amount; }
  void addVelocity (int, int, float, float);

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void set_bounds (int, float[]);
  void diffuse (int, float[], float[], float, float);
  void project (float[], float[], float[], float[]);
  void advect (int, float[], float[], float[], float[], float);

  void render_density ();
  void render_velocity ();
  void fade_density ();

  float  dt_;
  float  diffusion_;
  float  viscosity_;

  // density
  float* x_;
  float* x0_;
  // velocity
  float* v_x_;
  float* v_y_;
  float* v_x0_;
  float* v_y0_;

  int    resolution_;

  // noise
  double                z_;
  noise::module::Perlin module_;
};

#endif // PGE_2_H
