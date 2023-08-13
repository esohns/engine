#ifndef PGE_103_H
#define PGE_103_H

#include "olcPixelGameEngine.h"

class PGE_103
 : public olc::PixelGameEngine
{
 public:
  PGE_103 ();
  inline virtual ~PGE_103 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void tile ();
  void form (int32_t, int32_t, float, int32_t, olc::Pixel&, olc::Pixel&);
};

#endif // PGE_103_H
