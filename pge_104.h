#ifndef PGE_104_H
#define PGE_104_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_104
 : public olc::PixelGameEngine
{
 public:
  PGE_104 ();
  inline virtual ~PGE_104 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  int32_t m_;
  int32_t n_;

  float easeInOutSine (float);
  float loopNoise (int32_t, int32_t, float,
                   noise::module::Perlin*);
};

#endif // PGE_104_H
