#ifndef PGE_161_H
#define PGE_161_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_161
 : public olc::PixelGameEngine
{
 public:
  PGE_161 ();
  inline virtual ~PGE_161 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> particles_;
  struct reference_point
  {
    olc::vf2d position_;
    float     ratio_;
  };
  std::vector<struct reference_point> referencePoints_;
};

#endif // PGE_161_H
