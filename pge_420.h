#ifndef PGE_420_H
#define PGE_420_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_420
 : public olc::PixelGameEngine
{
 public:
  PGE_420 ();
  inline virtual ~PGE_420 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class cell
  {
   public:
    cell (float x, float y, float radius_, float speed_, int state_)
     : position (x, y)
     , prevPosition (position)
     , state (state_)
     , radius (radius_)
     , speed (speed_)
     //, color (olc::BLACK)
     , direction (-1.0f)
     , frequency (0.02f)
    {
      //static olc::Pixel palette_a[5] = {olc::Pixel (0xf0, 0x10, 0x1c), olc::Pixel (0xf0, 0x56, 0x97), olc::Pixel (0x0b, 0x46, 0x9b),
      //                                  olc::Pixel (0x32, 0xb6, 0xc3), olc::Pixel (0xf7, 0x80, 0x00)};
      //color = palette_a[Common_Tools::getRandomNumber (0, 4)];
    }

    int wRand (std::vector<float>& spec)
    {
      float sum = 0.0f;
      float r = Common_Tools::getRandomNumber (0.0f, 1.0f);
      int i = 0;
      for (std::vector<float>::iterator iterator = spec.begin ();
           iterator != spec.end ();
           ++iterator, ++i)
      {
        sum += *iterator;
        if (r <= sum)
          return i;
      } // end FOR

      return 0;
    }

    int newState (int state_in,
                  std::vector<std::vector<float> >& tM)
    {
      return wRand (tM[state_in]);
    }

    void update (std::vector<std::vector<float> >& tM,
                 int frameCount,
                 olc::PixelGameEngine* engine)
    {
      //direction = direction * std::sin (0.1f * frameCount);
      float theta = direction * frameCount * frequency;

      static float half_width_f = engine->ScreenWidth () / 2.0f;
      static float half_height_f = engine->ScreenHeight () / 2.0f;
      float temp1 = half_width_f  + 100.0f * std::cos (frequency / 2.0f * frameCount);
      float temp2 = half_height_f + 100.0f * std::sin (frequency / 2.0f * frameCount);
      position.x = temp1 + speed * radius * std::cos (theta);
      position.y = temp2 + speed * radius * std::sin (theta);

      static olc::Pixel palette_a[5] = {olc::Pixel (0xf0, 0x10, 0x1c), olc::Pixel (0xf0, 0x56, 0x97), olc::Pixel (0x0b, 0x46, 0x9b),
                                        olc::Pixel (0x32, 0xb6, 0xc3), olc::Pixel (0xf7, 0x80, 0x00)};

      if (frameCount > 1)
        engine->DrawLine (prevPosition, position, palette_a[state], 0xFFFFFFFF);
      prevPosition = position;

      state = newState (state, tM);
    }

    olc::vf2d  position;
    olc::vf2d  prevPosition;
    int        state;
    float      radius;
    float      speed;
    //olc::Pixel color;
    float      direction;
    float      frequency;
  };

  std::vector<cell>                cells_;
  int                              state0_;
  std::vector<std::vector<float> > states_;
};

#endif // PGE_420_H
