#ifndef GLUT_338_H
#define GLUT_338_H

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

#include "common_gl_common.h"

#include "common_image_tools.h"

#include "defines_15.h"

// GLUT routines
void engine_glut_338_reshape (int, int);
void engine_glut_338_key (unsigned char, int, int);
void engine_glut_338_key_special (int, int, int);
void engine_glut_338_menu (int);
void engine_glut_338_mouse_button (int, int, int, int);
void engine_glut_338_mouse_move (int, int);
void engine_glut_338_timer (int);
void engine_glut_338_draw (void);
void engine_glut_338_idle (void);
void engine_glut_338_visible (int);

//////////////////////////////////////////

class particle
{
 public:
  particle (float x, float y, float z)
   : position_ (x, y, z)
  {}

  void update (float scaleFactor, std::vector<std::vector<std::vector<float> > >& coeffs, std::vector<std::vector<int> >& coeff_2o_guide, float dt)
  {
    std::vector<float> xyz = {position_.x * scaleFactor, position_.y * scaleFactor, position_.z * scaleFactor};
    std::vector<float> dxyz = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 3; i++)
    {
      dxyz[i] += coeffs[i][0][0];
      for (int j = 0; j < 3; j++)
        dxyz[i] += coeffs[i][1][j] * xyz[j];
      for (int j = 0; j < 6; j++)
        dxyz[i] += coeffs[i][2][j] * xyz[coeff_2o_guide[j][0]] * xyz[coeff_2o_guide[j][1]];
    } // end FOR
    glm::vec3 dxyz_vec (dxyz[0], dxyz[1], dxyz[2]);
    float dxyz_mag = glm::length (dxyz_vec);
    for (int i = 0; i < 3; i++)
      dxyz[i] *= dt / dxyz_mag;
    position_.x += dxyz[0];
    position_.y += dxyz[1];
    position_.z += dxyz[2];
  }

  void draw (glm::vec3& centerVector, glm::vec3& xProjVector, glm::vec3& yProjVector, glm::vec3& zProjVector, float zoom, bool wireframe, float width)
  {
    glm::vec3 posVec = position_ - centerVector;
    float zVal = glm::dot (posVec, zProjVector);
    float z_colorFactor = std::min (std::max (zVal * zoom * 1.5f, -1.0f), 1.0f);

    float r, g, b;
    Common_Image_Tools::HSVToRGB (((255.0f * (1 - z_colorFactor) / 2) / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
    glColor3f (r, g, b);

    glPushMatrix ();
    glTranslatef ((glm::dot (posVec, xProjVector) * zoom + 1) * (width / 2.0f),
                  (glm::dot (posVec, yProjVector) * zoom + 1) * (width / 2.0f),
                  0.0f);
    wireframe ? glutWireSphere  ((width / 160.0f) * (2.0f / (z_colorFactor + 2.0f)), 16, 16)
              : glutSolidSphere ((width / 160.0f) * (2.0f / (z_colorFactor + 2.0f)), 16, 16);
    glPopMatrix ();
  }

  glm::vec3 position_;
};

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_338_CBData
{
  //
  std::vector<particle>                          particles;
  glm::vec3                                      centerVector;
  glm::vec3                                      xProjVector;
  glm::vec3                                      yProjVector;
  glm::vec3                                      zProjVector;
  std::vector<std::vector<std::vector<float> > > coefficients;
  std::vector<std::vector<int> >                 coefficients2oGuide;
  float                                          zoom;
  float                                          scaleFactor;
  float                                          dt;

  // menu
  bool                                           wireframe;

  // camera
  struct Common_GL_Camera                        camera;
};

#endif // GLUT_338_H
