#ifndef PGE_34_H
#define PGE_34_H

#include "olcPixelGameEngine.h"

class PGE_34
 : public olc::PixelGameEngine
{
 public:
  PGE_34 ();
  inline virtual ~PGE_34 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float factor_;
  float r_;

  olc::vf2d getVector (int, int);
};

#endif // PGE_34_H
