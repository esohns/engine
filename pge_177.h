#ifndef PGE_177_H
#define PGE_177_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_177
 : public olc::PixelGameEngine
{
 public:
  PGE_177 ();
  inline virtual ~PGE_177 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
};

#endif // PGE_177_H
