#ifndef PGE_63_H
#define PGE_63_H

#include "olcPixelGameEngine.h"

class PGE_63
 : public olc::PixelGameEngine
{
 public:
  PGE_63 ();
  inline virtual ~PGE_63 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float s_;
  float t_;
};

#endif // PGE_63_H
