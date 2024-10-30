#ifndef PGE_378_H
#define PGE_378_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_378
 : public olc::PixelGameEngine
{
 public:
  PGE_378 ();
  inline virtual ~PGE_378 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<std::vector<float> > grid_;
  std::vector<std::vector<float> > velocity_;
};

#endif // PGE_378_H
