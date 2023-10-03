#ifndef PGE_189_H
#define PGE_189_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_189
 : public olc::PixelGameEngine
{
 public:
  PGE_189 ();
  inline virtual ~PGE_189 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                  angle_;
  olc::vf2d              p0_;
  std::vector<olc::vf2d> points_;

  void shadow ();
};

#endif // PGE_189_H
