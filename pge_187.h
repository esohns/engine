#ifndef PGE_187_H
#define PGE_187_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_187
 : public olc::PixelGameEngine
{
 public:
  PGE_187 ();
  inline virtual ~PGE_187 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
  float                 t_;
  int32_t               h_;
};

#endif // PGE_187_H
