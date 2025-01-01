#ifndef GLUT_454_H
#define GLUT_454_H

#include <map>
#include <vector>

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_21.h"

//////////////////////////////////////////

enum sides { S_FRONT = 0, S_BACK, S_TOP, S_BOTTOM, S_LEFT, S_RIGHT };
enum directions { D_UP = 0, D_RIGHT, D_DOWN, D_LEFT };
static std::map<enum sides, std::vector<enum sides> > adjacencies;
static std::map<enum sides, std::vector<int> >        rotations;

struct position
{
  glm::vec2  pos;
  enum sides side;
};

class agent
{
 public:
  agent ()
    : positions ()
    , direction (static_cast<enum directions> (Common_Tools::getRandomNumber (0, 3)))
    , speed (Common_Tools::getRandomNumber (static_cast<float> (ENGINE_GLUT_454_DEFAULT_MIN_SPEED), static_cast<float> (ENGINE_GLUT_454_DEFAULT_MAX_SPEED)))
    , width (Common_Tools::getRandomNumber (static_cast<float> (ENGINE_GLUT_454_DEFAULT_MIN_WIDTH), static_cast<float> (ENGINE_GLUT_454_DEFAULT_MAX_WIDTH)))
  {
    struct position pos;
    pos.pos = {Common_Tools::getRandomNumber (-ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f),
               Common_Tools::getRandomNumber (-ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f)};
    pos.side = static_cast<enum sides> (Common_Tools::getRandomNumber (0, 5));
    positions.push_back (pos);
  }

  struct position rotatePoint (int angle, struct position& coord)
  {
    struct position result = coord;
    int angleInDegrees = angle % 360;
    switch (angleInDegrees)
    {
      case 0:
        break;
      case 90:
        result.pos = { -coord.pos.y, coord.pos.x };
        break;
      case 180:
        result.pos = { -coord.pos.x, -coord.pos.y };
        break;
      case 270:
        result.pos = { coord.pos.y, -coord.pos.x };
        break;
      default:
        ACE_ASSERT (false);
        break;
    } // end SWITCH

    return result;
  }

  struct break_position_result
  {
    break_position_result ()
     : positions ()
     , isValid (false)
    {}

    std::vector<struct position> positions;
    bool                         isValid;
  };
  struct break_position_result breakPosition (struct position& lastPos, struct position& newPos)
  {
    static float s = ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f;
    int rotationAngle;
    struct break_position_result p;
    struct position pos;
    if (newPos.pos.x < -s)
    {
      rotationAngle = rotations[lastPos.side][3];
      pos.pos = { -s, lastPos.pos.y };
      pos.side = lastPos.side;
      p.positions.push_back (pos);
      pos.pos = { s, lastPos.pos.y };
      pos.side = adjacencies[lastPos.side][3];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      pos.pos = { 2.0f * s + newPos.pos.x, lastPos.pos.y };
      pos.side = adjacencies[lastPos.side][3];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      p.isValid = true;
    } // end iF
    else if (newPos.pos.x > s)
    {
      rotationAngle = rotations[lastPos.side][1];
      pos.pos = { s, lastPos.pos.y };
      pos.side = lastPos.side;
      p.positions.push_back (pos);
      pos.pos = { -s, lastPos.pos.y };
      pos.side = adjacencies[lastPos.side][1];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      pos.pos = { speed + lastPos.pos.x + (2.0f * -s), lastPos.pos.y };
      pos.side = adjacencies[lastPos.side][1];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      p.isValid = true;
    } // end IF
    if (newPos.pos.y < -s)
    {
      rotationAngle = rotations[lastPos.side][0];
      pos.pos = { lastPos.pos.x, -s };
      pos.side = lastPos.side;
      p.positions.push_back(pos);
      pos.pos = { lastPos.pos.x, s };
      pos.side = adjacencies[lastPos.side][0];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      pos.pos = { lastPos.pos.x, 2.0f * s + newPos.pos.y };
      pos.side = adjacencies[lastPos.side][0];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      p.isValid = true;
    } // end IF
    else if (newPos.pos.y > s)
    {
      rotationAngle = rotations[lastPos.side][2];
      pos.pos = { lastPos.pos.x, s };
      pos.side = lastPos.side;
      p.positions.push_back (pos);
      pos.pos = { lastPos.pos.x, -s };
      pos.side = adjacencies[lastPos.side][2];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      pos.pos = { lastPos.pos.x, speed + lastPos.pos.y + (2.0f * -s) };
      pos.side = adjacencies[lastPos.side][2];
      p.positions.push_back (rotatePoint (rotationAngle, pos));
      p.isValid = true;
    } // end IF
    if (p.isValid)
      direction = static_cast<enum directions> ((direction + rotationAngle / 90) % 4);

    return p;
  }

