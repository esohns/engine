#ifndef PGE_403_H
#define PGE_403_H

#include <list>
#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "defines_18.h"

class PGE_403
 : public olc::PixelGameEngine
{
 public:
  PGE_403 ();
  inline virtual ~PGE_403 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class snake
  {
   public:
    snake ()
     : list ()
     , isDead (false)
    {
      olc::vf2d position (Common_Tools::getRandomNumber (-ENGINE_PGE_403_DEFAULT_CELL_SIZE / 2.0f, ENGINE_PGE_403_DEFAULT_CELL_SIZE / 2.0f),
                          Common_Tools::getRandomNumber (-ENGINE_PGE_403_DEFAULT_CELL_SIZE / 2.0f, ENGINE_PGE_403_DEFAULT_CELL_SIZE / 2.0f));
      list.push_back (position);

      float hue_f = Common_Tools::getRandomNumber (0.0f, 360.0f);
      float saturation_f = Common_Tools::getRandomNumber (10.0f, 70.0f) / 100.0f;
      float value_f = 100.0f / 100.0f;
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue_f, saturation_f, value_f, r, g, b);
      color.r = static_cast<uint8_t> (r * 255.0f);
      color.g = static_cast<uint8_t> (g * 255.0f);
      color.b = static_cast<uint8_t> (b * 255.0f);

      static olc::vf2d v (1.0f, 0.0f);
      float a = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      float x_rotated = v.x * std::cos (a) - v.y * std::sin (a);
      float y_rotated = v.y * std::sin (a) + v.x * std::cos (a);
      direction = { x_rotated, y_rotated };
    }

    void update ()
    {
      if (Common_Tools::testRandomProbability (ENGINE_PGE_403_DEFAULT_TURN_PROBABILITY))
      {
        static float a = static_cast<float> (M_PI_2);
        float x_rotated = direction.x * std::cos (a) - direction.y * std::sin (a);
        float y_rotated = direction.y * std::sin (a) + direction.x * std::cos (a);
        direction = { x_rotated, y_rotated };
      } // end IF

      ACE_ASSERT (!list.empty ());
      olc::vf2d head = *(list.begin ());
      head += direction * ENGINE_PGE_403_DEFAULT_STEP;
      static float half_cell_size_f = ENGINE_PGE_403_DEFAULT_CELL_SIZE / 2.0f;
      if (std::abs (head.x) > half_cell_size_f || std::abs (head.y) > half_cell_size_f)
      {
        isDead = true;
        return;
      } // end IF

      list.push_front (head);
      while (list.size () > ENGINE_PGE_403_DEFAULT_SNAKE_LENGTH)
        list.pop_back ();
    }

    void show (olc::vf2d& position,
               float angle,
               olc::PixelGameEngine* engine)
    {
      static auto op1 = [](olc::vf2d& data)
      {
        return data;
      };
      static auto op2 = [](olc::vf2d& data)
      {
        // rotate by 2*PI/3
        static float a = 2.0f * static_cast<float> (M_PI) / 3.0f;
        float x_rotated = data.x * std::cos (a) - data.y * std::sin (a);
        float y_rotated = data.x * std::sin (a) + data.y * std::cos (a);

        olc::vf2d v (x_rotated, y_rotated);
        return v;
      };
      static auto op3 = [](olc::vf2d& data)
      {
        // rotate by 2*2*PI/3
        static float a = 2.0f * 2.0f * static_cast<float> (M_PI) / 3.0f;
        float x_rotated = data.x * std::cos (a) - data.y * std::sin (a);
        float y_rotated = data.x * std::sin (a) + data.y * std::cos (a);

        olc::vf2d v (x_rotated, y_rotated);
        return v;
      };
      static auto op4 = [](olc::vf2d& data)
      {
        return olc::vf2d (data.x, -data.y);
      };
      static auto op5 = [](olc::vf2d& data)
      {
        olc::vf2d v (data.x, -data.y);

        // rotate by 2*PI/3
        static float a = 2.0f * static_cast<float> (M_PI) / 3.0f;
        float x_rotated = v.x * std::cos (a) - v.y * std::sin (a);
        float y_rotated = v.x * std::sin (a) + v.y * std::cos (a);

        olc::vf2d v_2 (x_rotated, y_rotated);
        return v_2;
      };
      static auto op6 = [](olc::vf2d& data)
      {
        olc::vf2d v (data.x, -data.y);

        // rotate by 2*2*PI/3
        static float a = 2.0f * 2.0f * static_cast<float> (M_PI) / 3.0f;
        float x_rotated = v.x * std::cos (a) - v.y * std::sin (a);
        float y_rotated = v.x * std::sin (a) + v.y * std::cos (a);

        olc::vf2d v_2 (x_rotated, y_rotated);
        return v_2;
      };
      static std::vector<std::function<olc::vf2d (olc::vf2d&)> > symmetries6_a = { op1, op2, op3, op4, op5, op6 };
      for (std::vector<std::function<olc::vf2d (olc::vf2d&)> >::iterator iterator = symmetries6_a.begin ();
           iterator != symmetries6_a.end ();
           ++iterator)
      {
        olc::vf2d first, p1, p2;
        ACE_ASSERT (!list.empty ());
        p1 = (*iterator) (*list.begin ());
        float x_rotated = p1.x * std::cos (angle) - p1.y * std::sin (angle);
        float y_rotated = p1.x * std::sin (angle) + p1.y * std::cos (angle);
        p1 = { x_rotated , y_rotated };
        first = p1;
        std::list<olc::vf2d>::iterator iterator_2 = list.begin ();
        for (std::advance (iterator_2, 1);
             iterator_2 != list.end ();
             ++iterator_2)
        {
          p2 = (*iterator) (*iterator_2);
          x_rotated = p2.x * std::cos (angle) - p2.y * std::sin (angle);
          y_rotated = p2.x * std::sin (angle) + p2.y * std::cos (angle);
          p2 = { x_rotated , y_rotated };

          engine->DrawLine (position + p1, position + p2, color, 0xFFFFFFFF);

          p1 = p2;
        } // end FOR

        //engine->DrawLine (position + p1, position + first, color, 0xFFFFFFFF);
      } // end FOR
    }

    olc::Pixel           color;
    olc::vf2d            direction;
    std::list<olc::vf2d> list;
    bool                 isDead;
  };
   
  class cell
  {
   public:
    cell (float x, float y)
     : position (x, y)
     , snakes ()
     , angle (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
    {}

    void update (olc::PixelGameEngine* engine)
    {
      while (snakes.size () < ENGINE_PGE_403_DEFAULT_SNAKE_COUNT)
        snakes.push_back (snake ());

      olc::vf2d mouse_position_s (static_cast<float> (engine->GetMouseX ()),
                                  static_cast<float> (engine->GetMouseY ()));
      for (std::vector<snake>::iterator iterator = snakes.begin ();
           iterator != snakes.end ();
           ++iterator)
      {
        float distance = mouse_position_s.dist (position);
        angle += 1.0f / std::max (0.1f, distance);
      } // end FOR

      for (std::vector<snake>::iterator iterator = snakes.begin ();
           iterator != snakes.end ();
           ++iterator)
        (*iterator).update ();

      for (std::vector<snake>::iterator iterator = snakes.begin ();
           iterator != snakes.end ();
           )
      {
        if ((*iterator).isDead)
        {
          iterator = snakes.erase (iterator);
          continue;
        } // end IF

        ++iterator;
      } // end FOR
    }

    void show (olc::PixelGameEngine* engine)
    {
      for (std::vector<snake>::iterator iterator = snakes.begin ();
           iterator != snakes.end ();
           ++iterator)
        (*iterator).show (position, angle, engine);
    }

    olc::vf2d          position;
    std::vector<snake> snakes;
    float              angle;
  };

  std::vector<cell> cells_;
};

#endif // PGE_403_H
  