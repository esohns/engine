#ifndef PGE_303_H
#define PGE_303_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_303
 : public olc::PixelGameEngine
{
 public:
  PGE_303 ();
  inline virtual ~PGE_303 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct circle
  {
    olc::vf2d position;
    float     d;
  };
  std::vector<struct circle>      circles_;

  struct segment
  {
    olc::vf2d start;
    olc::vf2d end;
  };
  std::vector<struct segment>     segments_;

  struct rectangle
  {
    olc::vf2d position;
    float     w;
    float     h;
    float     a;
  };
  std::vector<struct rectangle>   rectangles_;

  struct triangle
  {
    olc::vf2d p1;
    olc::vf2d p2;
    olc::vf2d p3;
  };
  std::vector<struct triangle>    triangles_;

  std::vector<olc::vf2d>          points_;
  int                             pointer_;
  float                           endtime_;

  void reset ();
  void gradients (int);
  float sdf (olc::vf2d&);

  inline float sdXor (float a, float b)
  { // Xor of two
    return std::max (std::min (a, b), -std::max (a, b));
  }

  inline olc::vf2d sdRotate (olc::vf2d& s, float a)
  { // sample point, angle to rotate
    return olc::vf2d (std::cos (a) * s.x + std::sin (a) * s.y, std::cos (a) * s.y - std::sin (a) * s.x);
  }

  float sdCircle (olc::vf2d& p, float r)
  { // sample pvector, radius  -  centered on origin
    return (p.mag () - r);
  }

  float sdSegment (olc::vf2d& p, olc::vf2d& a, olc::vf2d& b)
  { // sample point, segment end points
    olc::vf2d pa = p - a;
    olc::vf2d ba =  b - a;
    if (ba.mag () > 0.0f)
    { // not a point
      float h = std::max (std::min (pa.dot (ba) / ba.dot (ba), 1.0f), 0.0f);
      ba *= h;
      pa -= ba;
    } // end IF
    return pa.mag ();
  }

  float sdBox (olc::vf2d& p, float w, float h)
  { // sample pvector, b.x = width, b.y = height  - centered on origin
    olc::vf2d d (std::abs (p.x), std::abs (p.y));
    d -= {w, h};
    olc::vf2d d1 (std::max (d.x, 0.0f), std::max (d.y, 0.0f));
    return d1.mag () + std::min (std::max (d.x, d.y), 0.0f);
  }

  float sdTriangle (olc::vf2d& p, olc::vf2d& p0, olc::vf2d& p1, olc::vf2d& p2)
  { // sample point, triangle end points
    olc::vf2d e0 = p1 - p0;
    olc::vf2d e1 = p2 - p1;
    olc::vf2d e2 = p0 - p2;
    olc::vf2d v0 = p - p0;
    olc::vf2d v1 = p - p1;
    olc::vf2d v2 = p - p2;
    olc::vf2d pq0 = v0 - (e0 * std::min (std::max (v0.dot (e0) / e0.dot (e0), 0.0f), 1.0f));
    olc::vf2d pq1 = v1 - (e1 * std::min (std::max (v1.dot (e1) / e1.dot (e1), 0.0f), 1.0f));
    olc::vf2d pq2 = v2 - (e2 * std::min (std::max (v2.dot (e2) / e2.dot (e2), 0.0f), 1.0f));
    float s = e0.x * e2.y - e0.y * e2.x;
    olc::vf2d d (std::min (std::min (pq0.dot (pq0), pq1.dot (pq1)), pq2.dot (pq2)),
                 std::min (std::min (s * (v0.x * e0.y - v0.y * e0.x), s * (v1.x * e1.y - v1.y * e1.x)), s * (v2.x * e2.y - v2.y * e2.x)));
    if (d.y == std::abs (d.y))
      return -std::sqrt (d.x);
    return std::sqrt (d.x);
  }
};

#endif // PGE_303_H
