#ifndef PGE_262_H
#define PGE_262_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_262
 : public olc::PixelGameEngine
{
 public:
  PGE_262 ();
  virtual ~PGE_262 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class quadrat
  {
   public:
    quadrat (int ww, int hh, int aus, uint8_t color, bool r)
     : mode_ (0)
     , step_ (0)
     , position_ (0.0f, 0.0f)
     , w_ (ww)
     , h_ (hh)
     , color_ (color)
     , a_ (aus)
     , richtung_ (r)
    {
      if (!richtung_)
        mode_ = 1;
      position_.x = ww / 26.0f;
      position_.y = hh / 26.0f;
    }

    void display (olc::vf2d& pos, olc::PixelGameEngine* engine_in)
    {
      olc::Pixel color (color_, color_, color_, 255);
      engine_in->FillRect (static_cast<int32_t> (pos.x + position_.x), static_cast<int32_t> (pos.y + position_.y),
                           6 * w_ / 7, 6 * h_ / 7,
                           color);
    }

    void move (int maxstep)
    {
      static int dx[4] = {0, 1, 0, -1};
      static int dy[4] = {1, 0, -1, 0};

      if (richtung_)
      {
        position_.x += (dx[mode_] * 0.5f * w_) / (7.0f * maxstep);
        position_.y += (dy[mode_] * 0.5f * w_) / (7.0f * maxstep);
      } // end IF
      else
      {
        position_.x += (dx[mode_] * 0.5f * w_) / (7.0f * maxstep);
        position_.y += (dy[mode_] * 0.5f * w_) / (7.0f * maxstep);
      } // end ELSE

      if (step_ > maxstep)
      {//(w-6*w/7-w/10)){
        step_ = 0;
        if (richtung_)
          mode_ = (mode_ + 1) % 4;
        else
        {
          --mode_;
          if (mode_ < 0)
            mode_ = 3;
        } // end ELSE
      } // end IF
      ++step_;
    }

    int       mode_;
    int       step_;
    olc::vf2d position_;
    int       w_;
    int       h_;
    uint8_t   color_;
    int       a_;
    bool      richtung_;
  };
   
  int                  m_;
  int                  maxStep_;
  std::vector<uint8_t> cc_;
  int*                 step_;
  int*                 limit_;
  int*                 nlimit_;
  std::vector<quadrat> q_;
};

#endif // PGE_262_H
