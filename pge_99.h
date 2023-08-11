#ifndef PGE_99_H
#define PGE_99_H

#include "olcPixelGameEngine.h"

class PGE_99
 : public olc::PixelGameEngine
{
 public:
  PGE_99 ();
  inline virtual ~PGE_99 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;

  void draw_circle_arc (olc::vf2d&, float, float, float, olc::Pixel&);
};

#endif // PGE_99_H
