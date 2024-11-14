#ifndef PGE_414_H
#define PGE_414_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_19.h"

class PGE_414
 : public olc::PixelGameEngine
{
 public:
  PGE_414 ();
  inline virtual ~PGE_414 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y)
     : position (x, y)
     , velocity (0.0f, 0.0f)
     , acceleration (0.0f, 0.0f)
     , target (0.0f, 0.0f)
     , isDead (false)
     , maxSpeed (Common_Tools::getRandomNumber (0.25f, 2.0f))
     , maxForce (Common_Tools::getRandomNumber (8.0f, 15.0f))
     , color (olc::BLACK)
     , targetColor (olc::BLACK)
     , colorBlendRate (Common_Tools::getRandomNumber (0.01f, 0.05f))
     , size (0.0f)
    {}

    void update (olc::vf2d& mouse,
                 bool isLMBPressed)
    {
      float distToTarget = position.dist (target);
      float proximityMult;
      if (distToTarget < static_cast<float> (ENGINE_PGE_414_DEFAULT_OK_DISTANCE))
      {
        proximityMult = distToTarget / static_cast<float> (ENGINE_PGE_414_DEFAULT_OK_DISTANCE);
        velocity *= ENGINE_PGE_414_DEFAULT_FRICTION_CLOSE;
      } // end IF
      else
      {
        proximityMult = 1.0f;
        velocity *= ENGINE_PGE_414_DEFAULT_FRICTION;
      } // end ELSE

      if (distToTarget > 1.0f)
      {
        olc::vf2d steer = target - position;
        steer = steer.norm ();
        steer *= (maxSpeed * proximityMult * ENGINE_PGE_414_DEFAULT_SPEED);
        acceleration += steer;
      } // end IF

      float mouseDist = mouse.dist (position);
      if (mouseDist < ENGINE_PGE_414_DEFAULT_MOUSE_SIZE)
      {
        olc::vf2d push = (isLMBPressed ? mouse - position : position - mouse);
        push = push.norm ();
        push *= (ENGINE_PGE_414_DEFAULT_MOUSE_SIZE - mouseDist) * ENGINE_PGE_414_DEFAULT_MOUSE_ATTRACT;
        acceleration += push;
      } // end IF

      velocity += acceleration;
      if (velocity.mag () > maxForce * ENGINE_PGE_414_DEFAULT_SPEED)
      {
        velocity = velocity.norm ();
        velocity *= maxForce * ENGINE_PGE_414_DEFAULT_SPEED;
      } // end IF
      position += velocity;
      acceleration *= 0.0f;
    }

    void draw (olc::PixelGameEngine* engine)
    {
      float targetSize = 2.0f;
      if (!isDead)
      {
        float distToTarget = position.dist (target);
        targetSize = Common_GL_Tools::map (std::min (distToTarget, static_cast<float> (ENGINE_PGE_414_DEFAULT_OK_DISTANCE)),
                                           0.0f, static_cast<float> (ENGINE_PGE_414_DEFAULT_OK_DISTANCE),
                                           2.0f, ENGINE_PGE_414_DEFAULT_PARTICLE_MAX_SIZE);
      } // end IF
      size = Common_Math_Tools::lerp (size, targetSize, 0.1f);

      color = olc::PixelLerp (color, targetColor, colorBlendRate);

      engine->FillCircle (position, static_cast<int32_t> (size), color);
    }

    void kill (olc::PixelGameEngine* engine)
    {
      if (unlikely (isDead))
        return;

      static int screen_width_i = engine->ScreenWidth ();
      static int screen_height_i = engine->ScreenHeight ();
      static olc::vf2d center_s (screen_width_i / 2.0f, screen_height_i / 2.0f);
      olc::vf2d randomDirection (Common_Tools::getRandomNumber (0.0f, static_cast<float> (screen_width_i - 1)),
                                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (screen_height_i - 1)));
      olc::vf2d velocity_s = randomDirection - center_s;
      velocity_s = velocity_s.norm ();
      velocity_s *= std::max (static_cast<float> (screen_width_i - 1), static_cast<float> (screen_height_i - 1));

      target = center_s + velocity_s;
      targetColor = olc::BLACK;
      isDead = true;
    }

    bool isOutOfBounds (olc::PixelGameEngine* engine)
    {
      static int screen_width_i = engine->ScreenWidth ();
      static int screen_height_i = engine->ScreenHeight ();

      return (position.x < 0.0f || position.x > static_cast<float> (screen_width_i - 1) ||
              position.y < 0.0f || position.y > static_cast<float> (screen_height_i - 1));
    }

    olc::vf2d  position;
    olc::vf2d  velocity;
    olc::vf2d  acceleration;
    olc::vf2d  target;
    bool       isDead;
    float      maxSpeed;
    float      maxForce;
    olc::Pixel color;
    olc::Pixel targetColor;
    float      colorBlendRate;
    float      size;
  };

  std::vector<particle> particles_;
};

#endif // PGE_414_H
