#ifndef PGE_130_H
#define PGE_130_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_130
 : public olc::PixelGameEngine
{
 public:
  PGE_130 ();
  inline virtual ~PGE_130 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> positions_;
};

#endif // PGE_130_H
