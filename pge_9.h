#ifndef PGE_9_H
#define PGE_9_H

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

#define DISPLAY_DOTS 0
#define DISPLAY_PATTERN 1

// *NOTE*: see also: https://www.ime.usp.br/~otuyama/stereogram/basic/index.html
//  and              https://github.com/synesthesiam/magicpy
//  and              https://github.com/kaltinril/olcGames/stereogram
class PGE_9
 : public olc::PixelGameEngine
{
 public:
  PGE_9 ();
  ~PGE_9 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void dotPattern (int);
  void stampPattern (int);
  void addSprite (int, int);

  olc::Sprite* threeDImage;
  olc::Sprite* backgroundPattern;
  int state = DISPLAY_DOTS;
  int invert_display = 1;
};

#endif // PGE_5_H
