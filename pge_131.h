#ifndef PGE_131_H
#define PGE_131_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_131
 : public olc::PixelGameEngine
{
 public:
  PGE_131 ();
  inline virtual ~PGE_131 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  int32_t               w_;
};

#endif // PGE_131_H
