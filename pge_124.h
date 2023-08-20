#ifndef PGE_124_H
#define PGE_124_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

#include "defines_5.h"

class PGE_124
 : public olc::PixelGameEngine
{
 public:
  PGE_124 ();
  inline virtual ~PGE_124 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class point
  {
   public:
    olc::vf2d position_;
    olc::vf2d target_;
    int       rank_;
    int       countExclude_;
    olc::vf2d speed_;
    olc::vf2d exclusion_;
    olc::vf2d mouseAction_;

    point (float x, float y, int rank)
     : position_ (x, y)
     , target_ (position_)
     , rank_ (rank)
     , countExclude_ (0)
     , speed_ ()
     , exclusion_ ()
     , mouseAction_ ()
    {}

    void process (olc::PixelGameEngine* engine_in,
                  std::vector<point>& points_in)
    {
      olc::vf2d mouse_position (static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ()));
      float d = mouse_position.dist (position_);
      if (d < ENGINE_PGE_124_DEFAULT_MAX_DIST_MOUSE)
      {
        float f = 0.13f * (d - ENGINE_PGE_124_DEFAULT_MAX_DIST_MOUSE) / d;
        mouseAction_ = mouse_position - position_;
        mouseAction_ *= f;
      } // end IF

      olc::vf2d diff;
      for (int i = rank_ + 1; i < ENGINE_PGE_124_DEFAULT_NUMBER_OF_POINTS; i++)
      {
        d = points_in[i].position_.dist (position_);
        if (d < ENGINE_PGE_124_DEFAULT_MAX_DIST_POINTS && d > 0.0f)
        {
          diff = position_ - points_in[i].position_;
          diff = diff.norm ();
          diff *= ENGINE_PGE_124_DEFAULT_MAX_DIST_POINTS - d;
          exclusion_ += diff;
          countExclude_++;

          diff *= -1.0f;
          points_in[i].exclusion_ += diff;
          points_in[i].countExclude_++;
        } // end IF
      } // end FOR

      if (countExclude_ > 0)
      {
        exclusion_ /= static_cast<float> (countExclude_);
        exclusion_ *= ENGINE_PGE_124_DEFAULT_FRICTION_POINTS;
      } // end IF
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      exclusion_ += mouseAction_;
      speed_ += exclusion_;
      speed_ *= ENGINE_PGE_124_DEFAULT_FRICTION_AIR;
      position_ += speed_;

      if (position_.x < 0.0f || position_.x >= static_cast<float> (engine_in->ScreenWidth ()))
      {
        if (position_.x < 0.0f) position_.x = 0.0f;
        if (position_.x >= static_cast<float> (engine_in->ScreenWidth ())) position_.x = static_cast<float> (engine_in->ScreenWidth () - 1);
        speed_.x *= -1.1f;
      } // end IF
      if (position_.y < 0.0f || position_.y >= static_cast<float> (engine_in->ScreenHeight ()))
      {
        if (position_.y < 0.0f) position_.y = 0.0f;
        if (position_.y >= static_cast<float> (engine_in->ScreenHeight ())) position_.y = static_cast<float> (engine_in->ScreenHeight () - 1);
        speed_.y *= -1.1f;
      } // end IF

      exclusion_ *= 0.0f;
      mouseAction_ *= 0.0f;
      countExclude_ = 0;
    }
  };

  class triangle
  {
   public:
    olc::vf2d* p1;
    olc::vf2d* p2;
    olc::vf2d* p3;
    bool       isBad;

    //olc::vf2d center ()
    //{
    //  return line_intersector::simpleIntersect (p1.x, p1.y, (p2.x + p3.x) * 0.5f, (p2.y + p3.y) * 0.5f, p2.x, p2.y, (p3.x + p1.x) * 0.5f, (p3.y + p1.y) * 0.5f);
    //}

    bool almost_equal (triangle& other)
    {
      return (Common_Math_Tools::almost_equal (p1->x, other.p1->x, 2) &&
              Common_Math_Tools::almost_equal (p1->y, other.p1->y, 2) &&
              Common_Math_Tools::almost_equal (p2->x, other.p2->x, 2) &&
              Common_Math_Tools::almost_equal (p2->y, other.p2->y, 2) &&
              Common_Math_Tools::almost_equal (p3->x, other.p3->x, 2) &&
              Common_Math_Tools::almost_equal (p3->y, other.p3->y, 2));
    }

    bool containsVertex (olc::vf2d& v)
    {
      return (v == *p1) || (v == *p2) || (v == *p3);
    }

    bool circumCircleContains (olc::vf2d& v)
    {
      float ab = p1->mag2 ();
      float cd = p2->mag2 ();
      float ef = p3->mag2 ();

      float ax = p1->x;
      float ay = p1->y;
      float bx = p2->x;
      float by = p2->y;
      float cx = p3->x;
      float cy = p3->y;

      float circum_x = (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
      float circum_y = (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

      olc::vf2d circum (circum_x / 2.0f, circum_y / 2.0f);
      float circum_radius = p1->dist (circum) * p1->dist (circum);
      float dist = v.dist (circum) * v.dist (circum);
      return dist <= circum_radius;
    }
  };

  class circum_circle
  {
   public:
    static bool circumCircle (olc::vf2d& p, triangle& t, olc::vf2d& circle, float& z)
    {
      static float EPSILON = 9.999999747378752E-5f;
      float m1, m2, mx1, mx2, my1, my2, dx, dy, rsqr, drsqr;

      /* Check for coincident points */
      if (std::abs (t.p1->y - t.p2->y) < EPSILON && std::abs (t.p2->y - t.p3->y) < EPSILON)
      {
        //println("CircumCircle: Points are coincident.");
        return false;
      } // end IF

      if (std::abs (t.p2->y - t.p1->y) < EPSILON)
      {
        m2 = -(t.p3->x - t.p2->x) / (t.p3->y - t.p2->y);
        mx2 = (t.p2->x + t.p3->x) * 0.5f;
        my2 = (t.p2->y + t.p3->y) * 0.5f;
        circle.x = (t.p2->x + t.p1->x) * 0.5f;
        circle.y = m2 * (circle.x - mx2) + my2;
      } // end IF
      else if (std::abs (t.p3->y - t.p2->y) < EPSILON)
      {
        m1 = -(t.p2->x - t.p1->x) / (t.p2->y - t.p1->y);
        mx1 = (t.p1->x + t.p2->x) * 0.5f;
        my1 = (t.p1->y + t.p2->y) * 0.5f;
        circle.x = (t.p3->x + t.p2->x) * 0.5f;
        circle.y = m1 * (circle.x - mx1) + my1;
      } // end ELSE IF
      else
      {
        m1 = -(t.p2->x - t.p1->x) / (t.p2->y - t.p1->y);
        m2 = -(t.p3->x - t.p2->x) / (t.p3->y - t.p2->y);
        mx1 = (t.p1->x + t.p2->x) * 0.5f;
        mx2 = (t.p2->x + t.p3->x) * 0.5f;
        my1 = (t.p1->y + t.p2->y) * 0.5f;
        my2 = (t.p2->y + t.p3->y) * 0.5f;
        circle.x = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        circle.y = m1 * (circle.x - mx1) + my1;
      } // end ELSE

      dx = t.p2->x - circle.x;
      dy = t.p2->y - circle.y;
      rsqr = dx * dx + dy * dy;
      z = std::sqrt (rsqr);

      dx = p.x - circle.x;
      dy = p.y - circle.y;
      drsqr = dx * dx + dy * dy;

      return drsqr <= rsqr;
    }
  };

  class line_intersector
  {
   public:
    static bool intersect (float a_x1, float a_y1, float a_x2, float a_y2,
                           float b_x1, float b_y1, float b_x2, float b_y2,
                           olc::vf2d& p)
    {
      static float e = 1e-5f;

      float d = ((b_y2 - b_y1) * (a_x2 - a_x1)) - ((b_x2 - b_x1) * (a_y2 - a_y1));
      if (std::abs (d) < e) return false;

      float na, nb, ma, mb;
      na = ((b_x2 - b_x1) * (a_y1 - b_y1)) - ((b_y2 - b_y1) * (a_x1 - b_x1));
      nb = ((a_x2 - a_x1) * (a_y1 - b_y1)) - ((a_y2 - a_y1) * (a_x1 - b_x1));
      ma = na / d;
      mb = nb / d;
      if (ma < 0 || ma>1 || mb < 0 || mb>1) return false;

      p.x = a_x1 + (ma * (a_x2 - a_x1));
      p.y = a_y1 + (ma * (a_y2 - a_y1));

      return true;
    }

    static olc::vf2d simpleIntersect (float a_x1, float a_y1, float a_x2, float a_y2,
                                      float b_x1, float b_y1, float b_x2, float b_y2)
    {
      float na = ((b_x2 - b_x1) * (a_y1 - b_y1)) - ((b_y2 - b_y1) * (a_x1 - b_x1)),
             d = ((b_y2 - b_y1) * (a_x2 - a_x1)) - ((b_x2 - b_x1) * (a_y2 - a_y1)),
             ma = na / d;
      return olc::vf2d (a_x1 + (ma * (a_x2 - a_x1)), a_y1 + (ma * (a_y2 - a_y1)));
    }
  };

  class triangulator
  {
   public:
    class edge
    {
     public:
      olc::vf2d* p1, *p2;
      bool       isBad;

      edge (olc::vf2d& p1_in, olc::vf2d& p2_in)
       : p1 (&p1_in)
       , p2 (&p2_in)
       , isBad (false)
      {}

      bool operator!= (edge& other)
      {
        return !(*this == other);
      }
      bool operator== (edge& other)
      {
        return ((*p1 == *other.p1 && *p2 == *other.p2) ||
                (*p1 == *other.p2 && *p2 == *other.p1));
      }
      bool almost_equal (edge& other)
      {
        return ((Common_Math_Tools::almost_equal (p1->x, other.p1->x, 2) &&
                 Common_Math_Tools::almost_equal (p2->x, other.p2->x, 2) &&
                 Common_Math_Tools::almost_equal (p1->y, other.p1->y, 2) &&
                 Common_Math_Tools::almost_equal (p2->y, other.p2->y, 2)) ||
                (Common_Math_Tools::almost_equal (p1->x, other.p2->x, 2) &&
                 Common_Math_Tools::almost_equal (p2->x, other.p1->x, 2) &&
                 Common_Math_Tools::almost_equal (p1->y, other.p2->y, 2) &&
                 Common_Math_Tools::almost_equal (p2->y, other.p1->y, 2)));
      }
    };

    static bool sharedVertex (triangle& t1, triangle& t2)
    {
      return t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3 ||
             t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3 ||
             t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3;
    }

    static void sortXList (std::vector<olc::vf2d*>& ps)
    {
      int l = static_cast<int> (ps.size ());
      olc::vf2d* p1, *p2, *pi;
      int r;
      for (int i = 0; i < l - 1; i++)
      {
        pi = ps[i];
        p1 = pi;
        p2 = pi;
        r = i;
        for (int j = i + 1; j < l; j++)
        {
          p1 = ps[j];
          if (p1->x < p2->x)
          {
            p2 = p1;
            r = j;
          } // end IF
        } // end FOR
        if (r != i)
        {
          ps[r] = pi;
          ps[i] = p2;
        } // end IF
      } // end FOR
    }

    static void triangulate (std::vector<olc::vf2d*>& pxyz, std::vector<triangle>& triangles)
    {
      sortXList (pxyz);

      float xmin = pxyz[0]->x,
            ymin = pxyz[0]->y,
            xmax = xmin,
            ymax = ymin;
      for (std::vector<olc::vf2d*>::iterator iterator = pxyz.begin ();
           iterator != pxyz.end ();
           ++iterator)
      {
        if ((*iterator)->x < xmin) xmin = (*iterator)->x;
        else if ((*iterator)->x > xmax) xmax = (*iterator)->x;
        if ((*iterator)->y < ymin) ymin = (*iterator)->y;
        else if ((*iterator)->y > ymax) ymax = (*iterator)->y;
      } // end FOR
      float dx = xmax - xmin,
            dy = ymax - ymin,
            dmax = dx > dy ? dx : dy,
            two_dmax = dmax * 2.0f,
            xmid = (xmax + xmin) * 0.5f,
            ymid = (ymax + ymin) * 0.5f;
      olc::vf2d p1, p2, p3;
      p1 = {xmid - two_dmax, ymid - dmax};
      p2 = {xmid, ymid + two_dmax};
      p3 = {xmid + two_dmax, ymid - dmax};

      triangle super_triangle;
      super_triangle.p1 = &p1;
      super_triangle.p2 = &p2;
      super_triangle.p3 = &p3;
      super_triangle.isBad = false;
      triangles.push_back (super_triangle);

      olc::vf2d circle;
      float z;
      bool inside;
      std::vector<triangle> complete;
      for (std::vector<olc::vf2d*>::iterator iterator = pxyz.begin ();
           iterator != pxyz.end ();
           ++iterator)
      {
        std::vector<edge> edges;

        for (int j = static_cast<int> (triangles.size () -1); j >= 0; j--)
        {
          bool complete_b = false;
          for (std::vector<triangle>::iterator iterator_2 = complete.begin ();
              iterator_2 != complete.end ();
              ++iterator_2)
            if (triangles[j].almost_equal (*iterator_2))
            {
              complete_b = true;
              break;
            } // end IF
          if (complete_b)
            continue;

          circle = {0.0f, 0.0f};
          z = 0.0f;
          inside = circum_circle::circumCircle (**iterator, triangles[j], circle, z);
          //inside = triangles[j].circumCircleContains (**iterator);
          if (circle.x + z < (*iterator)->x)
            complete.push_back (triangles[j]);
          if (inside)
          {
            edges.push_back (edge (*triangles[j].p1, *triangles[j].p2));
            edges.push_back (edge (*triangles[j].p2, *triangles[j].p3));
            edges.push_back (edge (*triangles[j].p3, *triangles[j].p1));
            triangles[j].isBad = true;
          } // end IF
        } // end FOR
        triangles.erase (std::remove_if (triangles.begin (), triangles.end (),
                                         [] (triangle& t) { return t.isBad; }),
                         triangles.end ());

        for (std::vector<edge>::iterator iterator_3 = edges.begin ();
             iterator_3 != edges.end ();
             ++iterator_3)
          for (std::vector<edge>::iterator iterator_4 = iterator_3 + 1;
               iterator_4 != edges.end ();
               ++iterator_4)
          {
            if ((*iterator_3).almost_equal (*iterator_4))
            //if (*iterator_3 == *iterator_4)
            {
              (*iterator_3).isBad = true;
              (*iterator_4).isBad = true;
            } // end IF
          } // end FOR
        edges.erase (std::remove_if (edges.begin (), edges.end (),
                                     [](edge& e) { return e.isBad; }), edges.end ());

        for (std::vector<edge>::iterator iterator_3 = edges.begin ();
             iterator_3 != edges.end ();
             ++iterator_3)
        {
          triangle triangle_s;
          triangle_s.p1 = (*iterator_3).p1;
          triangle_s.p2 = (*iterator_3).p2;
          triangle_s.p3 = *iterator;
          triangle_s.isBad = false;
          triangles.push_back (triangle_s);
        } // end FOR
      } // end FOR

      for (int i = static_cast<int> (triangles.size () - 1); i >= 0; i--)
        if (sharedVertex (triangles [i], super_triangle))
        //if (triangles[i].containsVertex (p1) || triangles[i].containsVertex (p2) || triangles[i].containsVertex (p3))
          triangles[i].isBad = true;
      triangles.erase (std::remove_if (triangles.begin (), triangles.end (),
                                       [] (triangle& t) { return t.isBad; }),
                       triangles.end ());
    }
  };

  std::vector<point>      points_;
  std::vector<olc::vf2d*> positions_;

  void reset ();
};

#endif // PGE_124_H
