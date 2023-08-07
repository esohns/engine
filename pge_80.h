#ifndef PGE_80_H
#define PGE_80_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "noise/noise.h"

#include "defines_3.h"

class PGE_80
 : public olc::PixelGameEngine
{
 public:
  PGE_80 ();
  inline virtual ~PGE_80 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    olc::vf2d position_;
    olc::vf2d prev_;
    float     length_;
  };
  std::vector<particle>  particles_;
  noise::module::Perlin  noise_;
  std::vector<olc::vf2d> velocities_;

  float get_potential (float, float, int32_t, int32_t, float, noise::module::Perlin*);
  inline int32_t IX (int32_t x, int32_t y) { return x + y * ENGINE_PGE_80_DEFAULT_N_XY; }
  void vel_from_pot (std::vector<olc::vf2d>&, int32_t, int32_t, noise::module::Perlin*, int);
  olc::vf2d sample_2d (const std::vector<olc::vf2d>&, float, float, int32_t, int32_t);
  void advect_particle (struct particle&, const std::vector<olc::vf2d>&, int32_t, int32_t);
};

#endif // PGE_80_H
