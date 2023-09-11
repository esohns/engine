#ifndef PGE_182_H
#define PGE_182_H

#include <list>
#include <vector>

#include "olcPixelGameEngine.h"

class PGE_182
 : public olc::PixelGameEngine
{
 public:
  PGE_182 ();
  inline virtual ~PGE_182 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::list<olc::vf2d>   blackHoles_;
  std::vector<olc::vf2d> stars_;
};

#endif // PGE_182_H
