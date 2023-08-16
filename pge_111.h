#ifndef PGE_111_H
#define PGE_111_H

#include "olcPixelGameEngine.h"

class PGE_111
 : public olc::PixelGameEngine
{
 public:
  PGE_111 ();
  inline virtual ~PGE_111 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float theta_;

  void draw_circle_arc (olc::vf2d&, float, float, float, olc::Pixel&);
};

#endif // PGE_111_H
