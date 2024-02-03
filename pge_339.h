#ifndef PGE_339_H
#define PGE_339_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_339
 : public olc::PixelGameEngine
{
 public:
  PGE_339 ();
  inline virtual ~PGE_339 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> prev_;
  int                    t_;

  olc::vf2d move (olc::vf2d&, float, float);
};

#endif // PGE_339_H
