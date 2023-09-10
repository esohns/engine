#ifndef PGE_178_H
#define PGE_178_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_178
 : public olc::PixelGameEngine
{
 public:
  PGE_178 ();
  inline virtual ~PGE_178 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t               m_;
  noise::module::Perlin noise_;
};

#endif // PGE_178_H
