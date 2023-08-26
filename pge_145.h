#ifndef PGE_145_H
#define PGE_145_H

#include "olcPixelGameEngine.h"

class PGE_145
 : public olc::PixelGameEngine
{
 public:
  PGE_145 ();
  inline virtual ~PGE_145 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float c_;
  float t_;
};

#endif // PGE_145_H
