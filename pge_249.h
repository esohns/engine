#ifndef PGE_249_H
#define PGE_249_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_249
 : public olc::PixelGameEngine
{
 public:
  PGE_249 ();
  inline virtual ~PGE_249 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                          u_;
  std::vector<std::vector<int> > tiles_;
  std::vector<std::vector<int> > gapsPossibilities_;
  std::vector<int>               prevHorizGaps_;
  std::vector<int>               prevVertGaps_;
  std::vector<int>               horizGaps_;
  std::vector<int>               vertGaps_;

  void getGapsPossibilities (std::vector<int>&);
  std::vector<int> generateGaps ();
  float easeInOutExpo (float);
};

#endif // PGE_249_H
