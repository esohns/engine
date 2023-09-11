#ifndef PGE_181_H
#define PGE_181_H

#include "olcPixelGameEngine.h"

class PGE_181
 : public olc::PixelGameEngine
{
 public:
  PGE_181 ();
  inline virtual ~PGE_181 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> blackHoles_;
  std::vector<olc::vf2d> stars_;
};

#endif // PGE_181_H
