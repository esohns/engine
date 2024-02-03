#ifndef PGE_340_H
#define PGE_340_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_340
 : public olc::PixelGameEngine
{
 public:
  PGE_340 ();
  inline virtual ~PGE_340 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  int                   t_;
};

#endif // PGE_340_H
