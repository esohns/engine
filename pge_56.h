#ifndef PGE_56_H
#define PGE_56_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_56
 : public olc::PixelGameEngine
{
 public:
  PGE_56 ();
  inline virtual ~PGE_56 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
};

#endif // PGE_56_H
