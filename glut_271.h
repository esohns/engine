#ifndef GLUT_271_H
#define GLUT_271_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "engine_common.h"

#include "defines_12.h"

// GLUT routines
void engine_glut_271_reshape (int, int);
void engine_glut_271_key (unsigned char, int, int);
void engine_glut_271_key_special (int, int, int);
void engine_glut_271_menu (int);
void engine_glut_271_mouse_button (int, int, int, int);
void engine_glut_271_mouse_move (int, int);
void engine_glut_271_timer (int);
void engine_glut_271_draw (void);
void engine_glut_271_idle (void);
void engine_glut_271_visible (int);

struct triangle
{
  glm::vec3 p1;
  glm::vec3 p2;
  glm::vec3 p3;
};

struct Engine_OpenGL_GLUT_271_CBData
{
  //
  float                   maxAreaR;
  std::vector<float>      aryR;
  std::vector<glm::vec3>  arySphereCenter;
  glm::vec3               initNoiseXYZ;
  glm::vec3               initRot;
  glm::vec3               noiseRangeXYZ;
  float                   sphereR;
  int                     count;
  std::vector<std::vector<struct triangle> > aryTriangle;
  std::vector<std::vector<glm::vec3> >       aryTriCenter;
  std::vector<std::vector<glm::vec2> >       aryCentRotYZ;

  // noise
  //noise::module::Perlin   noise;
  OpenSimplexNoise        noise;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

class sub_triangle
{
 public:
  sub_triangle (struct triangle& triangle, int countObj, float r, int index,
                struct Engine_OpenGL_GLUT_271_CBData& cbData)
   : newMidXYZ_1_2 ()
   , newMidXYZ_2_3 ()
   , newMidXYZ_3_1 ()
  {
    if (countObj <= ENGINE_GLUT_271_DEFAULT_LIMIT_COUNT)
    {
      divide (triangle.p1, triangle.p2, triangle.p3, r, index, cbData);
      std::vector<struct triangle> triangles_a;
      struct triangle triangle_s;
      triangle_s.p1 = triangle.p1;
      triangle_s.p2 = newMidXYZ_1_2;
      triangle_s.p3 = newMidXYZ_3_1;
      triangles_a.push_back (triangle_s);
      triangle_s.p1 = newMidXYZ_1_2;
      triangle_s.p2 = triangle.p2;
      triangle_s.p3 = newMidXYZ_2_3;
      triangles_a.push_back (triangle_s);
      triangle_s.p1 = newMidXYZ_3_1;
      triangle_s.p2 = newMidXYZ_2_3;
      triangle_s.p3 = triangle.p3;
      triangles_a.push_back (triangle_s);
      triangle_s.p1 = newMidXYZ_1_2;
      triangle_s.p2 = newMidXYZ_2_3;
      triangle_s.p3 = newMidXYZ_3_1;
      triangles_a.push_back (triangle_s);
      for (int i = 0; i < static_cast<int> (triangles_a.size ()); i++)
        sub_triangle (triangles_a[i], countObj + 1, r, index, cbData);
    } // end IF
    else
    {
      addCenter (triangle, index, cbData); //[[x1, y1, z1], [x2, y2, z2], [x3, y3, z3]] -> [ave.x, ave.y, ave.z]
      addCentRotYZ (index, cbData); //angle X-Z plane, angle X-Y plane
      cbData.aryTriangle[index].push_back (triangle);
    } // end ELSE
  }

