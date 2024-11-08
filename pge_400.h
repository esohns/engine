#ifndef PGE_400_H
#define PGE_400_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_18.h"

class PGE_400
 : public olc::PixelGameEngine
{
 public:
  PGE_400 ();
  inline virtual ~PGE_400 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct splat
  {
    olc::vf2d              center;
    olc::Pixel             colour;
    float                  initialRadius;
    std::vector<olc::vf2d> points;
  };

  void setupPalette ();
  struct splat createSplat (olc::vf2d&, float, olc::Pixel&);
  void deformSplatBasedOn (struct splat&, struct splat&);
  void createAndAddOneSplat (olc::vf2d&, float, std::vector<struct splat>&);
  void restart ();
  void drawSplat (struct splat&);

  int                                     currentSplatLayerIndex_;
  olc::Pixel                              bgColor_;
  std::vector<olc::Pixel>                 palette_;
  std::vector<std::vector<struct splat> > splats_;
};

#endif // PGE_400_H
