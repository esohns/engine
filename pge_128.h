#ifndef PGE_128_H
#define PGE_128_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_128
 : public olc::PixelGameEngine
{
 public:
  PGE_128 ();
  inline virtual ~PGE_128 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 counter_;
  float                 mX_;
  float                 mY_;
  noise::module::Perlin noise_;

  void drawRect (int, int, int);
};

#endif // PGE_128_H
