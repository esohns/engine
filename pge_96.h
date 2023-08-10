#ifndef PGE_96_H
#define PGE_96_H

#include "olcPixelGameEngine.h"

class PGE_96
 : public olc::PixelGameEngine
{
 public:
  PGE_96 ();
  inline virtual ~PGE_96 () {}

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
};

#endif // PGE_96_H
