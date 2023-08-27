#ifndef PGE_150_H
#define PGE_150_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_150
 : public olc::PixelGameEngine
{
 public:
  PGE_150 ();
  inline virtual ~PGE_150 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                  c_;
  float                  h_;
  std::vector<olc::vf2d> fish_;
  //noise::module::Perlin  noise_;
  OpenSimplexNoise       noise_;
};

#endif // PGE_150_H
