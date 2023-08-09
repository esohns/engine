#ifndef PGE_94_H
#define PGE_94_H

#include "olcPixelGameEngine.h"

class PGE_94
 : public olc::PixelGameEngine
{
 public:
  PGE_94 ();
  inline virtual ~PGE_94 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d>               points_;
  std::vector<std::vector<olc::vi2d> > paths_;
  int32_t                              pointIdx_;
  float                                pointsPerFrame_;

  std::vector<olc::vi2d> possibleNeighbors (olc::vi2d&);
  bool inArray (olc::vi2d&, std::vector<olc::vi2d>&);
  void findPaths (olc::vi2d&, std::vector<olc::vi2d>&);
  void rsfc (float, float, float, bool, bool, int32_t);
  olc::Pixel rainbow (float);
  void draw_circle_arc (olc::vf2d&, float, float, float, olc::Pixel&);
};

#endif // PGE_94_H
