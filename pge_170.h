#ifndef PGE_170_H
#define PGE_170_H

#include <utility>
#include <vector>

#include "olcPixelGameEngine.h"

class PGE_170
 : public olc::PixelGameEngine
{
 public:
  PGE_170 ();
  inline virtual ~PGE_170 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int                               index_;
  std::vector<olc::vf2d>            particles_;
  std::vector<std::pair<int, int> > lines_;
};

#endif // PGE_170_H
