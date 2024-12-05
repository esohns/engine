#ifndef PGE_432_H
#define PGE_432_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_20.h"

class PGE_432
 : public olc::PixelGameEngine
{
 public:
  PGE_432 ();
  inline virtual ~PGE_432 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class mover
  {
   public:
    mover (olc::PixelGameEngine* engine)
      : position (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine->ScreenHeight () - 1)))
      , radius (Common_Tools::getRandomNumber (0.001f, 0.05f) * engine->ScreenWidth () / 2.0f)
      , t (-Common_Tools::getRandomNumber (0, 200))
    {
      initialization (engine);
    }

    void initialization (olc::PixelGameEngine* engine)
    {
      t = 0;
      t1 = 80;
      t2 = t1;
      previousPosition = position;
      previousRadius = radius;

      float factor_f;
      do
      {
        factor_f = Common_Tools::getRandomNumber (0.0f, 0.3f);
        factor_f = Common_Tools::testRandomProbability (0.5f) ? -factor_f : factor_f;
        targetPosition.x = position.x + factor_f * engine->ScreenWidth ();
        factor_f = Common_Tools::getRandomNumber (0.0f, 0.3f);
        factor_f = Common_Tools::testRandomProbability (0.5f) ? -factor_f : factor_f;
        targetPosition.y = position.y + factor_f * engine->ScreenHeight ();
        targetRadius = Common_Tools::getRandomNumber (0.001f, 0.05f) * engine->ScreenWidth () / 2.0f;
      } while (targetPosition.x < 0.0f || targetPosition.x > static_cast<float> (engine->ScreenWidth () - 1) ||
               targetPosition.y < 0.0f || targetPosition.y > static_cast<float> (engine->ScreenHeight () - 1));
    }

    void update (olc::PixelGameEngine* engine)
    {
      if (0 < t && t < t1)
      {
        float n =
          mover::easeInOutQuart (Common_GL_Tools::map (static_cast<float> (t), 0.0f, static_cast<float> (t1 - 1), 0.0f, 1.0f));
        position = previousPosition.lerp (targetPosition, n);
        radius = Common_Math_Tools::lerp (previousRadius, targetRadius, n);
      } // end IF

      if (t2 < t)
        initialization (engine);

      t++;
    }

    void draw (olc::PixelGameEngine* engine)
    {
      engine->DrawCircle (position, static_cast<int32_t> (radius), olc::WHITE, 0xFF);
    }

    static inline float easeInOutQuart (float x)
    {
      return x < 0.5f ? 8.0f * x * x * x * x 
                      : 1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 4)) / 2.0f;
    }

    olc::vf2d position;
    float     radius;
    int       t, t1, t2;
    olc::vf2d previousPosition;
    float     previousRadius;
    olc::vf2d targetPosition;
    float     targetRadius;
  };

  std::vector<mover> movers_;
};

#endif // PGE_432_H
