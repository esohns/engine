#ifndef PGE_179_H
#define PGE_179_H

#include "olcPixelGameEngine.h"

class PGE_179
 : public olc::PixelGameEngine
{
 public:
  PGE_179 ();
  inline virtual ~PGE_179 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;
};

#endif // PGE_179_H
