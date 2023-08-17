#ifndef PGE_115_H
#define PGE_115_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_115
 : public olc::PixelGameEngine
{
 public:
  PGE_115 ();
  inline virtual ~PGE_115 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> positions_;
  std::vector<olc::vf2d> velocities_;
  std::vector<olc::vf2d> accelerations_;
};

#endif // PGE_115_H
