#ifndef PGE_136_H
#define PGE_136_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_136
 : public olc::PixelGameEngine
{
 public:
  PGE_136 ();
  inline virtual ~PGE_136 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> dots_;
  noise::module::Perlin  noise_;
};

#endif // PGE_136_H
