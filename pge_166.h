#ifndef PGE_166_H
#define PGE_166_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "defines_7.h"

class PGE_166
 : public olc::PixelGameEngine
{
 public:
  PGE_166 ();
  inline virtual ~PGE_166 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 A_;
  noise::module::Perlin noise_;
  float                 t_;

  void T (float, int32_t = 7, int32_t = ENGINE_PGE_166_DEFAULT_W / 2, int32_t = ENGINE_PGE_166_DEFAULT_W / 2, float = ENGINE_PGE_166_DEFAULT_L);
};

#endif // PGE_166_H
