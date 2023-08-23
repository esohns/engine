#ifndef PGE_138_H
#define PGE_138_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_138
 : public olc::PixelGameEngine
{
 public:
  PGE_138 ();
  inline virtual ~PGE_138 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
  float                 maxx_;
  float                 maxy_;
  int32_t               y_;

  olc::Pixel getColor (float, float);
};

#endif // PGE_138_H
