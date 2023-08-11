#ifndef PGE_95_H
#define PGE_95_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_4.h"

// *NOTE*: see also: https://openprocessing.org/sketch/1980755

class PGE_95
 : public olc::PixelGameEngine
{
 public:
  PGE_95 ();
  virtual ~PGE_95 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class e_point
  {
  public:
    e_point (float theta, int32_t cellWidth, int32_t cellHeight)
     : theta_(theta)
     , position_()
    {
      initialize (cellWidth, cellHeight);
    }

    void initialize (int32_t cellWidth, int32_t cellHeight)
    {
      float r =
        cellWidth * cellHeight / static_cast<float> (std::sqrt (std::pow (cellHeight * std::cos (theta_), 2) + std::pow (cellWidth * std::sin (theta_), 2)));
      position_.x = r * std::cos (theta_);
      position_.y = r * std::sin (theta_);
    }

    void update (int32_t cellWidth, int32_t cellHeight, float thetarate)
    {
      float r =
        cellWidth * cellHeight / static_cast<float> (std::sqrt (std::pow (cellHeight * std::cos (theta_), 2) + std::pow (cellWidth * std::sin (theta_), 2)));
      position_.x = r * std::cos (theta_);
      position_.y = r * std::sin (theta_);

      theta_ += thetarate * static_cast<float> (M_PI);
    }

    //void tangential (int32_t cellWidth, int32_t cellHeight,
    //                olc::PixelGameEngine* engine_in)
    //{
    //  float tx = cellWidth * cellHeight / position_.x;
    //  float ty = cellWidth * cellHeight / position_.y;
    //  engine_in->DrawLine (static_cast<int32_t> (tx), 0, 0, static_cast<int32_t> (ty), olc::WHITE, 0xFFFFFFFF);
    //  engine_in->DrawCircle (position_, 10, olc::RED, 0xFF);
    //}

    float     theta_;
    olc::vf2d position_;
  };

  class cell
  {
   public:
    cell (int32_t col, int32_t row, int32_t cellWidth, int32_t cellHeight, olc::vf2d& origin)
     : column_ (col)
     , row_ (row)
     , color_ (olc::WHITE)
     , e_points_ ()
     , t_points_ ()
     , turn_ (std::floor (ENGINE_PGE_95_DEFAULT_HEIGHT / 400.0f))
     , pause_ ((col + row) * 15)
     , count_ (0)
     , position_ ()
    {
      static olc::Pixel colors_a[5] =
        {{0xe6, 0x39, 0x46, 0xFF},{0xf1, 0xfa, 0xee, 0xFF},{0xa8, 0xda, 0xdc, 0xFF},{0x45, 0x7b, 0x9d, 0xFF},{0x1d, 0x35, 0x57, 0xFF}};
      color_ = colors_a[Common_Tools::getRandomNumber (0, 4)];
      color_.a = 120;

      initialize (cellWidth, cellHeight, origin);
    }

    ~cell ()
    {
      for (std::vector<e_point*>::iterator iterator = e_points_.begin ();
           iterator != e_points_.end ();
           ++iterator)
        delete *iterator;
    }

    void initialize (int32_t cellWidth, int32_t cellHeight, olc::vf2d& origin)
    {
      for (int i = 0; i < 4; i++)
        e_points_.push_back (new e_point (i * static_cast<float> (M_PI) / 2.0f - static_cast<float> (M_PI) / 4.0f, cellWidth, cellHeight));

      float w = crosspoint (cellWidth, cellHeight, *e_points_[0], *e_points_[1]).x;
      float h = crosspoint (cellWidth, cellHeight, *e_points_[1], *e_points_[2]).y;

      position_.x = origin.x + (column_ - row_) * w;
      position_.y = origin.y + (column_ + row_) * h;

      for (size_t i = 0; i < e_points_.size (); i++)
      {
        if (i == 3)
          t_points_.push_back (crosspoint (cellWidth, cellHeight, *e_points_[i], *e_points_[0]));
        else
          t_points_.push_back (crosspoint (cellWidth, cellHeight, *e_points_[i], *e_points_[i + 1]));
        t_points_[i] += position_;
      } // end FOR
    }

    void update (int32_t cellWidth, int32_t cellHeight, float thetarate, float moverate)
    {
      if (count_ < pause_)
        count_++;
      else
      {
        for (std::vector<e_point*>::iterator iterator = e_points_.begin ();
            iterator != e_points_.end ();
            ++iterator)
          (*iterator)->update (cellWidth, cellHeight, thetarate);

        t_points_.clear ();
        for (size_t i = 0; i < e_points_.size (); i++)
        {
          if (i == 3)
            t_points_.push_back (crosspoint (cellWidth, cellHeight, *e_points_[i], *e_points_[0]));
          else
            t_points_.push_back (crosspoint (cellWidth, cellHeight, *e_points_[i], *e_points_[i + 1]));
          t_points_[i] += position_;
        } // end FOR

        position_.y -= moverate;
        //turn_ -= thetarate * 2.0f;
        turn_ -= thetarate;
      } // end ELSE
    }

    olc::vf2d crosspoint (int32_t cellWidth, int32_t cellHeight, e_point& p1, e_point& p2)
    {
      float x =
        cellWidth * cellWidth * (p1.position_.y - p2.position_.y) / (p2.position_.x * p1.position_.y - p1.position_.x * p2.position_.y);
      float y =
        cellHeight * cellHeight * (p1.position_.x - p2.position_.x) / (p1.position_.x * p2.position_.y - p2.position_.x * p1.position_.y);

      return olc::vf2d (x, y);
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      std::vector<olc::vf2d>::iterator iterator_last = t_points_.end ();
      std::advance (iterator_last, -1);
      for (std::vector<olc::vf2d>::iterator iterator = t_points_.begin ();
          iterator != t_points_.end ();
          ++iterator)
      {
        if (iterator != iterator_last)
        {
          std::vector<olc::vf2d>::iterator iterator_next = iterator;
          std::advance (iterator_next, 1);
          engine_in->DrawLine (*iterator, *iterator_next, color_, 0xFFFFFFFF);
        } // end IF
        else
        {
          std::vector<olc::vf2d>::iterator iterator_first = t_points_.begin ();
          engine_in->DrawLine (*iterator, *iterator_first, color_, 0xFFFFFFFF);
        } // end ELSE
      } // end FOR
    }

    bool isDone ()
    {
      return turn_ < 0.0f;
    }

    int32_t                column_;
    int32_t                row_;
    olc::Pixel             color_;
    std::vector<e_point*>  e_points_;
    std::vector<olc::vf2d> t_points_;
    float                  turn_;
    int32_t                pause_;
    int32_t                count_;
    olc::vf2d              position_;
  };

  int32_t            cellWidth_;
  int32_t            cellHeight_;
  float              thetaRate_;
  float              moveRate_;
  olc::vf2d          origin_;
  std::vector<cell*> cells_;
};

#endif // PGE_95_H
