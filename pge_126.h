#ifndef PGE_126_H
#define PGE_126_H

#include "olcPixelGameEngine.h"

class PGE_126
 : public olc::PixelGameEngine
{
 public:
  PGE_126 ();
  inline virtual ~PGE_126 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void paintCircle (float t, float o);
};

#endif // PGE_126_H
