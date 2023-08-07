#ifndef PGE_85_H
#define PGE_85_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_85
 : public olc::PixelGameEngine
{
 public:
  PGE_85 ();
  inline virtual ~PGE_85 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<float> linspace (float, float, int);

  class complex
  {
   public:
    complex (float x, float y)
     : x_ (x)
     , y_ (y)
    {}

    complex add (complex z)
    {
      return complex (x_ + z.x_, y_ + z.y_);
    }

    complex minus (complex z)
    {
      return complex (x_ - z.x_, y_ - z.y_);
    }

    complex mult (complex z)
    {
      return complex (x_ * z.x_ - y_ * z.y_, x_ * z.y_ + y_ * z.x_);
    }

    complex times (float z)
    {
      return complex (x_ * z, y_ * z);
    }

    complex divides (float z)
    {
      return complex (x_ / z, y_ / z);
    }

    complex exp ()
    {
      return complex (std::exp (x_) * std::cos (y_), std::exp (x_) * std::sin (y_));
    }

    complex scale (float s)
    {
      return complex (x_ * s, y_ * s);
    }

    complex sq ()
    {
      return mult (*this);
    }

    float modulus ()
    {
      return std::sqrt (x_ * x_ + y_ * y_);
    }

    float arg ()
    {
      return std::atan2 (y_, x_);
    }

    complex sqrt ()
    {
      float r_f = std::sqrt (modulus ());
      float arg_f = arg () / 2.0f;
      return complex (r_f * std::cos (arg_f), r_f * std::sin (arg_f));
    }

    float x_;
    float y_;
  };
  complex swirly (float, float, float, float, float);
};

#endif // PGE_85_H
