#ifndef PGE_153_H
#define PGE_153_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_153
 : public olc::PixelGameEngine
{
 public:
  PGE_153 ();
  virtual ~PGE_153 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> agents_;
  uint8_t*               grid_;
  noise::module::Perlin  noise_;
};

#endif // PGE_153_H
