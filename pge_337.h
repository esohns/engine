#ifndef PGE_337_H
#define PGE_337_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_337
 : public olc::PixelGameEngine
{
 public:
  PGE_337 ();
  inline virtual ~PGE_337 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    float r;
    float d;
  };

  //noise::module::Perlin        noise_;
  OpenSimplexNoise             noise_;
  std::vector<struct particle> particles_;
  int32_t                      t_;
};

#endif // PGE_337_H
