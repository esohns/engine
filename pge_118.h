#ifndef PGE_118_H
#define PGE_118_H

#include "olcPixelGameEngine.h"

class PGE_118
 : public olc::PixelGameEngine
{
 public:
  PGE_118 ();
  inline virtual ~PGE_118 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void harom (float ax, float ay, float bx, float by, int level, float ratio);
};

#endif // PGE_118_H
