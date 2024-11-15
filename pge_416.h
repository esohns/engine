#ifndef PGE_416_H
#define PGE_416_H

#include <deque>
#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_19.h"

class PGE_416
 : public olc::PixelGameEngine
{
 public:
  PGE_416 ();
  inline virtual ~PGE_416 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct state
  {
    olc::vf2d position;
    float     heading;
  };

  class agent
  {
   public:
    agent (std::vector<std::vector<bool> >& world,
           olc::PixelGameEngine* engine)
     : position_ (Common_Tools::getRandomNumber (0, engine->ScreenWidth () - 1),
                  Common_Tools::getRandomNumber (0, engine->ScreenHeight () - 1))
     , stack_ ()
    {
      struct state state_s = { position_, 0.0f };
      stack_.push_front (state_s);
      world[position_.x][position_.y] = true;
      engine->Draw (position_, olc::BLACK);
    }

    void update (std::vector<std::vector<bool> >& world,
                 noise::module::Perlin* noise,
                 int frameCount,
                 olc::PixelGameEngine* engine)
    {
      static int32_t screen_width_i = engine->ScreenWidth ();
      static int32_t screen_height_i = engine->ScreenHeight ();

      bool done_b = false, pop_b = true;
      float heading;
      while (!stack_.empty () && !done_b)
      {
        struct state top_s = stack_.front ();

        int num =
          Common_Tools::testRandomProbability (ENGINE_PGE_416_DEFAULT_BRANCHING_PROBABILITY) ? 2 : 1;
        heading = top_s.heading;
        for (int i = 0; i < num; i++, heading += static_cast<float> (M_PI_2))
        {
          float newX = static_cast<float> (top_s.position.x);
          float newY = static_cast<float> (top_s.position.y);
          do
          {
            float n = Common_GL_Tools::map (static_cast<float> (noise->GetValue (newX / static_cast<float> (screen_width_i),
                                                                                 newY / static_cast<float> (screen_height_i),
                                                                                 0.0/*static_cast<double> (frameCount) / 1000.0*/)),
                                            -1.0f, 1.0f, 0.0f, 1.0f) * ENGINE_PGE_416_DEFAULT_NOISE_FACTOR;
            newX = std::fmod (newX + std::cos (heading + n) + screen_width_i, static_cast<float> (screen_width_i));
            newY = std::fmod (newY + std::sin (heading + n) + screen_height_i, static_cast<float> (screen_height_i));
          } while (static_cast<int> (newX) == static_cast<int> (top_s.position.x) && static_cast<int> (newY) == static_cast<int> (top_s.position.y));

          if (world[static_cast<int> (newX)][static_cast<int> (newY)])
            continue;
          world[static_cast<int> (newX)][static_cast<int> (newY)] = true;

          struct state state_s;
          state_s.position.x = newX;
          state_s.position.y = newY;
          state_s.heading = heading;
          stack_.push_front (state_s);
          pop_b = false;

          engine->Draw (static_cast<int> (newX), static_cast<int> (newY), olc::BLACK);

          done_b = true;
        } // end FOR

        if (pop_b)
          stack_.pop_front ();
      } // end WHILE
    }

    olc::vi2d                position_;
    std::deque<struct state> stack_;
  };

  noise::module::Perlin           noise_;
  std::vector<agent>              agents_;
  std::vector<std::vector<bool> > world_;
};

#endif // PGE_416_H
