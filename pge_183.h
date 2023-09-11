#ifndef PGE_183_H
#define PGE_183_H

#include "olcPixelGameEngine.h"

class PGE_183
 : public olc::PixelGameEngine
{
 public:
  PGE_183 ();
  inline virtual ~PGE_183 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;
};

#endif // PGE_183_H
