#ifndef PGE_110_H
#define PGE_110_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_4.h"

class PGE_110
 : public olc::PixelGameEngine
{
 public:
  PGE_110 ();
  virtual ~PGE_110 ();

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
    float minR = 200.0f;
    float maxR = 255.0f;
    float minG = 20.0f;
    float maxG = 120.0f;
    float minB = 100.0f;
    float maxB = 140.0f;

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
    weed (float p_rad, float p_length, olc::PixelGameEngine* engine_in)
     : colors_ ()
     , positions_ ()
     , rad_ ()
     , cosinus_ (std::cos (p_rad))
     , sinus_ (std::sin (p_rad))
     , position_ (engine_in->ScreenWidth () / 2.0f + ENGINE_PGE_110_DEFAULT_RADIUS * cosinus_,
                  engine_in->ScreenHeight () / 2.0f + ENGINE_PGE_110_DEFAULT_RADIUS * sinus_)
     , numberSegments_ (static_cast<int32_t> (p_length / ENGINE_PGE_110_DEFAULT_MAX_DISTANCE))
     , mouseDistance_ (ENGINE_PGE_110_DEFAULT_MOUSE_DISTANCE)
     , color_ ()
    {
      positions_.push_back (position_);
      colors_.push_back (color_.getColor ());
      rad_.push_back (3.0f);
      for (int i = 1; i < numberSegments_; i++)
      {
        positions_.push_back ({positions_[i - 1].x - ENGINE_PGE_110_DEFAULT_MAX_DISTANCE * cosinus_,
                               positions_[i - 1].y - ENGINE_PGE_110_DEFAULT_MAX_DISTANCE * sinus_});
        colors_.push_back (color_.getColor ());
        rad_.push_back (3.0f);
      } // end FOR
    }

    void update (olc::PixelGameEngine* engine_in,
                 olc::vf2d& center,
                 noise::module::Perlin* noise_in,
                 olc::vd2d& noiseRoot_in,
                 bool noiseOn,
                 int32_t mode)
    {
      olc::vf2d mouse = {static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ())};

      positions_[0] = position_;
      for (int i = 1; i < numberSegments_; i++)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (noise_in->GetValue (noiseRoot_in.x + .002f * positions_[i].x, noiseRoot_in.y + .002f * positions_[i].y, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
        float noise_force_f = (0.5f - n) * 7.0f;
        if (noiseOn)
        {
          positions_[i].x += noise_force_f;
          positions_[i].y += noise_force_f;
        } // end IF
        olc::vf2d pv (cosinus_, sinus_);
        pv *=
          Common_GL_Tools::map (static_cast<float> (i), 1.0f, static_cast<float> (numberSegments_), ENGINE_PGE_110_DEFAULT_FLOATATION, 0.6f * ENGINE_PGE_110_DEFAULT_FLOATATION);
        positions_[i] += pv;

        //mouse interaction
        //if(pmouseX != mouseX || pmouseY != mouseY)
        {
          float d = mouse.dist (positions_[i]);
          if (d < mouseDistance_)// && pmouseX != mouseX && abs(pmouseX - mouseX) < 12)
          {
            olc::vf2d tmpPV = mouse;
            tmpPV -= positions_[i];
            tmpPV = tmpPV.norm ();
            tmpPV *= mouseDistance_;
            tmpPV = mouse - tmpPV;
            positions_[i] = tmpPV;
          } // end IF
        }

        olc::vf2d tmp = positions_[i - 1] - positions_[i];
        tmp = tmp.norm ();
        tmp *= ENGINE_PGE_110_DEFAULT_MAX_DISTANCE;
        positions_[i] = positions_[i - 1] - tmp;

        //keep the points inside the circle
        if (center.dist (positions_[i]) > ENGINE_PGE_110_DEFAULT_RADIUS)
        {
          olc::vf2d tmpPV = positions_[i];
          tmpPV -= center;
          tmpPV = tmpPV.norm ();
          tmpPV *= ENGINE_PGE_110_DEFAULT_RADIUS;
          tmpPV += center;
          positions_[i] = tmpPV;
        } // end IF
      } // end FOR

      updateColors ();

      olc::Pixel color;
      if (mode == 0)
      {
        color = olc::BLACK;
        color.a = 100;
      }
      for (int i = 0; i < numberSegments_; i++)
      {
        float r = rad_[i];
        if (mode == 1)
        {
          color = colors_[i];
          int next_i = (i == (numberSegments_ - 1) ? i : i + 1);
          engine_in->DrawLine (static_cast<int32_t> (positions_[i].x), static_cast<int32_t> (positions_[i].y),
                               static_cast<int32_t> (positions_[next_i].x), static_cast<int32_t> (positions_[next_i].y),
                               color, 0xFFFFFFFF);
        } // end IF
        else
        {
          color = colors_[i];
          engine_in->FillCircle (positions_[i], 2, color);
        } // end ELSE
      } // end FOR
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
    std::vector<float>      rad_;
    float                   cosinus_;
    float                   sinus_;
    olc::vf2d               position_;
    int32_t                 numberSegments_;
    float                   mouseDistance_;
    color                   color_;
  };

  olc::vf2d             center_;
  std::vector<weed*>    weeds_;
  noise::module::Perlin noise_;
  olc::vd2d             noiseRoot_;
  bool                  noiseOn_;
  int32_t               mode_;
};

#endif // PGE_110_H
