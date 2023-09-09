#ifndef PGE_176_H
#define PGE_176_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_176
 : public olc::PixelGameEngine
{
 public:
  PGE_176 ();
  inline virtual ~PGE_176 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;

  olc::vf2d pos (olc::vf2d&, int, int32_t);
  void joint (olc::vf2d&, olc::vf2d&);
};

#endif // PGE_176_H
