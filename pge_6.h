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
  struct fire_palette
  {
    int red;
    int green;
    int blue;
  };

  void initializePalette (struct fire_palette[]);
  void moveFire ();
  void moveUp ();
  void blurScreen ();
  void blurPixel (int);
  int getColor (int);
  void draw ();

  int WIDTH = ENGINE_PGE_6_DEFAULT_WIDTH;
  int HEIGHT = ENGINE_PGE_6_DEFAULT_HEIGHT;
  int SCREEN_SIZE = WIDTH * HEIGHT;
  int HEIGHT_MINUS_BOTTOM_LINE = sizeof (ACE_UINT32) * (SCREEN_SIZE - WIDTH);

  struct fire_palette palette[256];
  ACE_UINT32* fireBuffer = NULL;
  int numberOfFires = ENGINE_PGE_6_DEFAULT_NUMBER_OF_FIRES;
  ACE_UINT32 fire[ENGINE_PGE_6_DEFAULT_NUMBER_OF_FIRES];
};

#endif // PGE_6_H
