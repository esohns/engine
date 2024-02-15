#ifndef PGE_364_H
#define PGE_364_H

#include "olcPixelGameEngine.h"

class PGE_364
 : public olc::PixelGameEngine
{
 public:
  PGE_364 ();
  inline virtual ~PGE_364 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float minSize_, maxSize_;
  float hueFactor_;
  float theta_;
};

#endif // PGE_364_H
