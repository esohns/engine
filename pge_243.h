#ifndef PGE_243_H
#define PGE_243_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_243
 : public olc::PixelGameEngine
{
 public:
  PGE_243 ();
  inline virtual ~PGE_243 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int                   c_;
  int                   f_;
  float                 p_;
  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
};

#endif // PGE_243_H
