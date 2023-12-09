#ifndef PGE_263_H
#define PGE_263_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_263
 : public olc::PixelGameEngine
{
 public:
  PGE_263 ();
  inline virtual ~PGE_263 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int                   f_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;
};

#endif // PGE_263_H
