#ifndef PGE_156_H
#define PGE_156_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_156
 : public olc::PixelGameEngine
{
 public:
  PGE_156 ();
  inline virtual ~PGE_156 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> particles_;
  int32_t                y_;
};

#endif // PGE_156_H
