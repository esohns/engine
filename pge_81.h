#ifndef PGE_81_H
#define PGE_81_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_3.h"

class PGE_81
 : public olc::PixelGameEngine
{
 public:
  PGE_81 ();
  virtual ~PGE_81 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class snake
  {
   public:
    snake (int32_t x, int32_t y, int32_t dir, int32_t dist)
     : position_ (x, y)
     , direction_ (dir)
     , dist_ (dist)
     , devant_ (true)
     , droite_ (true)
     , gauche_ (true)
     , vie_ (true)
     , branche_ (NULL)
    {}

    void tester (olc::PixelGameEngine* engine_in,
                 const olc::Pixel& background_in)
    {
      devant_ = true;
      gauche_ = true;
      droite_ = true;

      olc::Pixel* p = engine_in->GetDrawTarget ()->GetData ();
      switch (direction_)
      {
        case 0: // gauche
          if (position_.x - dist_ < dist_)
            devant_ = false;
          else if (p[(position_.x - dist_) + position_.y * engine_in->ScreenWidth ()] != background_in)
            devant_ = false;
          if (position_.y + dist_ > engine_in->ScreenHeight () - dist_)
            gauche_ = false;
          else if (p[position_.x + (position_.y + dist_) * engine_in->ScreenWidth ()] != background_in)
            gauche_ = false;
          if (position_.y - dist_ < dist_)
            droite_ = false;
          else if (p[position_.x + (position_.y - dist_) * engine_in->ScreenWidth ()] != background_in)
            droite_ = false;
          break;
        case 1: // haut
          if (position_.y - dist_ < dist_)
            devant_ = false;
          else if (p[position_.x + (position_.y - dist_) * engine_in->ScreenWidth ()] != background_in)
            devant_ = false;
          if (position_.x - dist_ < dist_)
            gauche_ = false;
          else if (p[(position_.x - dist_) + position_.y * engine_in->ScreenWidth ()] != background_in)
            gauche_ = false;
          if (position_.x + dist_ > engine_in->ScreenWidth () - dist_)
            droite_ = false;
          else if (p[(position_.x + dist_) + position_.y * engine_in->ScreenWidth ()] != background_in)
            droite_ = false;
          break;
        case 2: // droite
          if (position_.x + dist_ > engine_in->ScreenWidth () - dist_)
            devant_ = false;
          else if (p[(position_.x + dist_) + position_.y * engine_in->ScreenWidth ()] != background_in)
            devant_ = false;
          if (position_.y - dist_ < dist_)
            gauche_ = false;
          else if (p[position_.x + (position_.y - dist_) * engine_in->ScreenWidth ()] != background_in)
            gauche_ = false;
          if (position_.y + dist_ > engine_in->ScreenHeight () - dist_)
            droite_ = false;
          else if (p[position_.x + (position_.y + dist_) * engine_in->ScreenWidth ()] != background_in)
            droite_ = false;
          break;
        case 3: // bas
          if (position_.y + dist_ > engine_in->ScreenHeight () - dist_)
            devant_ = false;
          else if (p[position_.x + (position_.y + dist_) * engine_in->ScreenWidth ()] != background_in)
            devant_ = false;
          if (position_.x + dist_ > engine_in->ScreenWidth () - dist_)
            gauche_ = false;
          else if (p[(position_.x + dist_) + position_.y * engine_in->ScreenWidth ()] != background_in)
            gauche_ = false;
          if (position_.x - dist_ < dist_)
            droite_ = false;
          else if (p[(position_.x - dist_) + position_.y * engine_in->ScreenWidth ()] != background_in)
            droite_ = false;
          break;
      } // end SWITCH
    }

    void brancher ()
    {
      if (!Common_Tools::testRandomProbability (0.2f)) // 20%
        return;
      // --> branch only 20% of the time

      if (devant_)
      {
        if (Common_Tools::getRandomNumber (0, 2) > 1)
        {
          if (gauche_)
            branche_ = new snake (position_.x, position_.y, (direction_ - 1 + 4) % 4, dist_);
          else if (droite_)
            branche_ = new snake (position_.x, position_.y, (direction_ + 1) % 4, dist_);
        } // end IF
        else
        {
          if (droite_)
            branche_ = new snake (position_.x, position_.y, (direction_ + 1) % 4, dist_);
          else if (gauche_)
            branche_ = new snake (position_.x, position_.y, (direction_ - 1 + 4) % 4, dist_);
        } // end ELSE
      } // end IF
    }

    void avancer ()
    {
      if (!devant_ && !gauche_ && !droite_)
      {
        vie_ = false;
        return;
      } // end IF

      if (devant_ && (gauche_ || droite_))
      {
        if (Common_Tools::testRandomProbability (0.98f)) // 98%
        {
          if (Common_Tools::getRandomNumber (0, 2) > 1)
          {
            if (gauche_)
              direction_ = (direction_ + 4 - 1) % 4;
            else if (droite_)
              direction_ = (direction_ + 1) % 4;
          } // end IF
          else
          {
            if (droite_)
              direction_ = (direction_ + 1) % 4;
            else if (gauche_)
              direction_ = (direction_ + 4 - 1) % 4;
          } // end ELSE
        } // end IF
      } // end IF
      else if (!devant_ && (gauche_ || droite_))
      {
        if (Common_Tools::getRandomNumber (0, 2) > 1)
        {
          if (gauche_)
            direction_ = (direction_ + 4 - 1) % 4;
          else if (droite_)
            direction_ = (direction_ + 1) % 4;
        } // end IF
        else
        {
          if (droite_)
            direction_ = (direction_ + 1) % 4;
          else if (gauche_)
            direction_ = (direction_ + 4 - 1) % 4;
        } // end ELSE
      } // end ELSE IF

      switch (direction_)
      {
        case 0: // gauche
          position_.x -= dist_;
          break;
        case 1: // haut
          position_.y -= dist_;
          break;
        case 2: // droite
          position_.x += dist_;
          break;
        case 3: // bas
          position_.y += dist_;
          break;
      } // end SWITCH
    }

    void dessiner (olc::PixelGameEngine* engine_in)
    {
      if (!vie_) return;

      olc::Pixel* p = engine_in->GetDrawTarget ()->GetData ();
      switch (direction_)
      {
        case 0:
          for (int i = 0; i <= dist_; i++)
            p[(position_.x + i) + position_.y * engine_in->ScreenWidth ()] = olc::BLACK;
          break;
        case 1:
          for (int i = 0; i <= dist_; i++)
            p[position_.x + (position_.y + i) * engine_in->ScreenWidth ()] = {100, 100, 100, 255};
          break;
        case 2:
          for (int i = 0; i <= dist_; i++)
            p[(position_.x - i) + position_.y * engine_in->ScreenWidth ()] = {150, 150, 150, 255};
          break;
        case 3:
          for (int i = 0; i <= dist_; i++)
            p[position_.x + (position_.y - i) * engine_in->ScreenWidth ()] = {200, 200, 200, 255};
          break;
      } // end SWITCH
    }

    void run (olc::PixelGameEngine* engine_in,
              const olc::Pixel& background_in)
    {
      branche_ = NULL;
      tester (engine_in, background_in);
      brancher ();
      avancer ();
      dessiner (engine_in);
    }

    olc::vi2d position_;
    int32_t   direction_;
    int32_t   dist_;
    bool      devant_;
    bool      droite_;
    bool      gauche_;
    bool      vie_;
    snake*    branche_;
  };

  std::vector<snake*> snakes_;
  int                 depart_;
  int                 dist_;
  olc::Pixel          background_;

  void reset ();
};

#endif // PGE_81_H
