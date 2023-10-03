#ifndef PGE_190_H
#define PGE_190_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_8.h"

class PGE_190
 : public olc::PixelGameEngine
{
 public:
  PGE_190 ();
  inline virtual ~PGE_190 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class color
  {
   public:
    color ()
     : R_ (Common_Tools::getRandomNumber (0.0f, 255.0f))
     , G_ (Common_Tools::getRandomNumber (0.0f, 255.0f))
     , B_ (Common_Tools::getRandomNumber (0.0f, 255.0f))
     , Rspeed_ ((Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (ENGINE_PGE_190_DEFAULT_MIN_SPEED, ENGINE_PGE_190_DEFAULT_MAX_SPEED))
     , Gspeed_ ((Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (ENGINE_PGE_190_DEFAULT_MIN_SPEED, ENGINE_PGE_190_DEFAULT_MAX_SPEED))
     , Bspeed_ ((Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (ENGINE_PGE_190_DEFAULT_MIN_SPEED, ENGINE_PGE_190_DEFAULT_MAX_SPEED))
    {}

    void update ()
    {
      R_ += Rspeed_;
      Rspeed_ = (R_ > 255.0f || R_ < 0.0f) ? -Rspeed_ : Rspeed_;
      G_ += Gspeed_;
      Gspeed_ = (G_ > 255.0f || G_ < 0.0f) ? -Gspeed_ : Gspeed_;
      B_ += Bspeed_;
      Bspeed_ = (B_ > 255.0f || B_ < 0.0f) ? -Bspeed_ : Bspeed_;
    }

    olc::Pixel get ()
    {
      olc::Pixel result;
      result.r = static_cast<uint8_t> (R_);
      result.g = static_cast<uint8_t> (G_);
      result.b = static_cast<uint8_t> (B_);

      return result;
    }

    float R_, G_, B_, Rspeed_, Gspeed_, Bspeed_;
  };

  class circle
  {
   public:
    circle () {}
    circle (float radius, float x, float y)
     : position_ (x, y)
     , radius_ (radius)
     , theta_ (0.0f)
     , nbLines_ (Common_Tools::getRandomNumber (3, 25))
     , rotSpeed_ ((Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (0.005f, 0.034f))
     , radSpeed_ ((Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (0.3f  , 1.4f))
    {}

    void update (float rMax_in)
    {
      theta_ += rotSpeed_;
      radius_ += radSpeed_;
      radSpeed_ *= std::abs (radius_) > rMax_in ? -1.0f : 1.0f;
    }

    olc::vf2d position_;
    float radius_;
    float theta_;
    int nbLines_;
    float rotSpeed_;
    float radSpeed_;
  };

  color               color_;
  float               dMin_;
  float               rMax_;
  std::vector<circle> circles_;

  void initialize (bool);
};

#endif // PGE_190_H
