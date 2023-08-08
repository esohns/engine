#ifndef PGE_89_H
#define PGE_89_H

#include "olcPixelGameEngine.h"

class PGE_89
 : public olc::PixelGameEngine
{
 public:
  PGE_89 ();
  inline virtual ~PGE_89 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  olc::Sprite b1_;
  olc::Sprite b2_;
  int32_t     yPos1_;
  int32_t     yPos2_;
  bool        isFirst_;
  olc::Pixel  palette_[31];

  void drawRows (int32_t, int);
  void drawRow (int32_t, int);
  void drawVertical (int32_t, int32_t, olc::Pixel&);
  void drawCrossing (int32_t, int32_t, bool, olc::Pixel&, olc::Pixel&);
};

#endif // PGE_89_H
