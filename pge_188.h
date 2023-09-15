#ifndef PGE_188_H
#define PGE_188_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_188
 : public olc::PixelGameEngine
{
 public:
  PGE_188 ();
  inline virtual ~PGE_188 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
  float                 C_;
};

#endif // PGE_188_H
