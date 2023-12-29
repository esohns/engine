#ifndef PGE_292_H
#define PGE_292_H

#include "olcPixelGameEngine.h"

class PGE_292
 : public olc::PixelGameEngine
{
 public:
  PGE_292 ();
  inline virtual ~PGE_292 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float smoothWave (float, float, float);
  void segment (int32_t, bool, float);
};

#endif // PGE_292_H