  void divide (glm::vec3& XYZ1, glm::vec3& XYZ2, glm::vec3& XYZ3, float r, int index,
               struct Engine_OpenGL_GLUT_271_CBData& cbData)
  {
    glm::vec3 midXYZ_1_2 ((XYZ1.x + XYZ2.x) / 2.0f,
                          (XYZ1.y + XYZ2.y) / 2.0f,
                          (XYZ1.z + XYZ2.z) / 2.0f);
    glm::vec3 midXYZ_2_3 ((XYZ2.x + XYZ3.x) / 2.0f,
                          (XYZ2.y + XYZ3.y) / 2.0f,
                          (XYZ2.z + XYZ3.z) / 2.0f);
    glm::vec3 midXYZ_3_1 ((XYZ3.x + XYZ1.x) / 2.0f,
                          (XYZ3.y + XYZ1.y) / 2.0f,
                          (XYZ3.z + XYZ1.z) / 2.0f);
    float distMid_1_2 = std::sqrt (
      (midXYZ_1_2.x - cbData.arySphereCenter[index].x) * (midXYZ_1_2.x - cbData.arySphereCenter[index].x) +
      (midXYZ_1_2.y - cbData.arySphereCenter[index].y) * (midXYZ_1_2.y - cbData.arySphereCenter[index].y) +
      (midXYZ_1_2.z - cbData.arySphereCenter[index].z) * (midXYZ_1_2.z - cbData.arySphereCenter[index].z));
    float distMid_2_3 = std::sqrt (
      (midXYZ_2_3.x - cbData.arySphereCenter[index].x) * (midXYZ_2_3.x - cbData.arySphereCenter[index].x) +
      (midXYZ_2_3.y - cbData.arySphereCenter[index].y) * (midXYZ_2_3.y - cbData.arySphereCenter[index].y) +
      (midXYZ_2_3.z - cbData.arySphereCenter[index].z) * (midXYZ_2_3.z - cbData.arySphereCenter[index].z));
    float distMid_3_1 = std::sqrt (
      (midXYZ_3_1.x - cbData.arySphereCenter[index].x) * (midXYZ_3_1.x - cbData.arySphereCenter[index].x) +
      (midXYZ_3_1.y - cbData.arySphereCenter[index].y) * (midXYZ_3_1.y - cbData.arySphereCenter[index].y) +
      (midXYZ_3_1.z - cbData.arySphereCenter[index].z) * (midXYZ_3_1.z - cbData.arySphereCenter[index].z));

    newMidXYZ_1_2.x = (midXYZ_1_2.x - cbData.arySphereCenter[index].x) / distMid_1_2 * r + cbData.arySphereCenter[index].x;
    newMidXYZ_1_2.y = (midXYZ_1_2.y - cbData.arySphereCenter[index].y) / distMid_1_2 * r + cbData.arySphereCenter[index].y;
    newMidXYZ_1_2.z = (midXYZ_1_2.z - cbData.arySphereCenter[index].z) / distMid_1_2 * r + cbData.arySphereCenter[index].z;

    newMidXYZ_2_3.x = (midXYZ_2_3.x - cbData.arySphereCenter[index].x) / distMid_2_3 * r + cbData.arySphereCenter[index].x;
    newMidXYZ_2_3.y = (midXYZ_2_3.y - cbData.arySphereCenter[index].y) / distMid_2_3 * r + cbData.arySphereCenter[index].y;
    newMidXYZ_2_3.z = (midXYZ_2_3.z - cbData.arySphereCenter[index].z) / distMid_2_3 * r + cbData.arySphereCenter[index].z;

    newMidXYZ_3_1.x = (midXYZ_3_1.x - cbData.arySphereCenter[index].x) / distMid_3_1 * r + cbData.arySphereCenter[index].x;
    newMidXYZ_3_1.y = (midXYZ_3_1.y - cbData.arySphereCenter[index].y) / distMid_3_1 * r + cbData.arySphereCenter[index].y;
    newMidXYZ_3_1.z = (midXYZ_3_1.z - cbData.arySphereCenter[index].z) / distMid_3_1 * r + cbData.arySphereCenter[index].z;
  }

  void addCenter (struct triangle& triangle, int index,
                  struct Engine_OpenGL_GLUT_271_CBData& cbData)
  {
    float centX = 0.0f;
    centX += triangle.p1.x;
    centX += triangle.p2.x;
    centX += triangle.p3.x;
    centX /= 3.0f;

