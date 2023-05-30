#ifndef PGE_26_H
#define PGE_26_H

#include <array>

#include "olcPixelGameEngine.h"

#include "defines.h"

class PGE_26
  : public olc::PixelGameEngine
{
 public:
  PGE_26 ();
  inline virtual ~PGE_26 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  typedef std::array<std::array<ACE_INT32, ENGINE_PGE_26_DEFAULT_WIDTH>, ENGINE_PGE_26_DEFAULT_HEIGHT> sandpile_t;
  sandpile_t sandpile_;
  sandpile_t next_;
  olc::Pixel defaultColor_;
  olc::Pixel colors_[4];

  void topple ();
  void render ();
};

#endif // PGE_26_H
