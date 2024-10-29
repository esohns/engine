#ifndef PGE_376_H
#define PGE_376_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_376
 : public olc::PixelGameEngine
{
 public:
  PGE_376 ();
  inline virtual ~PGE_376 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int i_;
  int j_;

  //noise::module::Perlin noise_;
  OpenSimplexNoise        noise_;

  olc::Pixel rainbow (float);
};

#endif // PGE_376_H
