#ifndef PGE_116_H
#define PGE_116_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"

class PGE_116
 : public olc::PixelGameEngine
{
 public:
  PGE_116 ();
  virtual ~PGE_116 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class color
  {
   public:
    float R_, G_, B_, Rspeed_, Gspeed_, Bspeed_;
    float minSpeed = 0.6f;
    float maxSpeed = 1.8f;
    float minR = 20.0f;
    float maxR = 85.0f;
    float minG = 198.0f;
    float maxG = 255.0f;
    float minB = 200.0f;
    float maxB = 212.0f;

    color ()
    {
      initialize ();
    }

    void initialize ()
    {
      R_ = Common_Tools::getRandomNumber (minR, maxR);
      G_ = Common_Tools::getRandomNumber (minG, maxG);
      B_ = Common_Tools::getRandomNumber (minB, maxB);
      Rspeed_ = (Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (minSpeed, maxSpeed);
      Gspeed_ = (Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (minSpeed, maxSpeed);
      Bspeed_ = (Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f) * Common_Tools::getRandomNumber (minSpeed, maxSpeed);
    }

    void update ()
    {
      Rspeed_ = ((R_ += Rspeed_) > maxR || (R_ < minR)) ? -Rspeed_ : Rspeed_;
      Gspeed_ = ((G_ += Gspeed_) > maxG || (G_ < minG)) ? -Gspeed_ : Gspeed_;
      Bspeed_ = ((B_ += Bspeed_) > maxB || (B_ < minB)) ? -Bspeed_ : Bspeed_;
    }

    olc::Pixel getColor ()
    {
      olc::Pixel result;
      result.r = static_cast<uint8_t> (R_);
      result.g = static_cast<uint8_t> (G_);
      result.b = static_cast<uint8_t> (B_);
      return result;
    }
  };

  class weed
  {
   public:
    weed (float p_rad, float p_length,
          olc::PixelGameEngine* engine_in,
          noise::module::Perlin* noise_in)
     : colors_ ()
     , positions_ ()
     , thetas_ ()
     , baseTheta_ (p_rad)
     , cosinus_ (std::cos (p_rad))
     , sinus_ (std::sin (p_rad))
     , position_ (engine_in->ScreenWidth () / 2.0f, engine_in->ScreenHeight () / 2.0f)
     , numberSegments_ (static_cast<int32_t> (p_length / ENGINE_PGE_116_DEFAULT_MAX_DISTANCE))
     , color_ ()
    {
      colors_.push_back (color_.getColor ());
      positions_.push_back (position_);
      thetas_.push_back (0.0f);
      for (int i = 1; i < numberSegments_; i++)
      {
        colors_.push_back (color_.getColor ());
        positions_.push_back ({positions_[i - 1].x - ENGINE_PGE_116_DEFAULT_MAX_DISTANCE * cosinus_,
                               positions_[i - 1].y - ENGINE_PGE_116_DEFAULT_MAX_DISTANCE * sinus_});
        double qqq = noise_in->GetValue (positions_[i].x / 22.0, positions_[i].y / 22.0, 0.0);
        qqq = Common_GL_Tools::map (qqq, -1.0, 1.0, -M_PI / 3.0, M_PI / 3.0) * Common_GL_Tools::map (static_cast<double> (i), 1.0, static_cast<double> (numberSegments_ - 1), 0.0, 1.0);
        thetas_.push_back (static_cast<float> (qqq));
        //thetas_.push_back (0.0f);
      } // end FOR
    }

    void update (olc::PixelGameEngine* engine_in,
                 olc::vf2d& center,
                 noise::module::Perlin* noise_in,
                 olc::vd2d& noiseRoot_in,
                 bool noiseOn,
                 float mouseDist,
                 int32_t& touchCount_inout)
    {
      positions_[0] = center;
      bool touched = false;
      for (int i = 1; i < numberSegments_; i++)
      {
        if (noiseOn)
        {
          float n = 
            Common_GL_Tools::map (static_cast<float> (noise_in->GetValue (noiseRoot_in.x + 0.002 * positions_[i].x,
                                                                          noiseRoot_in.y + 0.002 * positions_[i].y,
                                                                          0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
          float noiseForce = (0.5f - n) * 7.0f;
          positions_[i].x += noiseForce;
          positions_[i].y += noiseForce;
        } // end IF

        olc::vf2d pv (std::cos (baseTheta_ + thetas_[i]), std::sin (baseTheta_ + thetas_[i]));
        pv *= Common_GL_Tools::map (static_cast<float> (i), 1.0f, static_cast<float> (numberSegments_), ENGINE_PGE_116_DEFAULT_FLOATATION, 0.6f * ENGINE_PGE_116_DEFAULT_FLOATATION);
        positions_[i] += pv;

        if (!engine_in->GetMouse (0).bHeld)
        {
          olc::vf2d mouse (static_cast<float> (engine_in->GetMouseX ()),
                           static_cast<float> (engine_in->GetMouseY ()));
          float d = positions_[i].dist (mouse);
          if (d < mouseDist)
          {
            olc::vf2d tmpPV = mouse;
            tmpPV -= positions_[i];
            tmpPV = tmpPV.norm ();
            tmpPV *= mouseDist;
            tmpPV = mouse - tmpPV;
            positions_[i] = tmpPV;
          } // end IF
        } // end IF

        olc::vf2d tmp = positions_[i-1] - positions_[i];
        tmp = tmp.norm ();
        tmp *= ENGINE_PGE_116_DEFAULT_MAX_DISTANCE;
        olc::vf2d tmp_rotated;
        tmp_rotated.x = std::cos (thetas_[i]) * tmp.x - std::sin (thetas_[i]) * tmp.y;
        tmp_rotated.y = std::sin (thetas_[i]) * tmp.x + std::cos (thetas_[i]) * tmp.y;
        tmp = tmp_rotated;
        positions_[i] = positions_[i-1] - tmp;
      
        if (positions_[i].y > ENGINE_PGE_116_DEFAULT_BOTTOM)
        {
          touched = true;
          positions_[i].y = ENGINE_PGE_116_DEFAULT_BOTTOM;
        } // end IF
      } // end FOR

      if (touched)
        touchCount_inout++;
    
      updateColors ();
   
      for (int i = 0; i < numberSegments_; i++)
        engine_in->Draw (positions_[i], colors_[i]);
    }

    void updateColors ()
    {
      color_.update ();
      colors_[0] = color_.getColor ();
      for (int i = numberSegments_ - 1; i > 0; i--)
        colors_[i] = colors_[i - 1];
    }

    std::vector<olc::Pixel> colors_;
    std::vector<olc::vf2d>  positions_;
    std::vector<float>      thetas_;
    float                   baseTheta_;
    float                   cosinus_;
    float                   sinus_;
    olc::vf2d               position_;
    int32_t                 numberSegments_;
    color                   color_;
  };

  olc::vf2d             center_;
  std::vector<weed*>    weeds_;
  noise::module::Perlin noise_;
  olc::vd2d             noiseRoot_;
  bool                  noiseOn_;
  int32_t               touchCount_;
  float                 mouseDist_;
  olc::vf2d             targetSpeed_;
};

#endif // PGE_116_H
