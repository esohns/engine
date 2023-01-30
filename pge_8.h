#ifndef PGE_8_H
#define PGE_8_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_8
 : public olc::PixelGameEngine
{
 public:
  PGE_8 ();
  ~PGE_8 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct vec2d
  {
    float x, y;
  };
 
  void buildPointsOnLine (struct vec2d*, float, float, float, float, int, int);
  void buildNShapePerimeter (struct vec2d*, float, float, int, int, int, int);
  void buildPointsOnCircle (struct vec2d*, float, float, int, int, int);
  void buildRectangleOutline (struct vec2d*, float, float, float, float, int, int);
  struct vec2d* morph (struct vec2d*, struct vec2d*, float, int);
  void build ();
  void shuffle ();

  struct vec2d* points;
  struct vec2d* line;
  struct vec2d* square;
  struct vec2d* circle;
  struct vec2d* triangle;
  struct vec2d* poly;
  int polySides = 5;
  int polyRadius = 60;

  struct vec2d* objects[5];
  int totalObjects = 5;
  int currentObject = 0;
  int nextObject = 1;

  int totalPoints = 10000;

  float transitionTime = 1.0f;
  float currentTransitionTime = 0.0f;
  bool isMorphing = false;
  bool shufflePoints = true;

  double twoPi = M_PI * 2; // 360 degrees but in Radians
};

#endif // PGE_5_H
