#ifndef PGE_49_H
#define PGE_49_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_49
 : public olc::PixelGameEngine
{
 public:
  PGE_49 ();
  inline virtual ~PGE_49 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  double                yOffset_;
};

#endif // PGE_49_H
