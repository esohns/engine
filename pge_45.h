#ifndef PGE_45_H
#define PGE_45_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_45
 : public olc::PixelGameEngine
{
 public:
  PGE_45 ();
  inline virtual ~PGE_45 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> path_;

  olc::vf2d hilbert (int);
};

#endif // PGE_45_H