    float centY = 0.0f;
    centY += triangle.p1.y;
    centY += triangle.p2.y;
    centY += triangle.p3.y;
    centY /= 3.0f;

    float centZ = 0.0f;
    centZ += triangle.p1.z;
    centZ += triangle.p2.z;
    centZ += triangle.p3.z;
    centZ /= 3.0f;

    glm::vec3 center_s (centX, centY, centZ);
    cbData.aryTriCenter[index].push_back (center_s);
  }

  void addCentRotYZ (int index,
                     struct Engine_OpenGL_GLUT_271_CBData& cbData)
  {
    cbData.aryCentRotYZ[index].clear ();
    glm::vec2 rotation_s;
    for (int i = 0; i < static_cast<int> (cbData.aryTriCenter[index].size ()); i++)
      cbData.aryCentRotYZ[index].push_back (rotation_s);

    for (int i = 0; i < static_cast<int> (cbData.aryTriCenter[index].size ()); i++)
    {
      float rotY =
        std::atan2 (cbData.aryTriCenter[index][i].z - cbData.arySphereCenter[index].z,
                    cbData.aryTriCenter[index][i].x - cbData.arySphereCenter[index].x);
      float rx =
        std::sqrt ((cbData.aryTriCenter[index][i].z - cbData.arySphereCenter[index].z) * (cbData.aryTriCenter[index][i].z - cbData.arySphereCenter[index].z) +
                   (cbData.aryTriCenter[index][i].x - cbData.arySphereCenter[index].x) * (cbData.aryTriCenter[index][i].x - cbData.arySphereCenter[index].x));
      float ry = cbData.aryTriCenter[index][i].y - cbData.arySphereCenter[index].y;
      float rotZ = std::atan2 (ry, rx);
      rotation_s.x = rotY;
      rotation_s.y = rotZ;
      cbData.aryCentRotYZ[index][i] = rotation_s;
    } // end FOR
  }

  glm::vec3 newMidXYZ_1_2;
  glm::vec3 newMidXYZ_2_3;
  glm::vec3 newMidXYZ_3_1;
};

class base_triangle
{
 public:
  base_triangle (float r, int index, glm::vec3& aryCentXYZ,
                 struct Engine_OpenGL_GLUT_271_CBData& cbData)
  {
    std::vector<struct triangle> triangles_a;
    struct triangle triangle_s;
    triangle_s.p1.x =    r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =    r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =   -r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =   -r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =    r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =   -r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =    r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =   -r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =   -r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =   -r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =   -r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =   -r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =    r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =    r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =    r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =   -r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =    r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =    r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =    r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =   -r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =    r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);
    triangle_s.p1.x =   -r + aryCentXYZ.x; triangle_s.p1.y = 0.0f + aryCentXYZ.y; triangle_s.p1.z = 0.0f + aryCentXYZ.z;
    triangle_s.p2.x = 0.0f + aryCentXYZ.x; triangle_s.p2.y = 0.0f + aryCentXYZ.y; triangle_s.p2.z =   -r + aryCentXYZ.z;
    triangle_s.p3.x = 0.0f + aryCentXYZ.x; triangle_s.p3.y =    r + aryCentXYZ.y; triangle_s.p3.z = 0.0f + aryCentXYZ.z;
    triangles_a.push_back (triangle_s);

    std::vector<struct triangle> triangles_2;
    cbData.aryTriangle.push_back (triangles_2);
    std::vector<glm::vec3> centers_s;
    cbData.aryTriCenter.push_back (centers_s);
    std::vector<glm::vec2> rotations_s;
    cbData.aryCentRotYZ.push_back (rotations_s);
    int countObj = 0;
    for (int i = 0; i < static_cast<int> (triangles_a.size ()); i++)
      sub_triangle (triangles_a[i], countObj + 1, r, index, cbData);
  }
};

#endif // GLUT_271_H
