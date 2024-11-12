#ifndef PGE_405_H
#define PGE_405_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_405
 : public olc::PixelGameEngine
{
 public:
  PGE_405 ();
  inline virtual ~PGE_405 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<float> cr_;
  std::vector<float> beta_;
  std::vector<float> theta_;
  std::vector<float> zeta_;
  std::vector<float> eta_;
};

#endif // PGE_405_H
  