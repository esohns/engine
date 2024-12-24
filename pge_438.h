#ifndef PGE_438_H
#define PGE_438_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_20.h"

class PGE_438
 : public olc::PixelGameEngine
{
 public:
  PGE_438 ();
  inline virtual ~PGE_438 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y)
     : position (x, y)
     , target (x, y)
     , velocity (Common_Tools::getRandomNumber (0.0f, 1.0f),
                 Common_Tools::getRandomNumber (0.0f, 1.0f))
     , acceleration (0.0f, 0.0f)
     , maxSpeed (ENGINE_PGE_438_DEFAULT_MAX_SPEED)
     , maxForce (ENGINE_PGE_438_DEFAULT_MAX_FORCE)
     , color (olc::Pixel (255, 255, 255, 0))
     , size (Common_Tools::getRandomNumber (1, ENGINE_PGE_438_DEFAULT_MAX_SIZE))
    {
      velocity = velocity.norm ();
    }

    void update ()
    {
      // Arrival behavior towards target
      olc::vf2d desired = target - position;
      float d = desired.mag ();
      float speed = maxSpeed;
      if (d < 100.0f)
        speed = Common_GL_Tools::map (d, 0.0f, 100.0f, 0.0f, maxSpeed);
      desired = desired.norm ();
      desired *= speed;
      olc::vf2d steer = desired - velocity;
      d = steer.mag ();
      if (d > maxForce)
      {
        steer = steer.norm ();
        steer *= maxForce;
      } // end IF
      acceleration += steer;

      velocity += acceleration;
      d = velocity.mag ();
      if (d > maxSpeed)
      {
        velocity = velocity.norm ();
        velocity *= maxSpeed;
      } // end IF

      position += velocity;
      acceleration *= 0.0f;

      // Fade color based on speed
      color.a =
        static_cast<uint8_t> (Common_GL_Tools::map (speed, 0.0f, maxSpeed, 255.0f, 0.0f));
    }

    inline void show (olc::PixelGameEngine* engine)
    {
      engine->FillCircle (position, size, color);
    }

    olc::vf2d  position;
    olc::vf2d  target;
    olc::vf2d  velocity;
    olc::vf2d  acceleration;
    float      maxSpeed;
    float      maxForce;
    olc::Pixel color;
    int32_t    size;
  };

  void setFormation ();

  std::vector<std::function<olc::vf2d (int)> > formations_;
  int                                          formationType_;
  std::vector<particle>                        particles_;
};

#endif // PGE_438_H
