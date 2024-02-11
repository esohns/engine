#ifndef PGE_356_H
#define PGE_356_H

#include "olcPixelGameEngine.h"

class PGE_356
 : public olc::PixelGameEngine
{
 public:
  PGE_356 ();
  inline virtual ~PGE_356 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  bool isNotBounded (float, float, int);

  int   c_;
  float y_;
  int   max_iterations_;
};

#endif // PGE_356_H
