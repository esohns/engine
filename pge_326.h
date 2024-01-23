#ifndef PGE_326_H
#define PGE_326_H

#include "olcPixelGameEngine.h"

class PGE_326
 : public olc::PixelGameEngine
{
 public:
  PGE_326 ();
  inline virtual ~PGE_326 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class turtle
  {
   public:
    turtle (float x, float y)
     : position_ (x, y)
     , angle_ (0.0f)
     , penDown_ (false)
    {}

    void moveTo (float newX, float newY, olc::PixelGameEngine* engine, const olc::Pixel& color)
    {
      olc::vf2d next_position_s (newX, newY);
      if (penDown_)
        engine->DrawLine (position_, next_position_s, color, 0xFFFFFFFF);

      position_ = next_position_s;
    }

    void moveForward (float distance, olc::PixelGameEngine* engine, const olc::Pixel& color)
    {
      float newX = position_.x + std::cos (angle_) * distance;
      float newY = position_.y + std::sin (angle_) * distance;
      moveTo (newX, newY, engine, color);
    }

    void moveBackward (float distance, olc::PixelGameEngine* engine, const olc::Pixel& color)
    {
      moveForward (-distance, engine, color);
    }

    void turnTo (float angle)
    {
      angle_ = angle;
    }

    void turnRight (float angle)
    {
      angle_ += angle;
    }

    void turnLeft (float angle)
    {
      angle_ -= angle;
    }

    void penUp ()
    {
      penDown_ = false;
    }

    void penDown ()
    {
      penDown_ = true;
    }

    olc::vf2d position_;
    float     angle_;
    bool      penDown_;
  };

  turtle turtle_;
  float  hue_;
};

#endif // PGE_326_H
