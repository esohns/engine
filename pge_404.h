#ifndef PGE_404_H
#define PGE_404_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_18.h"

class PGE_404
 : public olc::PixelGameEngine
{
 public:
  PGE_404 ();
  inline virtual ~PGE_404 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class firefly
  {
   public:
    firefly (float x, float y)
     : position (x, y)
     , velocity ()
     , acceleration ()
     , color ()
     , newColor ()
     , lit (0)
     , angle (0.0f)
    {
      static olc::Pixel palette_a[7] = { olc::Pixel (0x4a, 0xd3, 0xc8), olc::Pixel (0xc6, 0x93, 0x22), olc::Pixel (0xc0, 0xbd, 0x9b),
                                         olc::Pixel (0xa0, 0x8b, 0x8d), olc::Pixel (0x00, 0x5e, 0x84), olc::Pixel (0x42, 0xa5, 0xe4),
                                         olc::Pixel (0x02, 0xc2, 0x87) };
      int index = Common_Tools::getRandomNumber (0, 6);
      color = palette_a[index];

      acceleration.x = Common_Tools::getRandomNumber (0.0f, 1.0f);
      acceleration.y = Common_Tools::getRandomNumber (0.0f, 1.0f);
      acceleration = acceleration.norm ();
      acceleration *= 0.01f;
    }

    void update (olc::PixelGameEngine* engine)
    {
      acceleration = acceleration.norm ();
      acceleration *= 0.015f;

      velocity += acceleration;
      if (velocity.mag () > 1.0f)
        velocity = velocity.norm ();
      angle = std::atan2 (velocity.y, velocity.x);

      position += velocity;

      // edges
      if (position.x < -10.0f)
        position.x = static_cast<float> (engine->ScreenWidth () - 1) + 10.0f;
      else if (position.x > static_cast<float> (engine->ScreenWidth() - 1) + 10.0f)
        position.x = -10.0f;
      if (position.y < -10.0f)
        position.y = static_cast<float> (engine->ScreenHeight () - 1) + 10.0f;
      else if (position.y > static_cast<float> (engine->ScreenHeight () - 1) + 10.0f)
        position.y = -10.0f;
    }

    void relate (std::vector<firefly>& fireflies)
    {
      for (int j = 0; j < fireflies.size (); j++)
      {
        firefly& other = fireflies[j];
        if (this == &other)
          continue;

        float distance = position.dist (other.position);
        if (distance < 30.0f)
        {
          if (lit > 50)
          {
            olc::vf2d force = other.position - position;
            force = force.norm ();
            float forceMag = Common_GL_Tools::map (distance, 0.0f, 30.0f, 0.01f, 0.001f);
            force *= forceMag;
            acceleration += force;
          } // end IF

          if (lit == 0 && distance < 20.0f)
            lit = 100;
          if (lit == 100)
          {
            olc::Pixel c1 = other.color;
            newColor = olc::PixelLerp (c1, color, Common_Tools::getRandomNumber (0.1f, 0.9f));
          } // end IF
        } // end IF
      } // end FOR

      if (lit == 1)
        color = newColor;
    }

    static olc::Pixel colorMixer (noise::module::Perlin* noise,
                                  olc::vf2d& position)
    { // *NOTE*: same palette as above
      static olc::Pixel palette_a[7] = { olc::Pixel (0x4a, 0xd3, 0xc8), olc::Pixel (0xc6, 0x93, 0x22), olc::Pixel (0xc0, 0xbd, 0x9b),
                                         olc::Pixel (0xa0, 0x8b, 0x8d), olc::Pixel (0x00, 0x5e, 0x84), olc::Pixel (0x42, 0xa5, 0xe4),
                                         olc::Pixel (0x02, 0xc2, 0x87) };

      float c =
        Common_GL_Tools::map (static_cast<float> (noise->GetValue (position.x / 200.0f, position.y / 200.0f, 0.0f)),
                              -1.0f, 1.0f, 0.0f, 1.0f) * 7.0f;
      int c1 = static_cast<int> (std::floor (c));
      int c2 = static_cast<int> (std::floor (c + 1.0f)) % 7;
      olc::Pixel color1 = palette_a[c1];
      olc::Pixel color2 = palette_a[c2];
      float dec;
      float mix = std::modf (c, &dec);
      olc::Pixel color = olc::PixelLerp (color1, color2, mix);
      return color;
    }

    void mutate (noise::module::Perlin* noise)
    {
      if (Common_Tools::testRandomProbability (0.0002f))
        color = PGE_404::firefly::colorMixer (noise, position);
    }

    void show (olc::PixelGameEngine* engine)
    {
      if (lit > 0)
        lit--;

      uint8_t colorAlpha = Common_GL_Tools::map (std::abs (lit - 50), 0, 50, 255, 0);
      olc::Pixel c1 = color;
      c1.a = colorAlpha;
      engine->FillCircle (position, 4, c1);
    }

    olc::vf2d            position;
    olc::vf2d            velocity;
    olc::vf2d            acceleration;
    olc::Pixel           color;
    olc::Pixel           newColor;
    int                  lit;
    float                angle;
  };

  std::vector<firefly>  fireflies_;
  noise::module::Perlin noise_;
};

#endif // PGE_404_H
  