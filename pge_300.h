#ifndef PGE_300_H
#define PGE_300_H

#include "olcPixelGameEngine.h"

class PGE_300
 : public olc::PixelGameEngine
{
 public:
  PGE_300 ();
  inline virtual ~PGE_300 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int   X_;
  int   Y_;
  float P_;
};

#endif // PGE_300_H
