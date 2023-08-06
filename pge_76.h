#ifndef PGE_76_H
#define PGE_76_H

#include "olcPixelGameEngine.h"

class PGE_76
 : public olc::PixelGameEngine
{
 public:
  PGE_76 ();
  inline virtual ~PGE_76 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t numCircles_;
  int32_t numVertices_;

  void updateCntByMouse ();
  olc::vf2d getCenterByTheta (float, float, float);
  float getSizeByTheta (float, float, float);
  olc::Pixel getColorByTheta (float, float);
};

#endif // PGE_76_H
