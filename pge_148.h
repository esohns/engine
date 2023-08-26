#ifndef PGE_148_H
#define PGE_148_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_148
 : public olc::PixelGameEngine
{
 public:
  PGE_148 ();
  inline virtual ~PGE_148 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int64_t               t_;
  float                 x_;
  float                 y_;
  int32_t               z_;
  float                 r_;
  noise::module::Perlin noise_;
};

#endif // PGE_148_H
