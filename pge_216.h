#ifndef PGE_216_H
#define PGE_216_H

#include "olcPixelGameEngine.h"

class PGE_216
 : public olc::PixelGameEngine
{
 public:
  PGE_216 ();
  inline virtual ~PGE_216 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float a_;

  float distance (int32_t, int32_t, int32_t, int32_t);
};

#endif // PGE_216_H
