#include "stdafx.h"

#include "pge_8.h"

#include <algorithm>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_8::PGE_8 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 8";
}

PGE_8::~PGE_8 ()
{
}

void
PGE_8::buildPointsOnLine (struct vec2d* points,
                          float x1, float y1, float x2, float y2,
                          int arrayStart, int numberOfSteps)
{
  float xSlope = (x2 - x1) / float(numberOfSteps - 1);
  float ySlope = (y2 - y1) / float(numberOfSteps - 1);

  for (int i = 0; i < numberOfSteps; i++)
  {
    int arrayPos = arrayStart + i;
    points[arrayPos].x = x1 + (xSlope * i);
    points[arrayPos].y = y1 + (ySlope * i);
  } // end FOR
}

void
PGE_8::buildNShapePerimeter (struct vec2d* points,
                             float centerX, float centerY, int radius,
                             int arrayStart, int numberOfSteps, int numberOfSides)
{
  int stepsPerSide = numberOfSteps / numberOfSides;
  float angleToNextSide = float(twoPi) / numberOfSides;

  for (int i = 0; i < numberOfSides; i++)
  {
    float x1 = centerX + cosf (angleToNextSide * i) * radius;
    float y1 = centerY + sinf (angleToNextSide * i) * radius;

    float x2 = centerX + cosf (angleToNextSide * (i + 1)) * radius;
    float y2 = centerY + sinf (angleToNextSide * (i + 1)) * radius;

    buildPointsOnLine (points, x1, y1, x2, y2, arrayStart + (i * stepsPerSide), stepsPerSide);
  } // end FOR
}

void
PGE_8::buildPointsOnCircle (struct vec2d* points,
                            float centerX, float centerY, int radius,
                            int arrayStart, int numberOfSteps)
{
  float angleStep = float(twoPi) / numberOfSteps;

  for (int i = 0; i < numberOfSteps; i++)
  {
    float x = cosf (angleStep * i);
    float y = sinf (angleStep * i);

    points[i].x = centerX + (x * radius);
    points[i].y = centerY + (y * radius);
  } // end FOR
}

void
PGE_8::buildRectangleOutline (struct vec2d* points,
                              float x1, float y1, float x2, float y2,
                              int arrayStart, int numberOfSteps)
{
  int stepsPerSide = numberOfSteps / 4;

  buildPointsOnLine (points, x1, y1, x1, y2, arrayStart + (0 * stepsPerSide), stepsPerSide);
  buildPointsOnLine (points, x1, y1, x2, y1, arrayStart + (1 * stepsPerSide), stepsPerSide);
  buildPointsOnLine (points, x2, y1, x2, y2, arrayStart + (2 * stepsPerSide), stepsPerSide);
  buildPointsOnLine (points, x1, y2, x2, y2, arrayStart + (3 * stepsPerSide), stepsPerSide);
}

struct PGE_8::vec2d*
PGE_8::morph (struct vec2d* currentShape, struct vec2d* targetShape,
              float percentComplete, int numberOfPoints)
{
  struct vec2d* points = new struct vec2d[numberOfPoints];

  for (int i = 0; i < numberOfPoints; i++)
  {
    float xSlope = (targetShape[i].x - currentShape[i].x) * percentComplete;
    float ySlope = (targetShape[i].y - currentShape[i].y) * percentComplete;

    points[i].x = currentShape[i].x + xSlope;
    points[i].y = currentShape[i].y + ySlope;
  } // end FOR

  return points;
}

void
PGE_8::build ()
{
  buildPointsOnLine (line, 30, 50, 200, 220, 0, totalPoints);
  buildRectangleOutline (square, 10, 10, 100, 100, 0, totalPoints);
  buildPointsOnCircle (circle, 170, 70, 50, 0, totalPoints);
  buildNShapePerimeter (poly, ScreenWidth () / 2.0f, ScreenHeight () / 2.0f, polyRadius, 0, totalPoints, polySides);
  buildNShapePerimeter (triangle, ScreenWidth () / 2.0f, ScreenHeight () / 2.0f, 40, 0, totalPoints, 3);
}

void
PGE_8::shuffle ()
{
  // obtain a time-based seed:
  static unsigned seed =
    static_cast<unsigned int> (std::chrono::system_clock::now ().time_since_epoch ().count ());

  std::shuffle (line, line + totalPoints, std::default_random_engine (seed));
  std::shuffle (square, square + totalPoints, std::default_random_engine (seed));
  std::shuffle (poly, poly + totalPoints, std::default_random_engine (seed));
  std::shuffle (triangle, triangle + totalPoints, std::default_random_engine (seed));

  //std::random_shuffle (line, line + totalPoints);
  //std::random_shuffle (square, square + totalPoints);
  //std::random_shuffle (poly, poly + totalPoints);
  //std::random_shuffle (triangle, triangle + totalPoints);
}

bool
PGE_8::OnUserCreate ()
{
  square = new struct vec2d[totalPoints];
  circle = new struct vec2d[totalPoints];
  line = new struct vec2d[totalPoints];
  poly = new struct vec2d[totalPoints];
  triangle = new struct vec2d[totalPoints];

  objects[0] = line;
  objects[1] = triangle;
  objects[2] = square;
  objects[3] = poly;
  objects[4] = circle;

  build ();
  if (shufflePoints)
    shuffle ();

  points = line;

  return true;
}

bool
PGE_8::OnUserUpdate (float fElapsedTime)
{
  for (int i = 0; i < totalPoints; i++)
  {
    uint8_t colorShade = static_cast<uint8_t> ((250.0f * (i / float (totalPoints))) + 5.0f);
    Draw (static_cast<int32_t> (points[i].x), static_cast<int32_t> (points[i].y), olc::Pixel (colorShade, colorShade, colorShade));
    //Draw (points[i].x, points[i].y, olc::WHITE);
  }

  if (GetKey (olc::Key::B).bPressed)
    build ();

  if (GetKey(olc::Key::S).bPressed)
    shuffle ();

  if (GetMouse(0).bPressed)
    isMorphing = true;


  if (GetMouse(1).bPressed && !isMorphing)
  {
    nextObject = nextObject - 1;

    if (nextObject < 0)
      nextObject = totalObjects - 1;

    if (nextObject == currentObject)
      nextObject = nextObject - 1;

    if (nextObject < 0)
      nextObject = totalObjects - 1;

    isMorphing = true;
  } // end IF

  if (isMorphing)
  {
    float percentComplete = currentTransitionTime / transitionTime;
    if (percentComplete >= 1.0f)
    {
      percentComplete = 1.0f;
      isMorphing = false;
    } // end IF

    points = morph (objects[currentObject], objects[nextObject], percentComplete, totalPoints);

    if (!isMorphing)
    {
      currentTransitionTime = 0.0f;
      currentObject = nextObject;
      nextObject = (nextObject + 1) % totalObjects;
    } // end IF

    currentTransitionTime = currentTransitionTime + fElapsedTime;
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
