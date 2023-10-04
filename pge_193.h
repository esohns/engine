#ifndef PGE_193_H
#define PGE_193_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_193
 : public olc::PixelGameEngine
{
 public:
  PGE_193 ();
  inline virtual ~PGE_193 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct tentacle_part
  {
    olc::vf2d  position;
    float      width;
    float      height;
    olc::Pixel color;
  };

  class tentacle
  {
   public:
    olc::vf2d                  position_;
    float                      orientation_;
    //float                      angularVelocity_;
    int                        nbParts_;
    float                      compactness_;
    std::vector<tentacle_part> parts_;

    tentacle (olc::vf2d& pos, int nb, float w, float h, float o, float c)
     : position_ (pos)
     , orientation_ (o)
     //, angularVelocity_ (0.0f)
     , nbParts_ (nb)
     , compactness_ (c)
     , parts_ ()
    {
      float coul = 255.0f / static_cast<float> (nbParts_);
      for (int i = 0; i < nbParts_; i++)
      {
        struct tentacle_part part_s;
        part_s.width = (nbParts_ - i) * w / static_cast<float> (nbParts_);
        part_s.height = (nbParts_ - i) * h / static_cast<float> (nbParts_);
        part_s.position = position_;
        part_s.position.x += compactness_ * i * std::cos (orientation_);
        part_s.position.y += compactness_ * i * std::sin (orientation_);
        part_s.color = {255, static_cast<uint8_t> (255.0f - coul * i), 255};
        parts_.push_back (part_s);
      } // end FOR
    }

    void update ()
    {
      parts_[0].position = position_;
      parts_[1].position.x = parts_[0].position.x + (compactness_ * std::cos (orientation_));
      parts_[1].position.y = parts_[0].position.y + (compactness_ * std::sin (orientation_));
      for (int i = 2; i < nbParts_; i++)
      {
        olc::vf2d currentPos = parts_[i].position;
        olc::vf2d dist = currentPos - parts_[i - 2].position;
        float distmag = dist.mag ();
        olc::vf2d pos = parts_[i - 1].position;
        olc::vf2d move = dist * compactness_;
        move /= distmag;
        pos += move;
        parts_[i].position = pos;
      } // end FOR
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      for (int i = nbParts_ - 1; i >= 0; i--)
        engine_in->FillCircle (parts_[i].position, static_cast<int32_t> (parts_[i].width / 2.0f), parts_[i].color);
    }
  };

  class creature
  {
   public:
     olc::vf2d             position_;
     float                 radX_;
     float                 radY_;
     float                 orientation_;
     olc::Pixel            headClr_;
     std::vector<tentacle> tentacles_;
     int                   nbTentacles_;
     int                   tentaclesLength_;
     int                   moveTime_;
     int                   moveDuration_;
     olc::vf2d             dest_;
     olc::vf2d             lastPos_;
     float                 moveDistance_;
     bool                  reachedDest_;

     creature (olc::vf2d& pos, float rx, float ry, int nb, int l, float ts, float td)
      : position_ (pos)
      , radX_ (rx)
      , radY_ (ry)
      , orientation_ (0.0f)
      //, headClr_ (olc::WHITE)
      , tentacles_ ()
      , nbTentacles_ (nb)
      , tentaclesLength_ (l)
      , moveTime_ (0)
      , moveDuration_ (Common_Tools::getRandomNumber (200, 350))
      , dest_ (Common_Tools::getRandomNumber (-1.0f, 1.0f),
               Common_Tools::getRandomNumber (-1.0f, 1.0f))
      , lastPos_ (pos)
      , moveDistance_ (Common_Tools::getRandomNumber (30.0f, 120.0f))
      , reachedDest_ (true)
     {
       dest_ *= moveDistance_;

       for (int i = 0; i < nbTentacles_; i++)
       {
         float tx =
           position_.x + (std::cos (i * 2.0f * static_cast<float> (M_PI) / nbTentacles_) * radX_ / 2.0f);
         float ty =
           position_.y + (std::sin (i * 2.0f * static_cast<float> (M_PI) / nbTentacles_) * radY_ / 2.0f);
         olc::vf2d pos (tx, ty);
         float tr = std::atan2 (ty - position_.y, tx - position_.x);
         tentacles_.push_back (tentacle (pos, l, ts, ts, tr, td));
       } // end FOR
     }

     void update (olc::PixelGameEngine* engine_in)
     {
       for (int i = 0; i < nbTentacles_; i++)
       {
         tentacles_[i].position_.x =
           position_.x + (std::cos (i * 2.0f * static_cast<float> (M_PI) / nbTentacles_) * radX_ / 2.0f);
         tentacles_[i].position_.y =
           position_.y + (std::sin (i * 2.0f * static_cast<float> (M_PI) / nbTentacles_) * radY_ / 2.0f);
         tentacles_[i].orientation_ =
           std::atan2 ((tentacles_[i].position_.y - position_.y), (tentacles_[i].position_.x - position_.x));
         tentacles_[i].update ();
       } // end FOR

       if (reachedDest_)
       {
         lastPos_ = position_;
         dest_.x = Common_Tools::getRandomNumber (-1.0f, 1.0f);
         dest_.y = Common_Tools::getRandomNumber (-1.0f, 1.0f);
         dest_ = dest_.norm ();
         dest_ *= moveDistance_;

         olc::vf2d nextPos = position_ + dest_;
         if (nextPos.x > engine_in->ScreenWidth () - 1)
           dest_.x = -std::abs (dest_.x);
         else if (nextPos.x < 0)
           dest_.x = std::abs (dest_.x);
         if (nextPos.y > engine_in->ScreenHeight () - 1)
           dest_.y = -std::abs (dest_.y);
         else if (nextPos.y < 0)
           dest_.y = std::abs (dest_.y);
         reachedDest_ = false;
         moveTime_ = 0;
       } // end IF
       else
       {
         position_.x =
           easeInOutExpo (static_cast<float> (moveTime_), lastPos_.x, dest_.x, static_cast<float> (moveDuration_));
         position_.y =
           easeInOutExpo (static_cast<float> (moveTime_), lastPos_.y, dest_.y, static_cast<float> (moveDuration_));
         moveTime_++;
         if (moveTime_ >= moveDuration_)
           reachedDest_ = true;
       } // end ELSE
       orientation_ +=
         Common_Tools::getRandomNumber (-2.0f, 2.0f) * (0.8f * static_cast<float> (M_PI) / 180.0f);

       for (int i = 0; i < nbTentacles_; i++)
         tentacles_[i].draw (engine_in);
       engine_in->FillCircle (position_, 30 / 2, olc::WHITE);
     }
  };

  std::vector<creature> creatures_;

  static float easeInOutExpo (float, float, float, float);
};

#endif // PGE_193_H
