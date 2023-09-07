#ifndef PGE_172_H
#define PGE_172_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_172
 : public olc::PixelGameEngine
{
 public:
  PGE_172 ();
  inline virtual ~PGE_172 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 C_;
  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
};

#endif // PGE_172_H
