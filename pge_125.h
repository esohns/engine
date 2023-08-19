#ifndef PGE_125_H
#define PGE_125_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_125
 : public olc::PixelGameEngine
{
 public:
  PGE_125 ();
  inline virtual ~PGE_125 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin  noise_;
  std::vector<olc::vf2d> points_;
  float                  c_;
  float                  d_;
  float                  h_;
};

#endif // PGE_125_H
