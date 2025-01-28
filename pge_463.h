#ifndef PGE_463_H
#define PGE_463_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

class PGE_463
 : public olc::PixelGameEngine
{
 public:
  PGE_463 ();
  inline virtual ~PGE_463 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void divideRect (float, float, float, float, int32_t);
  void initialize ();

  class rectangle
  {
   public:
    rectangle (float x, float y, int32_t w, int32_t h, olc::PixelGameEngine* engine)
     : position (x, y)
     , size (w, h)
     , t (0)
     , t1 (30)
     , t2 (10 + t1)
     , t3 (30 + t2)
     , distance (0.0f)
     , theta (0.0f)
     , position0 (position)
     , position1 (0.0f, 0.0f)
    {
      static olc::vf2d center_s (engine->ScreenWidth () / 2.0f, engine->ScreenHeight () / 2.0f);
      distance = position.dist (center_s);
      float r = distance * 0.25f;
      theta = std::atan2 (y - center_s.y, x - center_s.x);
      position1 = { x + r * std::cos (theta), y + r * std::sin (theta) };
    }

    void show (olc::PixelGameEngine* engine)
    {
      int32_t x = static_cast<int32_t> (position.x - size.x / 2);
      int32_t y = static_cast<int32_t> (position.y - size.y / 2);
      engine->FillRect (x, y, size.x + 1, size.y + 1, olc::WHITE);
    }

    inline float easeInOutQuart (float n)
    {
      return n < 0.5f ? 8.0f * n * n * n * n : 1.0f - static_cast<float> (std::pow (-2.0f * n + 2.0f, 4)) / 2.0f;
    }

    void update ()
    {
      t++;
      if (0 < t && t < t1)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t), 0.0f, static_cast<float> (t1 - 1), 0.0f, 1.0f);
        position.x = Common_Math_Tools::lerp (position0.x, position1.x, easeInOutQuart (n));
        position.y = Common_Math_Tools::lerp (position0.y, position1.y, easeInOutQuart (n));
      } // end IF
      else if (t2 < t && t < t3)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t), static_cast<float> (t2), static_cast<float> (t3 - 1), 0.0f, 1.0f);
        position.x = Common_Math_Tools::lerp (position1.x, position0.x, easeInOutQuart (n));
        position.y = Common_Math_Tools::lerp (position1.y, position0.y, easeInOutQuart (n));
      } // end ELSE IF
    }

    olc::vf2d position;
    olc::vi2d size;
    int32_t   t, t1, t2, t3;
    float     distance, theta;
    olc::vf2d position0, position1;
  };

  std::vector<rectangle> rectangles_;
};

#endif // PGE_463_H
