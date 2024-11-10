#ifndef PGE_402_H
#define PGE_402_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_402
 : public olc::PixelGameEngine
{
 public:
  PGE_402 ();
  inline virtual ~PGE_402 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct plotter
  {
    olc::Pixel color;
    float      radius;
  };

  struct plotter getPlotter (float);

  //noise::module::Perlin noise_;
  OpenSimplexNoise noise_;
};

#endif // PGE_402_H
  