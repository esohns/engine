#ifndef PGE_43_H
#define PGE_43_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_2.h"

class PGE_43
 : public olc::PixelGameEngine
{
 public:
  PGE_43 ();
  virtual ~PGE_43 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class wave
  {
   public:
    wave (float amplitude_in, float period_in, float phase_in)
     : amplitude_ (amplitude_in)
     , period_ (period_in)
     , phase_ (phase_in)
    {}

    float evaluate (int32_t x)
    {
      return std::sin (phase_ + static_cast<float> (2.0 * M_PI * static_cast<double> (x)) / period_) * amplitude_;
    }

    void update ()
    {
      phase_ += ENGINE_PGE_43_DEFAULT_PHASE_INCREMENT;
    }

   private:
    float amplitude_;
    float period_;
    float phase_;
  };

  std::vector<wave*> waves_;
};

#endif // PGE_43_H
