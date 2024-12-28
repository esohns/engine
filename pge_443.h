#ifndef PGE_443_H
#define PGE_443_H

#include "olcPixelGameEngine.h"

class PGE_443
 : public olc::PixelGameEngine
{
 public:
  PGE_443 ();
  inline virtual ~PGE_443 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float funPattern (float, float, float);
  float funRadius (float);
  float myLog (int32_t, int32_t);
  float myAtan2 (int32_t, int32_t);

  int32_t columns_;
  int32_t rows_;
  float   t_;
};

#endif // PGE_443_H
