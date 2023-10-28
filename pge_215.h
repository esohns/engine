#ifndef PGE_215_H
#define PGE_215_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_215
 : public olc::PixelGameEngine
{
 public:
  PGE_215 ();
  inline virtual ~PGE_215 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> points1_;
  std::vector<olc::vf2d> points2_;
  olc::Pixel             color1_;
  olc::Pixel             color2_;
  olc::Pixel             white_;
  float                  diameter_;
  float                  idx_;

  std::vector<olc::vf2d> makeStar (float, float, float, float);
  void drawStarStep (std::vector<olc::vf2d>&, float, olc::Pixel&, olc::Pixel&, float);
};

#endif // PGE_215_H
