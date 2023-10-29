#ifndef PGE_217_H
#define PGE_217_H

#include "olcPixelGameEngine.h"

class PGE_217
 : public olc::PixelGameEngine
{
 public:
  PGE_217 ();
  inline virtual ~PGE_217 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float h_;
  float delta_;

  void draw_circle_arc (olc::vf2d&, float, float, float, olc::Pixel&);
  void makeLine (olc::vf2d&, float, float, olc::Pixel&, olc::Pixel&);
  olc::vf2d lerpPoints (float, float, float, float, float);
};

#endif // PGE_217_H
