#ifndef PGE_230_H
#define PGE_230_H

#include "olcPixelGameEngine.h"

class PGE_230
 : public olc::PixelGameEngine
{
 public:
  PGE_230 ();
  inline virtual ~PGE_230 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float squareRadius (float);
  float easeOutElastic (float);
  float easeInElastic (float);
};

#endif // PGE_230_H