  void update ()
  {
    if (Common_Tools::testRandomProbability (ENGINE_GLUT_454_DEFAULT_W_CHANCE))
    {
      if (Common_Tools::testRandomProbability (0.5f))
        direction = static_cast<enum directions> ((direction + 1) % 4);
      else
        direction = static_cast<enum directions> ((direction + 3) % 4);
    } // end IF

    struct position lastPos = positions.back ();
    struct position newPos;
    switch (direction)
    {
      case D_UP:
        newPos.pos.x = lastPos.pos.x;
        newPos.pos.y = lastPos.pos.y - speed;
        break;
      case D_DOWN:
        newPos.pos.x = lastPos.pos.x;
        newPos.pos.y = lastPos.pos.y + speed;
        break;
      case D_LEFT:
        newPos.pos.x = lastPos.pos.x - speed;
        newPos.pos.y = lastPos.pos.y;
        break;
      case D_RIGHT:
        newPos.pos.x = lastPos.pos.x + speed;
        newPos.pos.y = lastPos.pos.y;
        break;
      default:
        ACE_ASSERT (false);
        break;
    } // end SWITCH
    newPos.side = lastPos.side;
    struct break_position_result breakPositionResult = breakPosition (lastPos, newPos);
    if (breakPositionResult.isValid)
      positions.insert (positions.end (), breakPositionResult.positions.begin (), breakPositionResult.positions.end ());
    else
      positions.push_back (newPos);
    
    if (positions.size () > ENGINE_GLUT_454_DEFAULT_NUMBER_OF_SEGMENTS)
    {
      std::vector<struct position> new_positions (positions.end () - ENGINE_GLUT_454_DEFAULT_NUMBER_OF_SEGMENTS, positions.end ());
      positions = new_positions;
    } // end IF
  }

  void show ()
  {
    for (int i = 0; i < static_cast<int> (positions.size ()) - 1; i++)
      if (likely (positions[i].side == positions[i + 1].side))
      {
        glPushMatrix ();
        float x1 = positions[i].pos.x;
        float y1 = positions[i].pos.y;
        float x2 = positions[i + 1].pos.x;
        float y2 = positions[i + 1].pos.y;

        switch (positions[i].side)
        {
          case S_FRONT:
            glTranslatef (0.0f, 0.0f, ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f);
            break;
          case S_BACK:
            glTranslatef (0.0f, 0.0f, -ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f);
            glRotatef (180.0f, 0.0f, 1.0f, 0.0f);
            break;
          case S_TOP:
            glTranslatef (0.0f, -ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f);
            glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
            break;
          case S_BOTTOM:
            glTranslatef (0.0f, ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f);
            glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
            break;
          case S_RIGHT:
            glTranslatef (ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f, 0.0f);
            glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
            break;
          case S_LEFT:
            glTranslatef (-ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f, 0.0f);
            glRotatef (-90.0f, 0.0f, 1.0f, 0.0f);
            break;
          default:
            break;
        } // end SWITCH

        uint8_t color =
          static_cast<uint8_t> (Common_GL_Tools::map (i, 0, static_cast<int> (positions.size () - 1), 255, 0));
        glColor3ub (color, color, color);
        if (x1 == x2)
        {
          glTranslatef (-width / 2.0f, -std::abs (y2 - y1) / 2.0f, 0.0f);
          glRectf ((x1 + x2) / 2.0f, (y1 + y2) / 2.0f,
                   (x1 + x2) / 2.0f + width, (y1 + y2) / 2.0f + std::abs (y2 - y1));
        } // end IF
        else
        {
          glTranslatef (-std::abs (x2 - x1) / 2.0f, -width / 2.0f, 0.0f);
          glRectf ((x1 + x2) / 2.0f, (y1 + y2) / 2.0f,
                   (x1 + x2) / 2.0f + std::abs (x2 - x1), (y1 + y2) / 2.0f + width);
        } // end ELSE
        glPopMatrix ();
      } // end IF
  }

  std::vector<struct position> positions;
  enum directions              direction;
  float                        speed;
  float                        width;
};

//////////////////////////////////////////

// GLUT routines
void engine_glut_454_reshape (int, int);
void engine_glut_454_key (unsigned char, int, int);
void engine_glut_454_key_special (int, int, int);
void engine_glut_454_menu (int);
void engine_glut_454_mouse_button (int, int, int, int);
void engine_glut_454_mouse_move (int, int);
void engine_glut_454_timer (int);
void engine_glut_454_draw (void);
void engine_glut_454_idle (void);
void engine_glut_454_visible (int);

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_454_CBData
{
  // menu
  bool                                         wireframe;

  // camera
  struct Engine_GL_Camera                      camera;

  // mouse
  int                                          mousex;
  int                                          mousey;
  bool                                         mouse_0_ButtonDown;

  // scene
  std::vector<agent>                           agents;
};

#endif // GLUT_454_H
