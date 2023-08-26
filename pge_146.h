#ifndef PGE_146_H
#define PGE_146_H

#include "olcPixelGameEngine.h"

class PGE_146
 : public olc::PixelGameEngine
{
 public:
  PGE_146 ();
  inline virtual ~PGE_146 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float   h_;
  float   q_;
  int32_t y_;

  float f (float, float, int n = 4);
};

#endif // PGE_146_H
