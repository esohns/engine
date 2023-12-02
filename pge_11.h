#ifndef PGE_11_H
#define PGE_11_H

#include <vector>

#include "olcPixelGameEngine.h"
#include "olcPGEX_AdditionalColours.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_11
 : public olc::PixelGameEngine
{
 public:
  PGE_11 ();
  ~PGE_11 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct ball
  {
    olc::vf2d pos;
    olc::vf2d vel;
    olc::vf2d acc;
    float radius;
  };

  void add (const olc::vf2d&, float = 40.0f);
  //float map (float, float, float, float, float);

  std::vector<ball> balls_;
};

#endif // PGE_11_H
