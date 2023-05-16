#ifndef PGE_23_H
#define PGE_23_H

#include <array>

#include "olcPixelGameEngine.h"

#include "defines.h"

class PGE_23
  : public olc::PixelGameEngine
{
public:
  PGE_23 ();
  inline virtual ~PGE_23 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

private:
  struct element
  {
    float a;
    float b;
  };
  typedef std::array<std::array<struct element, ENGINE_PGE_23_DEFAULT_WIDTH>, ENGINE_PGE_23_DEFAULT_HEIGHT> grid_t;
  grid_t grid_;
  grid_t next_;

  float dA_ = 1.0f;
  float dB_ = 0.5f;
  float feed_ = 0.055f;
  float k_ = 0.062f;

  float laplaceA (int, int);
  float laplaceB (int, int);
};

#endif // PGE_23_H
