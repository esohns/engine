#ifndef PGE_354_H
#define PGE_354_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_354
 : public olc::PixelGameEngine
{
 public:
  PGE_354 ();
  inline virtual ~PGE_354 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<float> X;
  std::vector<float> Y;
  std::vector<float> FX;
  std::vector<float> FY;
  std::vector<float> VX;
  std::vector<float> VY;
  std::vector<int>   Q;
  int                OKNG;
  float              L;
  float              KX, KY;
  float              DX, DY;
  int                N;
  int                I, II, III;
  int                W;
  float              A;
  float              C;
};

#endif // PGE_354_H
