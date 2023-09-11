#ifndef PGE_184_H
#define PGE_184_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_184
 : public olc::PixelGameEngine
{
 public:
  PGE_184 ();
  inline virtual ~PGE_184 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t               m_;
  noise::module::Perlin noise_;
};

#endif // PGE_184_H
