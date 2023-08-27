#ifndef PGE_149_H
#define PGE_149_H

#include "olcPixelGameEngine.h"

class PGE_149
 : public olc::PixelGameEngine
{
 public:
  PGE_149 ();
  inline virtual ~PGE_149 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;
};

#endif // PGE_149_H
