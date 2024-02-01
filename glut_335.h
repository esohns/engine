#ifndef GLUT_335_H
#define GLUT_335_H

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

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "common_gl_common.h"

#include "common_image_tools.h"

#include "defines_15.h"

// GLUT routines
void engine_glut_335_reshape (int, int);
void engine_glut_335_key (unsigned char, int, int);
void engine_glut_335_key_special (int, int, int);
void engine_glut_335_menu (int);
void engine_glut_335_mouse_button (int, int, int, int);
void engine_glut_335_mouse_move (int, int);
void engine_glut_335_timer (int);
void engine_glut_335_draw (void);
void engine_glut_335_idle (void);
void engine_glut_335_visible (int);

//////////////////////////////////////////

class cube
{
 public:
  cube (int _numTileAng, int _numTileY, float _cylinderR, float _cylinderY, int index, float _torusR)
   : index_ (index)
   , stepAngle_ ((2.0f * static_cast<float> (M_PI)) / static_cast<float> (_numTileAng))
   , r_ (_cylinderR)
   , maxY_ (_cylinderY)
   , stepY_ (_cylinderY / static_cast<float> (_numTileY))
   , torusR_ (_torusR)
  {}

  void update (int _numTileAng, int _numTileY, bool wireframe,
               //noise::module::Perlin* noise,
               OpenSimplexNoise* noise,
               std::vector<std::vector<std::vector<float> > >& _aryInitNoiseXYZ,
               std::vector<float>& _aryNoiseRangeXYZ,
               int _count,
               float _noiseStepT,
               float _colH,
               float direction)
  {
    for (int i = 0; i < _numTileAng; i++)
      for (int j = 0; j < _numTileY; j++)
      {
        float x = r_ * std::cos (stepAngle_ * i);
        float z = -r_ * std::sin (stepAngle_ * i);
        float y = -maxY_ / 2.0f + stepY_ / 2.0f + stepY_ * j + stepY_ / static_cast<float> (_numTileAng) * 1.0f * i;
        std::vector<float> aryNoiseVal;
        for (int k = 0; k < ENGINE_GLUT_335_DEFAULT_NOISE_DIMENSION; k++)
          aryNoiseVal.push_back (calculateOSNoise (noise, _aryInitNoiseXYZ, _aryNoiseRangeXYZ, x, y, z, k, _count, _noiseStepT));
        draw (_numTileY, stepAngle_ * i, y, aryNoiseVal, j, wireframe, _colH, direction);
      } // end FOR
  }

  void draw (int _numTileY, float ang, float y, std::vector<float>& aryNoiseVal, int indexY, bool wireframe, float _colH, float direction)
  {
    float newR = torusR_ / static_cast<float> (_numTileY) * indexY;
    float h = torusR_ / static_cast<float> (_numTileY);

    glPushMatrix ();

    glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI_2), 1.0f, 0.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * -ang, 0.0f, 0.0f, 1.0f);

    glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI) * aryNoiseVal[1], 1.0f, 0.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI) * aryNoiseVal[2], 0.0f, 1.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI) * aryNoiseVal[3], 0.0f, 0.0f, 1.0f);

    glTranslatef (-r_, 0.0f, -maxY_ / 2.0f + maxY_ * aryNoiseVal[0]);

    static float ampAng1 = 3.0f;
    static float ampAng2 = 4.0f;
    float hue_f =
      (std::sin (_colH / 360.0f * 2.0f * static_cast<float> (M_PI) + static_cast<float> (M_PI) * ampAng1 / static_cast<float> (_numTileY) * indexY * direction + ang * ampAng2) * 0.5f + 0.5f) * 360.0f;
    float r, g, b;
    Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
    //specularMaterial(hue_f, 255, 255, 250);
    glColor4f (r, g, b, 1.0f - indexY / static_cast<float> (_numTileY));

    //float newR2 = std::max (0.0f, newR);
    //if (newR2 > 0.0f)
      wireframe ? glutWireTorus (h, newR, 32, 32)
                : glutSolidTorus (h, newR, 32, 32);
  
    glPopMatrix ();
  }

  float calculateOSNoise (//noise::module::Perlin* noise,
                          OpenSimplexNoise* noise,
                          std::vector<std::vector<std::vector<float> > >& _aryInitNoiseXYZ,
                          std::vector<float>& _aryNoiseRangeXYZ,
                          float x, float y, float z,
                          int i,
                          int _count,
                          float _noiseStepT)
  {
    //float noiseVal = static_cast<float> (noise->GetValue (
    float noiseVal = static_cast<float> (noise->Evaluate (
      _aryInitNoiseXYZ[index_][0][i] + _aryNoiseRangeXYZ[0] * x / r_,
      _aryInitNoiseXYZ[index_][1][i] + _aryNoiseRangeXYZ[1] * y / maxY_ + _count * _noiseStepT,
      _aryInitNoiseXYZ[index_][2][i] + _aryNoiseRangeXYZ[2] * z / r_
    )) * 0.5f + 0.5f;

    return noiseVal;
  }

  int   index_;
  float stepAngle_;
  float r_;
  float maxY_;
  float stepY_;
  float torusR_;
};

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_335_CBData
{
  //
  float                   cylinderR;
  float                   cylinderY;
  float                   torusR;
  float                   colH;
  float                   direction;
  std::vector<cube>       cubes;
  std::vector<std::vector<std::vector<float> > > initNoiseXYZ;
  std::vector<float>      noiseRangeXYZ;
  std::vector<float>      initRot;
  int                     count;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  int                     mouseX;
  int                     mouseY;

  // noise
  //noise::module::Perlin   noise;
  OpenSimplexNoise        noise;
};

#endif // GLUT_335_H
