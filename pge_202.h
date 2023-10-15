#ifndef PGE_202_H
#define PGE_202_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "defines_9.h"

class PGE_202
 : public olc::PixelGameEngine
{
 public:
  PGE_202 ();
  inline virtual ~PGE_202 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  olc::Pixel            color_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
  float                 t_;
};

#endif // PGE_202_H
