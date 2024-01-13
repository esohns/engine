#ifndef PGE_305_H
#define PGE_305_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_305
 : public olc::PixelGameEngine
{
 public:
  PGE_305 ();
  inline virtual ~PGE_305 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  //noise::module::Perlin  noise_;
  OpenSimplexNoise       noise_;
  std::vector<olc::vf2d> noiseCoeffX_;
  std::vector<olc::vf2d> noiseCoeffY_;
  std::vector<olc::vf2d> particles_;
  float                  scaleFactor_;

  float nn (float, float, int);
};

#endif // PGE_305_H
