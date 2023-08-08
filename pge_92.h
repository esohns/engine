#ifndef PGE_92_H
#define PGE_92_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_gl_common.h"

class PGE_92
 : public olc::PixelGameEngine
{
 public:
  PGE_92 ();
  inline virtual ~PGE_92 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  double                noiseOffsetX_;
  double                noiseOffsetY_;
  Common_GL_Color_t     startColor_;
  Common_GL_Color_t     endColor_;
  noise::module::Perlin noise_;

  float sdfValue (float, float);
};

#endif // PGE_92_H
