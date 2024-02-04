#ifndef PGE_342_H
#define PGE_342_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_342
 : public olc::PixelGameEngine
{
 public:
  PGE_342 ();
  inline virtual ~PGE_342 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  int                   t_;
};

#endif // PGE_342_H
