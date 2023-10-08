#ifndef PGE_195_H
#define PGE_195_H
// *TODO*: I failed to port this sketch successfully: https://openprocessing.org/sketch/1559054

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_195
 : public olc::PixelGameEngine
{
 public:
  PGE_195 ();
  inline virtual ~PGE_195 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<float> rx_;
  std::vector<float> ry_;
  std::vector<float> xx_;
  std::vector<float> yy_;
  float              mw_;
  int32_t            t_;
  float              fpx_;
  float              fpy_;

  void restart ();
};

#endif // PGE_195_H
