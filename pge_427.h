#ifndef PGE_427_H
#define PGE_427_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_19.h"

class PGE_427
 : public olc::PixelGameEngine
{
 public:
  PGE_427 ();
  inline virtual ~PGE_427 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y, int l)
     : position (x, y)
     , velocity (Common_Tools::getRandomNumber (-1.0f, 1.0f), Common_Tools::getRandomNumber (-1.0f, 1.0f))
     , level (l)
     , life (0)
    {
      velocity = velocity.norm ();
      velocity *= Common_GL_Tools::map (static_cast<float> (l), 0.0f, static_cast<float> (ENGINE_PGE_427_DEFAULT_MAX_LEVEL), 5.0f, 2.0f);
    }

    void move (std::vector<particle>& particles)
    {
      velocity *= 0.9f; // friction
      position += velocity;

      ++life;
      if (life % 10 == 0)
      {
        if (level > 0)
        {
          level--;
          particles.push_back (particle (position.x, position.y, level - 1));
        } // end IF
      } // end IF
    }

    olc::vf2d position;
    olc::vf2d velocity;
    int       level;
    int       life;
  };

  struct triangle
  {
    olc::vf2d p1, p2, p3;
  };
  struct triangle superTriangle (std::vector<olc::vf2d>&);
  struct circum_circle
  {
    circum_circle ()
     : i (0), j (0), k (0)
     , center ()
     , radius (0.0f)
     , is_valid (false)
    {}

    int       i, j, k;
    olc::vf2d center;
    float     radius;
    bool      is_valid;
  };
  struct circum_circle circumCircle (std::vector<olc::vf2d>&, int, int, int);
  void deDuplicate (std::vector<std::pair<int, int> >&);
  // delaunay-triangulation
  std::vector<int> triangulate (std::vector<olc::vf2d>&);

  std::vector<particle> particles_;
  bool                  fill_;
};

#endif // PGE_427_H
