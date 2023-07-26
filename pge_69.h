#ifndef PGE_69_H
#define PGE_69_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_69
 : public olc::PixelGameEngine
{
 public:
  PGE_69 ();
  inline virtual ~PGE_69 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  float                 w_;

  void tree (int32_t, float, float, float, float, float, int);
};

#endif // PGE_69_H
