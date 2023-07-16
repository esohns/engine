#ifndef PGE_57_H
#define PGE_57_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_57
 : public olc::PixelGameEngine
{
 public:
  PGE_57 ();
  virtual ~PGE_57 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (int32_t x_in, int32_t y_in, const olc::Pixel& color_in)
     : position_ (static_cast<float> (x_in), static_cast<float> (y_in))
     , velocity_ (0.0f, 0.0f)
     , mass_ (Common_Tools::getRandomNumber (0.003f, 0.03f))
     , color_ (color_in)
    {}

    void update ()
    {
      position_ += velocity_;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (mass_ * 1000.0f), color_);
    }

    olc::vf2d  position_;
    olc::vf2d  velocity_;
    float      mass_;

   private:
    olc::Pixel color_;
  };

  void handleInteractions ();

  std::vector<particle*> particles_;
  olc::Pixel             color_;
};

#endif // PGE_57_H
