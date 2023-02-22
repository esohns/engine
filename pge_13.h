#ifndef PGE_13_H
#define PGE_13_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_13
 : public olc::PixelGameEngine
{
 public:
  PGE_13 ();
  ~PGE_13 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    olc::vf2d pos;
    olc::vf2d vel;
    olc::vf2d acc;
    olc::Pixel col;
    float fLifespan;
    bool bIsExplosion;

    int id;
  };

  std::vector<struct particle> fireworks;
  float gravity = 60.0f;

  void add (const olc::vf2d&, const olc::vf2d&, const olc::Pixel& = olc::WHITE, bool = false);
  void explode (const olc::vf2d&);
};

#endif // PGE_13_H
