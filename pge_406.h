#ifndef PGE_406_H
#define PGE_406_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_18.h"

class PGE_406
 : public olc::PixelGameEngine
{
 public:
  PGE_406 ();
  inline virtual ~PGE_406 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class atom
  {
   public:
    enum Type
    {
      TYPE_POSITIVE = 0,
      TYPE_NEGATIVE,
      //
      TYPE_INVALID = -1 // --> mouse
    };

    atom (enum atom::Type type_in,
          olc::PixelGameEngine* engine)
     : type (type_in)
     , radius (0)
     , color ()
     , position (Common_Tools::getRandomNumber (-1.0f, 1.0f),
                 Common_Tools::getRandomNumber (-1.0f, 1.0f))
     , velocity (0.0f, 0.0f)
     , acceleration (0.0f, 0.0f)
    {
      switch (type)
      {
        case TYPE_POSITIVE:
        {
          radius = 16;
          color = olc::Pixel (255, 165, 0, 128); // orange
          break;
        }
        case TYPE_NEGATIVE:
        {
          radius = 24;
          color = olc::GREEN; color.a = 128;
          break;
        }
        case TYPE_INVALID: // --> mouse
        default:
          break;
      } // end SWITCH
    }

    atom (olc::vf2d& position_in) // --> mouse
     : type (TYPE_INVALID)
     , radius (ENGINE_PGE_406_DEFAULT_MOUSE_RADIUS)
     , color (olc::WHITE)
     , position (position_in)
     , velocity (0.0f, 0.0f)
     , acceleration (0.0f, 0.0f)
    {}

    void show (olc::PixelGameEngine* engine)
    {
      static olc::vf2d center_s (engine->ScreenWidth () / 2.0f,
                                 engine->ScreenHeight () / 2.0f);
      engine->FillCircle (center_s + position, radius, color);
    }

    void update ()
    {
      velocity += acceleration;

      if (velocity.mag () > ENGINE_PGE_406_DEFAULT_MAX_VELOCITY)
      {
        velocity = velocity.norm ();
        velocity *= ENGINE_PGE_406_DEFAULT_MAX_VELOCITY;
      } // end IF

      position += velocity;

      velocity *= ENGINE_PGE_406_DEFAULT_FRICTION;
      //acceleration *= 0.0f;
      acceleration = olc::vf2d (0.0f, ENGINE_PGE_406_DEFAULT_GRAVITY);
    }

    void wrap (olc::PixelGameEngine* engine)
    {
      static float half_screen_width_f = engine->ScreenWidth () / 2.0f;
      static float half_screen_height_f = engine->ScreenHeight () / 2.0f;
      position.x = std::clamp (position.x, static_cast<float> (radius) - half_screen_width_f, half_screen_width_f - static_cast<float> (radius));
      position.y = std::clamp (position.y, static_cast<float> (radius) - half_screen_height_f, half_screen_height_f - static_cast<float> (radius));
    }

    enum atom::Type type;
    int32_t         radius;
    olc::Pixel      color;
    olc::vf2d       position;
    olc::vf2d       velocity;
    olc::vf2d       acceleration;
  };

  void avoidOverlapping ();
  void attractBetweenChanges ();  // separation phase
  void attractBetweenChanges2 (); // attraction phase

  std::vector<atom> atoms_;
};

#endif // PGE_406_H
  