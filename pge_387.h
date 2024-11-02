#ifndef PGE_387_H
#define PGE_387_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_17.h"

class PGE_387
 : public olc::PixelGameEngine
{
 public:
  PGE_387 ();
  virtual ~PGE_387 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class boid
  {
   public:
    boid (float x, float y)
     : position_ (x, y)
     , velocity_ (Common_Tools::getRandomNumber (0.0f, 1.0f), Common_Tools::getRandomNumber (0.0f, 1.0f))
     , acceleration_ (0.0f, 0.0f)
    {
      velocity_ *= Common_Tools::getRandomNumber (-3.0f, 3.0f);
    }

    void edges (olc::PixelGameEngine* engine)
    {
      static float screen_width_f = static_cast<float> (engine->ScreenWidth ());
      static float screen_height_f = static_cast<float> (engine->ScreenHeight ());

      if (position_.x > screen_width_f)
        position_.x = 0.0f;
      else if (position_.x < 0.0f)
        position_.x = screen_width_f - 1.0f;

      if (position_.y > screen_height_f)
        position_.y = 0.0f;
      else if (position_.y < 0.0f)
        position_.y = screen_height_f - 1.0f;
    }

    olc::vf2d alignment (std::vector<boid*>& boids)
    {
      olc::vf2d steering;
      int total = 0;

      for (int i = 0; i < static_cast<int> (boids.size ()); i++)
      {
        boid& other = *boids[i];
        float distance = position_.dist (other.position_);
        if (&other != this && distance < ENGINE_PGE_387_DEFAULT_PERCEPTION_RADIUS)
        {
          steering += other.velocity_;
          total += 1;
        } // end IF
      } // end FOR

      if (total > 0)
      {
        steering /= static_cast<float> (total);
        steering = steering.norm ();
        steering *= ENGINE_PGE_387_DEFAULT_MAX_SPEED;
        steering -= velocity_;
        static olc::vf2d upper_bound = { ENGINE_PGE_387_DEFAULT_MAX_FORCE, ENGINE_PGE_387_DEFAULT_MAX_FORCE };
        static olc::vf2d lower_bound = { -ENGINE_PGE_387_DEFAULT_MAX_FORCE, -ENGINE_PGE_387_DEFAULT_MAX_FORCE };
        steering = steering.clamp (lower_bound, upper_bound);
      } // end IF

      return steering;
    }

    olc::vf2d cohesion (std::vector<boid*>& boids)
    {
      olc::vf2d steering;
      int total = 0;

      for (int i = 0; i < static_cast<int> (boids.size()); i++)
      {
        boid& other = *boids[i];
        float distance = position_.dist (other.position_);
        if (&other != this && distance < ENGINE_PGE_387_DEFAULT_PERCEPTION_RADIUS)
        {
          steering += other.position_;
          total += 1;
        } // end IF
      } // end FOR

      if (total > 0)
      {
        steering /= static_cast<float> (total);
        steering -= position_;
        steering = steering.norm ();
        steering *= ENGINE_PGE_387_DEFAULT_MAX_SPEED;
        steering -= velocity_;
        static olc::vf2d upper_bound = { ENGINE_PGE_387_DEFAULT_MAX_FORCE, ENGINE_PGE_387_DEFAULT_MAX_FORCE };
        static olc::vf2d lower_bound = { -ENGINE_PGE_387_DEFAULT_MAX_FORCE, -ENGINE_PGE_387_DEFAULT_MAX_FORCE };
        steering = steering.clamp (lower_bound, upper_bound);
      } // end IF

      return steering;
    }

    olc::vf2d separation (std::vector<boid*>& boids)
    {
      olc::vf2d steering;
      int total = 0;

      for (int i = 0; i < static_cast<int> (boids.size ()); i++)
      {
        boid& other = *boids[i];
        float distance_f = position_.dist (other.position_);
        if (&other != this && distance_f < ENGINE_PGE_387_DEFAULT_PERCEPTION_RADIUS)
        {
          olc::vf2d neighborVector = position_ - other.position_;
          neighborVector /= (distance_f * distance_f);
          steering += neighborVector;
          total += 1;
        } // end IF
      } // end FOR

      if (total > 0)
      {
        steering /= static_cast<float> (total);
        steering = steering.norm ();
        steering *= ENGINE_PGE_387_DEFAULT_MAX_SPEED;
        steering -= velocity_;
        static olc::vf2d upper_bound = { ENGINE_PGE_387_DEFAULT_MAX_FORCE, ENGINE_PGE_387_DEFAULT_MAX_FORCE };
        static olc::vf2d lower_bound = { -ENGINE_PGE_387_DEFAULT_MAX_FORCE, -ENGINE_PGE_387_DEFAULT_MAX_FORCE };
        steering = steering.clamp (lower_bound, upper_bound);
      } // end IF

      return steering;
    }

    void flock (std::vector<boid*>& boids)
    {
      olc::vf2d alignment_s = alignment (boids);
      olc::vf2d cohesion_s = cohesion (boids);
      olc::vf2d separation_s = separation (boids);

      acceleration_ += alignment_s;
      acceleration_ += cohesion_s;
      acceleration_ += separation_s;
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      edges (engine_in);
      
      velocity_ += acceleration_;
      position_ += velocity_;
      acceleration_ *= 0.0f;
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, 1, olc::WHITE, 0xFF);
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
  };

  class point
  {
   public:
    point (float x_, float y_, void* userData_)
     : x (x_)
     , y (y_)
     , userData (userData_)
    {}

    float x;
    float y;
    void* userData;
  };

  class rectangle
  {
   public:
    rectangle (float x_, float y_, float w_, float h_)
     : x (x_)
     , y (y_)
     , w (w_)
     , h (h_)
    {}
  
    inline bool contains (point& point)
    {
      return (point.x >= x     &&
              point.x <  x + w &&
              point.y >= y     &&
              point.y <  y + h);
    }

    bool intersects (rectangle& boundary)
    {
      float boundaryR = boundary.x + boundary.w;
      float boundaryL = boundary.x;
      float boundaryT = boundary.y;
      float boundaryB = boundary.y + boundary.h;
    
      float rangeR = x + w;
      float rangeL = x;
      float rangeT = y;
      float rangeB = y + h;

      return (boundaryR >= rangeL &&
              boundaryL <= rangeR &&
              boundaryT <= rangeB &&
              boundaryB >= rangeT);
    }

    float x;
    float y;
    float w;
    float h;
  };

  class circle
  {
   public:
    circle (float x_, float y_, float r_)
     : x (x_)
     , y (y_)
     , r (r_)
    {}
  
    bool contains (point& point)
    {
      float distX = std::abs (x - point.x);
      float distY = std::abs (y - point.y);
      float distance = std::sqrt ((distX * distX) + (distY * distY));
  
      return (distance <= r);
    }

    bool intersects (rectangle& boundary)
    {
      float testX = x;
      float testY = y;

      // which edge is closest?
      if (x < boundary.x)
        testX = boundary.x;                  // test left edge
      else if (x > boundary.x + boundary.w)
        testX = boundary.x + boundary.w;     // right edge
      if (y < boundary.y)
        testY = boundary.y;                  // top edge
      else if (y > boundary.y + boundary.h)
        testY = boundary.y + boundary.h;     // bottom edge

      // get distance from closest edges
      float distX = x - testX;
      float distY = y - testY;
      float distance = std::sqrt ((distX * distX) + (distY * distY));

      // if the distance is less than the radius, collision!
      return (distance <= r);
    }

    float x;
    float y;
    float r;
  };

  class quadtree
  {
   public:
    quadtree (rectangle& boundary_, int capacity_)
     : boundary (boundary_)
     , capacity (capacity_)
     , points ()
     , divided (false)
     , northeast (NULL)
     , northwest (NULL)
     , southeast (NULL)
     , southwest (NULL)
    {}

    ~quadtree ()
    {
      if (divided)
      {
        delete northeast;
        delete northwest;
        delete southeast;
        delete southwest;
      } // end IF
    }

    void clear ()
    {
      points.clear ();
      if (divided)
      {
        delete northeast; northeast = NULL;
        delete northwest; northwest = NULL;
        delete southeast; southeast = NULL;
        delete southwest; southwest = NULL;
      } // end IF
      divided = false;
    }
  
    bool insert (point& point)
    {
      if (!boundary.contains (point))
        return false;
    
      if (points.size () < capacity)
      {
        points.push_back (point);
        return true;
      }
      
      if (!divided)
        subdivide (); 

      if (northeast->insert(point))
        return true;
      else if (northwest->insert (point))
        return true;
      else if (southeast->insert (point))
        return true;
      else if (southwest->insert (point))
        return true;

      return false;
    }
  
    void subdivide ()
    {
      rectangle northeastBoundary (boundary.x + boundary.w / 2.0f, boundary.y, boundary.w / 2.0f, boundary.h / 2.0f);
      northeast = new quadtree (northeastBoundary, capacity);
      rectangle northwestBoundary (boundary.x, boundary.y, boundary.w / 2.0f, boundary.h / 2.0f);
      northwest = new quadtree (northwestBoundary, capacity);
      rectangle southeastBoundary (boundary.x + boundary.w / 2.0f, boundary.y + boundary.h / 2.0f, boundary.w / 2.0f, boundary.h / 2.0f);
      southeast = new quadtree (southeastBoundary, capacity);
      rectangle southwestBoundary (boundary.x, boundary.y + boundary.h / 2.0f, boundary.w / 2.0f, boundary.h / 2.0f);
      southwest = new quadtree (southwestBoundary, capacity);

      divided = true;
    }
  
    bool query (circle& range, std::vector<boid*>& found)
    {
      if (!range.intersects (boundary))
        return false;

      for (int i=0; i < static_cast<int> (points.size ()); i++)
        if (range.contains (points[i]))
          found.push_back (static_cast<boid*> (points[i].userData));

      if (divided)
      {
        northeast->query (range, found);
        northwest->query (range, found);
        southeast->query (range, found);
        southwest->query (range, found);
      } // end IF

      return !found.empty ();
    }
  
    void draw (olc::PixelGameEngine* engine)
    {
      engine->DrawRect (static_cast<int32_t> (boundary.x), static_cast<int32_t> (boundary.y),
                        static_cast<int32_t> (boundary.w), static_cast<int32_t> (boundary.h),
                        olc::WHITE);
    
      if (divided)
      {
        northeast->draw (engine);
        northwest->draw (engine);
        southeast->draw (engine);
        southwest->draw (engine);
      } // end IF
    }

    rectangle          boundary;
    int                capacity;
    std::vector<point> points;
    bool               divided;

    quadtree*          northeast;
    quadtree*          northwest;
    quadtree*          southeast;
    quadtree*          southwest;
  };

  std::vector<boid>                boids_;
  quadtree*                        quadtree_;
};

#endif // PGE_387_H
