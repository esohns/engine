#ifndef PGE_180_H
#define PGE_180_H

#include "olcPixelGameEngine.h"

class PGE_180
 : public olc::PixelGameEngine
{
 public:
  PGE_180 ();
  inline virtual ~PGE_180 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;
};

#endif // PGE_180_H
