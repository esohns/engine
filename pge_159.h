#ifndef PGE_159_H
#define PGE_159_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_159
 : public olc::PixelGameEngine
{
 public:
  PGE_159 ();
  inline virtual ~PGE_159 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t                index_;
  noise::module::Perlin  noise_;
  std::vector<olc::vf2d> particles_;
};

#endif // PGE_159_H
