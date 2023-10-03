#ifndef PGE_191_H
#define PGE_191_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_191
 : public olc::PixelGameEngine
{
 public:
  PGE_191 ();
  inline virtual ~PGE_191 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> points_;
};

#endif // PGE_191_H
