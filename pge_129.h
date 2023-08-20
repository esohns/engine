#ifndef PGE_129_H
#define PGE_129_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_129
 : public olc::PixelGameEngine
{
 public:
  PGE_129 ();
  inline virtual ~PGE_129 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  //noise::module::Perlin noise_;
  OpenSimplexNoise noise_;
  int32_t          w_;
};

#endif // PGE_129_H
