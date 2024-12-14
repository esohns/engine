#ifndef PGE_437_H
#define PGE_437_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_437
 : public olc::PixelGameEngine
{
 public:
  PGE_437 ();
  inline virtual ~PGE_437 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
};

#endif // PGE_437_H
