#ifndef PGE_173_H
#define PGE_173_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_173
 : public olc::PixelGameEngine
{
 public:
  PGE_173 ();
  inline virtual ~PGE_173 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
};

#endif // PGE_173_H
