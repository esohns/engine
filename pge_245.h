#ifndef PGE_245_H
#define PGE_245_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_245
 : public olc::PixelGameEngine
{
 public:
  PGE_245 ();
  inline virtual ~PGE_245 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<std::vector<olc::vf2d> >                        points_;
  std::vector<std::vector<olc::vf2d> >                        newPoints_;
  std::vector<std::vector<olc::Pixel> >                       g_;
  std::vector<std::vector<std::pair<olc::vf2d, olc::vi2d> > > ds_;

  olc::vf2d genPoint (int, int);
  std::vector<std::vector<olc::vf2d> > genPoints ();
  std::vector<std::vector<olc::Pixel> > gradient (int, int, olc::Pixel&, olc::Pixel&, olc::Pixel&, olc::Pixel&);
  void drawMesh (std::vector<std::vector<olc::vf2d> >&);
};

#endif // PGE_245_H
