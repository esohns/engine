#ifndef PGE_157_H
#define PGE_157_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_157
 : public olc::PixelGameEngine
{
 public:
  PGE_157 ();
  inline virtual ~PGE_157 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> flowers_;
};

#endif // PGE_157_H
