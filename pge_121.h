#ifndef PGE_121_H
#define PGE_121_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_121
 : public olc::PixelGameEngine
{
 public:
  PGE_121 ();
  inline virtual ~PGE_121 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> cells_;
  int32_t                nbCellW_;
  int32_t                nbCellH_;
};

#endif // PGE_121_H
