#ifndef PGE_33_H
#define PGE_33_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_33
 : public olc::PixelGameEngine
{
 public:
  PGE_33 ();
  virtual ~PGE_33 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (int x, int y)
     : position_ ({static_cast<float> (x), static_cast<float> (y)})
     , radius_ (4)
     , highlight_ (false)
    {}

    bool intersects (const particle& other)
    {
      float d = position_.dist (other.position_);
      return (d < radius_ + other.radius_);
    }

    void setHighlight (bool value)
    {
      highlight_ = value;
    }

    void move ()
    {
      position_.x += Common_Tools::getRandomNumber (-1.0f, 1.0f);
      position_.y += Common_Tools::getRandomNumber (-1.0f, 1.0f);
    }

    void render (olc::PixelGameEngine* engine_in)
    {
      if (highlight_)
        engine_in->FillCircle (position_, radius_, olc::WHITE);
      else
        engine_in->DrawCircle (position_, radius_, olc::WHITE, 0xFF);
    }

    olc::vf2d position_;
    int       radius_;
    bool      highlight_;
  };

  class point
  {
   public:
    point (int x, int y, void* userData)
     : position_ ({static_cast<float> (x), static_cast<float> (y)})
     , userData_ (userData)
    {}

    olc::vf2d position_;
    void*     userData_;
  };

  class rectangle
  {
   public:
    rectangle (int x, int y, int w, int h)
     : position_ ({static_cast<float> (x), static_cast<float> (y)})
     , width_ (w)
     , height_ (h)
    {}

    bool contains (const point& point_in) const
    {
      return (point_in.position_.x >= position_.x - width_   &&
              point_in.position_.x <= position_.x + width_   &&
              point_in.position_.y >= position_.y - height_  &&
              point_in.position_.y <= position_.y + height_);
    }

    bool intersects (const rectangle& range_in) const
    {
      return !(range_in.position_.x - range_in.width_ > position_.x + width_   ||
               range_in.position_.x + range_in.width_ < position_.x - width_   ||
               range_in.position_.y - range_in.height_ > position_.y + height_ ||
               range_in.position_.y + range_in.height_ < position_.y - height_);
    }

    olc::vf2d position_;
    int       width_;
    int       height_;
  };

  class circle
  {
   public:
    circle (int x, int y, int r)
     : position_ ({static_cast<float> (x), static_cast<float> (y)})
     , radius_ (r)
     , rSquared_ (r * r)
    {}

    bool contains (const point& point_in) const
    {
      // check if the point is in the circle by checking if the euclidean distance of
      // the point and the center of the circle if smaller or equal to the radius of
      // the circle
      float d =
        static_cast<float> (std::pow ((point_in.position_.x - position_.x), 2) + std::pow ((point_in.position_.y - position_.y), 2));
      return d <= rSquared_;
    }

    bool intersects (const rectangle& range_in) const
    {
      float xDist = std::abs (range_in.position_.x - position_.x);
      float yDist = std::abs (range_in.position_.y - position_.y);

      // no intersection
      if (xDist > (radius_ + range_in.width_) || yDist > (radius_ + range_in.height_))
        return false;

      // intersection within the circle
      if (xDist <= range_in.width_ || yDist <= range_in.height_)
        return true;

      // intersection on the edge of the circle
      float edges =
        static_cast<float> (std::pow ((xDist - range_in.width_), 2) + std::pow ((yDist - range_in.height_), 2));
      return edges <= rSquared_;
    }

    olc::vf2d position_;
    int       radius_;
    int       rSquared_;
  };

  class quad_tree
  {
   public:
    quad_tree (const rectangle& boundary_in,
               int capacity_in)
     : boundary_ (boundary_in)
     , capacity_ (capacity_in)
     , points_ ()
     , divided_ (false)
     , northeast_ (NULL)
     , northwest_ (NULL)
     , southeast_ (NULL)
     , southwest_ (NULL)
    {}
    ~quad_tree ()
    {
      delete northeast_;
      delete northwest_;
      delete southeast_;
      delete southwest_;
    }

    void subdivide ()
    {
      int x = static_cast<int> (boundary_.position_.x);
      int y = static_cast<int> (boundary_.position_.y);
      int w = static_cast<int> (boundary_.width_ / 2.0f);
      int h = static_cast<int> (boundary_.height_ / 2.0f);

      rectangle ne (x + w, y - h, w, h);
      northeast_ = new quad_tree (ne, capacity_);
      rectangle nw (x - w, y - h, w, h);
      northwest_ = new quad_tree (nw, capacity_);
      rectangle se (x + w, y + h, w, h);
      southeast_ = new quad_tree (se, capacity_);
      rectangle sw (x - w, y + h, w, h);
      southwest_ = new quad_tree (sw, capacity_);

      divided_ = true;
    }

    bool insert (const point& point_in)
    {
      if (!boundary_.contains (point_in))
        return false;

      if (points_.size () < capacity_)
      {
        points_.push_back (point_in);
        return true;
      } // end IF

      if (!divided_)
        subdivide ();

      return (northeast_->insert (point_in) ||
              northwest_->insert (point_in) ||
              southeast_->insert (point_in) ||
              southwest_->insert (point_in));
    }

    typedef std::vector<point> points_t;
    void query (const circle& range_in, points_t& result_in)
    {
      if (!range_in.intersects (boundary_))
        return;

      for (points_t::const_iterator iterator = points_.begin ();
           iterator != points_.end ();
           ++iterator)
        if (range_in.contains (*iterator))
          result_in.push_back (*iterator);

      if (divided_)
      {
        northwest_->query (range_in, result_in);
        northeast_->query (range_in, result_in);
        southwest_->query (range_in, result_in);
        southeast_->query (range_in, result_in);
      } // end IF
    }

    rectangle  boundary_;
    int        capacity_;
    points_t   points_;
    bool       divided_;
    quad_tree* northeast_;
    quad_tree* northwest_;
    quad_tree* southeast_;
    quad_tree* southwest_;
  };

  typedef std::vector<particle*> particles_t;
  particles_t particles_;
  quad_tree*  quadTree_;
};

#endif // PGE_33_H
