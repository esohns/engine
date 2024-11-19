#ifndef PGE_422_H
#define PGE_422_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_422
 : public olc::PixelGameEngine
{
 public:
  PGE_422 ();
  inline virtual ~PGE_422 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<uint8_t> grid_;
  int32_t              m_;
  std::vector<float>   mat_;
  float                zoom_;
};

#endif // PGE_422_H
