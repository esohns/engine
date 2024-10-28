#ifndef PGE_372_H
#define PGE_372_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_372
 : public olc::PixelGameEngine
{
 public:
  PGE_372 ();
  inline virtual ~PGE_372 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct rectangle
  {
    olc::vf2d position;
    float     w;
    float     h;
  };

  //noise::module::Perlin noise_;
  OpenSimplexNoise        noise_;
};

#endif // PGE_372_H
