#ifndef GLUT_284_H
#define GLUT_284_H

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

#include "common_tools.h"

#include "common_gl_common.h"

#include "defines_13.h"

// GLUT routines
void engine_glut_284_reshape (int, int);
void engine_glut_284_key (unsigned char, int, int);
void engine_glut_284_key_special (int, int, int);
void engine_glut_284_menu (int);
void engine_glut_284_mouse_button (int, int, int, int);
void engine_glut_284_mouse_move (int, int);
void engine_glut_284_timer (int);
void engine_glut_284_draw (void);
void engine_glut_284_idle (void);
void engine_glut_284_visible (int);

class ring_parent
{
 public:
  ring_parent (float minW, glm::vec2& xyOut, float rOut, int level, Common_GL_Color_t& colParent)
   : xyOut_ (xyOut)
   , rOut_ (rOut)
   , rIn_ (rOut * 0.8f)
   , rInAngle_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
   , xyIn_ (xyOut.x + (rOut_ - rIn_) * std::cos (rInAngle_),
            xyOut.y + (rOut_ - rIn_) * std::sin (rInAngle_))
   , level_ (level)
   , maxLevel_ (2)
   , h_ (0.0f)
   , color_ ()
   , rotateAngle_ (0.0f)
   , rotateSpeed_ ()
   , children_ ()
  {
    h_ = (minW / 20.0f) * 0.5f + level_ * (minW / 20.0f) * Common_Tools::getRandomNumber (0.8f, 1.2f);

    static Common_GL_Color_t palette_a[6] =
      { Common_GL_Color_t (0xaf, 0x3e, 0x4d), Common_GL_Color_t (0x2e, 0x86, 0xab), Common_GL_Color_t (0x75, 0x8e, 0x4f),
        Common_GL_Color_t (0x00, 0x2A, 0x32), Common_GL_Color_t (0xf6, 0xae, 0x2d), Common_GL_Color_t (0xFA, 0xC9, 0xB8) };
    int col_index = Common_Tools::getRandomNumber (0, 5);
    color_ = palette_a[col_index];
    while ((color_.r == colParent.r) && (color_.g == colParent.g) && (color_.b == colParent.b))
    {
      col_index = Common_Tools::getRandomNumber (0, 5);
      color_ = palette_a[col_index];
    } // end WHILE

    rotateSpeed_ =
      1.3f * Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI) / 300.0f) / (rOut_ / minW) * 0.4f * Common_Tools::getRandomNumber (-1.0f, 1.0f);

    if (level_ <= maxLevel_)
      createChildren (minW);
  }

  void createChildren (float minW)
  {
    int numChildren = 3;//150;
    float maxChildROut = rOut_ / 3.0f;
    children_.clear ();

    for (int i = 0; i < numChildren; i++)
    {
      glm::vec2 xy = setChildCentXy ();
      while (checkInChildren (xy))
        xy = setChildCentXy ();
      float r = setChildR (xy, maxChildROut);
      children_.push_back (ring_parent (minW, xy, r, level_ + 1, color_));
    } // end FOR
  }

  glm::vec2 setChildCentXy ()
  {
    float d = Common_Tools::getRandomNumber (0.0f, rOut_);
    float ang = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    glm::vec2 xy = xyOut_ + glm::vec2 (d * std::cos (ang), d * std::sin (ang));

    return xy;
  }

  bool checkInChildren (glm::vec2& xy)
  {
    bool isInChildren = false;
    for (int i = 0; i < static_cast<int> (children_.size ()); i++)
    {
      float d = glm::distance (xy, children_[i].xyOut_);
      if (d < children_[i].rOut_)
      {
        isInChildren = true;
        break;
      } // end IF
    } // end FOR

    return isInChildren;
  }

  float setChildR (glm::vec2& xy, float maxChildROut)
  {
    float r = maxChildROut;
    for (int i = 0; i < static_cast<int> (children_.size ()); i++)
    {
      float d = glm::distance (xy, children_[i].xyOut_);
      float rNew = d - children_[i].rOut_;
      if (rNew < r)
        r = rNew;
    } // end FOR

    float outlineClearanceRatio = 0.99f;
    float d = glm::distance (xy, xyOut_);
    float rNew_2 = std::max (0.0f, (rOut_ * outlineClearanceRatio) - d);
    if (rNew_2 < r)
      r = rNew_2;

    return r;
  }

  void drawPillar (glm::vec2& xyOut, float rOut, Common_GL_Color_t& colOut, float h, bool wireframe)
  {
    glColor3f (colOut.r / 255.0f, colOut.g / 255.0f, colOut.b / 255.0f);
    static int numEllipse = 1;
    for (int i = 0; i < numEllipse; i++)
    {
      glPushMatrix ();
      glTranslatef (xyOut.x, xyOut.y, 0.0f);
      glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI_2), 1.0f, 0.0f, 0.0f);
      float r = rOut / static_cast<float> (numEllipse * (numEllipse - i));
      glTranslatef (0.0f, -h / 2.0f, 0.0f);
      glTranslatef (0.0f, 0.0f, -h / 2.0f);
      wireframe ? glutWireCylinder (r, h, 64, 1)
                : glutSolidCylinder (r, h, 64, 1);//_minW / r * 1 / 10);
      glPopMatrix ();
    } // end FOR
  }

  void draw (bool wireframe)
  {
    glPushMatrix ();

    glTranslatef (xyOut_.x, xyOut_.y, 0.0f);
    //glRotatef ((180.0f / static_cast<float> (M_PI)) * rotateAngle_, 0.0f, 0.0f, 1.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * rotateAngle_, 0.0f, 1.0f, 0.0f);
    glTranslatef (-xyOut_.x, -xyOut_.y, 0.0f);

    drawPillar (xyOut_, rOut_, color_, h_, wireframe);

    if (level_ <= maxLevel_)
      for (int i = 0; i < static_cast<int> (children_.size ()); i++)
        children_[i].draw (wireframe);
    glPopMatrix ();

    rotateAngle_ += rotateSpeed_;
  }

  glm::vec2                xyOut_;
  float                    rOut_;
  float                    rIn_;
  float                    rInAngle_;
  glm::vec2                xyIn_;
  int                      level_;
  int                      maxLevel_;
  float                    h_;
  Common_GL_Color_t        color_;
  float                    rotateAngle_;
  float                    rotateSpeed_;
  std::vector<ring_parent> children_;
};

struct Engine_OpenGL_GLUT_284_CBData
{
  //
  float                    minW;
  std::vector<ring_parent> rings;

  // menu
  bool                     wireframe;

  // camera
  struct Common_GL_Camera  camera;

  // mouse
  float                    angle;
  float                    deltaAngle;
  int                      xOrigin;
};

//////////////////////////////////////////

void setObject (struct Engine_OpenGL_GLUT_284_CBData&);

#endif // GLUT_284_H
