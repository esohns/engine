#ifndef PGE_186_H
#define PGE_186_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_186
 : public olc::PixelGameEngine
{
 public:
  PGE_186 ();
  inline virtual ~PGE_186 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
};

#endif // PGE_186_H
