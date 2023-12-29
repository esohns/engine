#ifndef PGE_290_H
#define PGE_290_H

#include "olcPixelGameEngine.h"

#include "defines_13.h"

class PGE_290
 : public olc::PixelGameEngine
{
 public:
  PGE_290 ();
  inline virtual ~PGE_290 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float x_[ENGINE_PGE_290_DEFAULT_NUM];
  float y_[ENGINE_PGE_290_DEFAULT_NUM];
  float vx_[ENGINE_PGE_290_DEFAULT_NUM];
  float vy_[ENGINE_PGE_290_DEFAULT_NUM];
  float minDistSq_;
  float maxDistSq_;
};

#endif // PGE_290_H
