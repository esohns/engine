#ifndef PGE_185_H
#define PGE_185_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_185
 : public olc::PixelGameEngine
{
 public:
  PGE_185 ();
  inline virtual ~PGE_185 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
  float                 t_;
};

#endif // PGE_185_H
