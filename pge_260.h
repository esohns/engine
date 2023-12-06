#ifndef PGE_260_H
#define PGE_260_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_260
 : public olc::PixelGameEngine
{
 public:
  PGE_260 ();
  inline virtual ~PGE_260 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> positions_;
  std::vector<olc::vf2d> velocities_;
  int                    num_;

  void resetParticles ();
};

#endif // PGE_260_H
