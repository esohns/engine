#ifndef PGE_231_H
#define PGE_231_H

#include <list>

#include "olcPixelGameEngine.h"

class PGE_231
 : public olc::PixelGameEngine
{
 public:
  PGE_231 ();
  inline virtual ~PGE_231 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::list<olc::vf2d> makePolygon (float, int, float);
  std::list<olc::vf2d> makeBlob (float, float);
};

#endif // PGE_231_H
