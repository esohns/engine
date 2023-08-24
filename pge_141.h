#ifndef PGE_141_H
#define PGE_141_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_6.h"

class PGE_141
 : public olc::PixelGameEngine
{
 public:
  PGE_141 ();
  inline virtual ~PGE_141 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class point
  {
   public:
    float x;
    float y;
    float z;
    bool  stayOrGo_;

    point ()
     : x (Common_Tools::getRandomNumber (-1.0f, 1.0f))
     , y (-1.5f)
     , z (Common_Tools::getRandomNumber (0.1f, 0.6f))
    {}
  };

  class system
  {
   public:
    std::vector<point> points_;

    void spawn ()
    {
      points_.push_back (point ());
    }

    void update ()
    {
      points_.erase (std::remove_if (points_.begin (), points_.end (),
                                     [] (point& p) { return (p.y += 0.005f) >= 1.5f; }),
                     points_.end ());
    }

    float dist2 (point& a, point& b)
    {
      return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    }

    void connectPoints (olc::PixelGameEngine* engine_in,
                        float zoom)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      std::sort (points_.begin (), points_.end (),
                 [] (point& a, point& b) { return (a.z < b.z); });

      for (size_t i = points_.size (); i--;)
        for (size_t j = i; j--;)
        {
          float d = dist2 (points_[j], points_[i]);
          bool condition_b = true;
          for (std::vector<point>::iterator iterator = points_.begin ();
               iterator != points_.end ();
               ++iterator)
            if (d > dist2 (points_[i], *iterator) && d > dist2 (points_[j], *iterator))
            {
              condition_b = false;
              break;
            } // end IF
          if (condition_b)
          {
            float x1 = (points_[i].x / points_[i].z) * zoom;
            float y1 = (points_[i].y / points_[i].z) * zoom;
            float x2 = (points_[j].x / points_[j].z) * zoom;
            float y2 = (points_[j].y / points_[j].z) * zoom;
            float depth = (points_[i].z + points_[j].z) / 2;
            //strokeWeight (10 / depth);
            olc::Pixel color;
            color.r = static_cast<uint8_t> (Common_GL_Tools::map (depth, 0.1f, 0.7f, 0.0f, 255.0f));
            color.g = color.r;
            color.b = color.r;
            engine_in->DrawLine (half_width_i + static_cast<int32_t> (x1), half_height_i + static_cast<int32_t> (y1),
                                 half_width_i + static_cast<int32_t> (x2), half_height_i + static_cast<int32_t> (y2),
                                 color, 0xFFFFFFFF);
          } // end IF
        } // end FOR
    }
  };

  system system_;
  float  zoom_;
};

#endif // PGE_141_H
