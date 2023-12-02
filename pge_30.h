#ifndef PGE_30_H
#define PGE_30_H

#include "olcPixelGameEngine.h"

#include "defines_2.h"

class PGE_30
 : public olc::PixelGameEngine
{
 public:
  PGE_30 ();
  inline virtual ~PGE_30 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  uint8_t red_[ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS];
  uint8_t green_[ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS];
  uint8_t blue_[ENGINE_PGE_30_DEFAULT_MAX_ITERATIONS];
  float angle_;
};

#endif // PGE_30_H
