#ifndef PGE_419_H
#define PGE_419_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_419
 : public olc::PixelGameEngine
{
 public:
  PGE_419 ();
  inline virtual ~PGE_419 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void D (int, int, int);

  float            C_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise noise_;
};

#endif // PGE_419_H
