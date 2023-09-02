#ifndef PGE_168_H
#define PGE_168_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_168
 : public olc::PixelGameEngine
{
 public:
  PGE_168 ();
  inline virtual ~PGE_168 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 counter_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;

  void D (float, float, float, float, int32_t);
};

#endif // PGE_168_H
