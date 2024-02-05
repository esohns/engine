#ifndef PGE_344_H
#define PGE_344_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_344
 : public olc::PixelGameEngine
{
 public:
  PGE_344 ();
  inline virtual ~PGE_344 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    olc::vf2d position;
    float     r;
    int32_t   l;
  };

  //noise::module::Perlin        noise_;
  OpenSimplexNoise             noise_;
  std::vector<struct particle> particles_;
  int32_t                      t_;
};

#endif // PGE_344_H
