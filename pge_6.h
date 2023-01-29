#ifndef PGE_6_H
#define PGE_6_H

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_6
 : public olc::PixelGameEngine
{
 public:
  PGE_6 ();
  ~PGE_6 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct fire_pallete
  {
    int red;
    int green;
    int blue;
  };

  void initializePalette (struct fire_pallete[]);
  void moveFire ();
  void moveUp ();
  void blurScreen ();
  void blurPixel (int);
  int getColor (int);
  void draw ();

  int WIDTH = ENGINE_PGE_6_DEFAULT_WIDTH;
  int HEIGHT = ENGINE_PGE_6_DEFAULT_HEIGHT;
  int SCREEN_SIZE = WIDTH * HEIGHT;
  int HEIGHT_MINUS_BOTTOM_LINE = sizeof (int) * (SCREEN_SIZE - WIDTH);

  struct fire_pallete pallete[256];
  int* fireBuffer = NULL;
  int numberOfFires = ENGINE_PGE_6_DEFAULT_NUMBER_OF_FIRES;
  int fire[ENGINE_PGE_6_DEFAULT_NUMBER_OF_FIRES];
};

#endif // PGE_5_H
