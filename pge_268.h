#ifndef PGE_268_H
#define PGE_268_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

class PGE_268
 : public olc::PixelGameEngine
{
 public:
  PGE_268 ();
  inline virtual ~PGE_268 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (olc::vf2d& position,
              olc::vf2d& velocity,
              olc::Pixel color)
     : position_ (position)
     , velocity_ (velocity)
     , color_ (color)
     , life_ (400)
     , nextV_ (NULL)
    {}

    float easeOutCirc (float x)
    {
      return std::sqrt (1.0f - static_cast<float> (std::pow (x - 1.0f, 2)));
    }

    void update ()
    {
      --life_;

      float t =
        easeOutCirc (Common_GL_Tools::map (static_cast<float> (life_), 0.0f, 400.0f, 0.0f, 1.0f));
      float colorAlpha = Common_GL_Tools::map (t, 0.0f, 1.0f, 0.0f, 0.1f);
      color_.a = static_cast<uint8_t> (colorAlpha * 255.0f);

      position_ += velocity_;

      if (nextV_)
      {
        velocity_ = *nextV_;
        delete nextV_; nextV_ = NULL;
      } // end IF
    }

    void draw (olc::PixelGameEngine* engine)
    {
      float t =
        easeOutCirc (Common_GL_Tools::map (static_cast<float> (life_), 0.0f, 300.0f, 0.0f, 1.0f));
      float d = Common_GL_Tools::map (t, 0.0f, 1.0f, 0.0f, 3.0f);
      engine->FillCircle (position_, static_cast<int32_t> (d), color_);
      engine->DrawLine (position_, position_ + velocity_, color_, 0xFFFFFFFF);
    }

    olc::vf2d  position_;
    olc::vf2d  velocity_;
    olc::Pixel color_;
    int        life_;
    olc::vf2d* nextV_;
  };

  class edge
  {
   public:
    edge (olc::vf2d& start,
          olc::vf2d& end,
          float innerN,
          float outerN,
          bool mirror)
     : start_ (start)
     , end_ (end)
     , vec_ (end_ - start_)
     , nLeft_ (outerN)
     , nRight_ (innerN)
     , mirror_ (mirror)
    {}

    void draw (olc::PixelGameEngine* engine)
    {
      engine->DrawLine (start_, end_, olc::WHITE, 0xFFFFFFFF);
    }

    bool linesIntersect (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
    {
      float Ax = x2 - x1;
      float Ay = y2 - y1;
      float Bx = x4 - x3;
      float By = y4 - y3;
      float det = Ax * By - Ay * Bx;
      if (det == 0.0f)
        return false;

      float Cx = x3 - x1;
      float Cy = y3 - y1;
      float t = (Cx * By - Cy * Bx) / det;
      float u = (Cx * Ay - Cy * Ax) / det;
      return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
    }

    olc::vf2d getIntersection (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
    {
      float a = (y2 - y1) / (x2 - x1);
      float b = y1 - a * x1;
      float c = (y4 - y3) / (x4 - x3);
      float d = y3 - c * x3;

      if (a == c)
      { ACE_ASSERT (false);
        //print("No unique intersection point.");
      } // end IF
      else if (x2 - x1 == 0.0f || x4 - x3 == 0.0f)
      {
        float verticalX, a_2, b_2;
        if (x2 - x1 == 0)
        {
          verticalX = x1;
          a_2 = (y4 - y3) / (x4 - x3);
          b_2 = y3 - a_2 * x3;
        } // end IF
        else
        {
          verticalX = x3;
          a_2 = (y2 - y1) / (x2 - x1);
          b_2 = y1 - a_2 * x1;
        } // end ELSE
        float y = a_2 * verticalX + b_2;
        return olc::vf2d (verticalX, y);
      } // end ELSE IF
      else
      {
        float x = (d - b) / (a - c);
        float y = a * x + b;
        return olc::vf2d (x, y);
      } // end ELSE

      return olc::vf2d ();
    }

    std::string leftOrRight (float x, float y)
    {
      olc::vf2d temp (x - start_.x, y - start_.y);
      float angle =
        std::atan2 (vec_.y * temp.x - vec_.x * temp.y, vec_.x * temp.x + vec_.y * temp.y);
      if (angle > 0.0f)
        return ACE_TEXT_ALWAYS_CHAR ("right");
      else
        return ACE_TEXT_ALWAYS_CHAR ("left");
    }

    void refraction (particle& p)
    {
      if (linesIntersect (start_.x, start_.y, end_.x, end_.y, p.position_.x, p.position_.y, p.position_.x + p.velocity_.x, p.position_.y + p.velocity_.y))
      {
        olc::vf2d intersection =
          getIntersection (start_.x, start_.y, end_.x, end_.y, p.position_.x, p.position_.y, p.position_.x + p.velocity_.x, p.position_.y + p.velocity_.y);
        olc::vf2d vecIn = intersection - p.position_;
        
        olc::vf2d temp;
        temp.x = std::cos (static_cast<float> (M_PI_2)) * vec_.x - std::sin (static_cast<float> (M_PI_2)) * vec_.y;
        temp.y = std::sin (static_cast<float> (M_PI_2)) * vec_.x + std::cos (static_cast<float> (M_PI_2)) * vec_.y;
        temp = temp.norm ();
        temp *= (p.velocity_.mag () - vecIn.mag ());
        olc::vf2d vecOut = temp;
        float theta_f =
          std::atan2 (vecOut.y * p.velocity_.x - vecOut.x * p.velocity_.y, vecOut.x * p.velocity_.x + vecOut.y * p.velocity_.y);
        if (std::abs (theta_f) > static_cast<float> (M_PI_2))
        {
          temp.x = std::cos (static_cast<float> (M_PI)) * vecOut.x - std::sin (static_cast<float> (M_PI)) * vecOut.y;
          temp.y = std::sin (static_cast<float> (M_PI)) * vecOut.x + std::cos (static_cast<float> (M_PI)) * vecOut.y;
          vecOut = temp;
        } // end IF

        float thetaIn =
          std::atan2 (vecOut.y * p.velocity_.x - vecOut.x * p.velocity_.y, vecOut.x * p.velocity_.x + vecOut.y * p.velocity_.y);
        float nIn, nOut;
        if (leftOrRight (p.position_.x, p.position_.y) == std::string (ACE_TEXT_ALWAYS_CHAR ("left")))
        {
          nIn = nLeft_;
          nOut = nRight_;
        } // end IF
        else
        {
          nIn = nRight_;
          nOut = nLeft_;
        } // end ELSE

        float thetaOut = std::asin (nIn * std::sin (thetaIn) / nOut);
        if (std::isnan (thetaOut) || mirror_)
        {
          temp.x = std::cos (static_cast<float> (M_PI)) * vecOut.x - std::sin (static_cast<float> (M_PI)) * vecOut.y;
          temp.y = std::sin (static_cast<float> (M_PI)) * vecOut.x + std::cos (static_cast<float> (M_PI)) * vecOut.y;
          vecOut = temp;

          temp.x = std::cos (-thetaIn) * vecOut.x - std::sin (-thetaIn) * vecOut.y;
          temp.y = std::sin (-thetaIn) * vecOut.x + std::cos (-thetaIn) * vecOut.y;
          vecOut = temp;
        } // end IF
        else
        {
          temp.x = std::cos (thetaOut) * vecOut.x - std::sin (thetaOut) * vecOut.y;
          temp.y = std::sin (thetaOut) * vecOut.x + std::cos (thetaOut) * vecOut.y;
          vecOut = temp;
        } // end ELSE
        temp = vecOut;
        temp = temp.norm ();
        temp *= p.velocity_.mag ();
        p.nextV_ = new olc::vf2d (temp);
        p.velocity_ = vecIn + vecOut;
      } // end IF
    }

    olc::vf2d start_;
    olc::vf2d end_;
    olc::vf2d vec_;
    float     nLeft_;
    float     nRight_;
    bool      mirror_;
  };

  std::vector<particle> particles_;
  std::vector<edge>     edges_;
  bool                  showGrid_;
  bool                  walls_;

  void generateParticle ();
};

#endif // PGE_268_H
