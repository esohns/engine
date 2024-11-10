#ifndef PGE_401_H
#define PGE_401_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_18.h"

class PGE_401
 : public olc::PixelGameEngine
{
 public:
  PGE_401 ();
  inline virtual ~PGE_401 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class walker
  {
   public:
    walker (olc::PixelGameEngine* engine)
     : position (engine->ScreenWidth () / 2.0f, engine->ScreenHeight () / 2.0f)
     , color (Common_Tools::getRandomNumber (150, 255),
              Common_Tools::getRandomNumber (100, 255),
              Common_Tools::getRandomNumber (50, 150),
              Common_Tools::getRandomNumber (50, 150))
     , size (Common_Tools::getRandomNumber (1, 4))
     , rotationAngle (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , rotationSpeed (Common_Tools::getRandomNumber (0.005f, 0.02f))
    {}

    void draw (olc::PixelGameEngine* engine)
    {
      olc::vf2d velocity (std::cos (rotationAngle), std::sin (rotationAngle));
      velocity *= Common_Tools::getRandomNumber (0.5f, 2.0f);
      position += velocity;

      color.a = Common_Tools::getRandomNumber (10, 128);

      engine->FillCircle (position, size, color);

      rotationAngle += rotationSpeed;
    }

    olc::vf2d  position;
    olc::Pixel color;
    int32_t    size;
    float      rotationAngle;
    float      rotationSpeed;
  };

  std::vector<walker> walkers_;
};

#endif // PGE_401_H
