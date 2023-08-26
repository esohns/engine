#ifndef PGE_147_H
#define PGE_147_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_147
 : public olc::PixelGameEngine
{
 public:
  PGE_147 ();
  inline virtual ~PGE_147 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 t_;
  float                 m_;
  noise::module::Perlin noise_;

  void tree (float, float, float, int32_t = 9, int32_t = 60);
};

#endif // PGE_147_H
