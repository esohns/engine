#ifndef PGE_429_H
#define PGE_429_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_19.h"

void draw_circle_arc (olc::vf2d&, float, float, float, olc::Pixel&, olc::PixelGameEngine*);

//////////////////////////////////////////

class PGE_429
 : public olc::PixelGameEngine
{
 public:
  PGE_429 ();
  virtual ~PGE_429 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class element
  {
   public:
    virtual void initialize (float) = 0;
    virtual void draw (int, int, float, olc::PixelGameEngine*) = 0;
  };

  class arc
   : public element
  {
   public:
    arc (olc::vf2d& topLeft_, std::vector<olc::Pixel>& colors_)
     : topLeft (topLeft_)
     , center (static_cast<int> (std::floor (Common_Tools::getRandomNumber (0.0f, 1.0f) * 4.0f)))
     , colors (colors_)
    {}

    virtual void initialize (float size)
    {
      center =
        static_cast<int> (std::floor (Common_Tools::getRandomNumber (0.0f, 1.0f) * 4.0f));
    }

    virtual void draw (int lifespan, int direction, float size, olc::PixelGameEngine* engine)
    {
      olc::vf2d circle_center_s;
      circle_center_s.x = center % 3 == 0 ? topLeft.x : topLeft.x + size;
      circle_center_s.y = center < 2 ? topLeft.y : topLeft.y + size;
      float angle =
        Common_GL_Tools::map (static_cast<float> (lifespan), 0.0f, 255.0f, 0.0f, static_cast<float> (M_PI_2));
      int n = 0;
      if (direction == 1)
      {
        float begin = center * static_cast<float> (M_PI_2);
        for (float scl = 1.0f; scl > 0.0f; scl -= 1.0f / 3.0f + 0.01f)
        {
          olc::Pixel color = colors[n++ % colors.size ()];
          draw_circle_arc (circle_center_s, size * 2.0f * scl, begin, begin + angle, color, engine);
        } // end FOR
      } // end IF
      else
      {
        float end = (center + 1) * static_cast<float> (M_PI_2);
        for (float scl = 1.0f; scl > 0.0f; scl -= 1.0f / 3.0f + 0.01f)
        {
          olc::Pixel color = colors[n++ % colors.size()];
          draw_circle_arc (circle_center_s, size * 2.0f * scl, end - angle, end, color, engine);
        } // end FOR
      } // end ELSE
    }

    olc::vf2d               topLeft;
    int                     center;
    std::vector<olc::Pixel> colors;
  };

  class line
   : public element
  {
   public:
    line (olc::vf2d& topLeft_, float size, std::vector<olc::Pixel>& colors_)
     : topLeft (topLeft_)
     , beginIndex (0)
     , colors (colors_)
     , begin ()
     , end ()
    {
      initialize (size);
    }

    virtual void initialize (float size)
    {
      beginIndex = static_cast<int> (std::floor (Common_Tools::getRandomNumber (0.0f, 1.0f) * 4.0f));
      begin = { size, 0.0f };
      end = { 0.0f, size };
    }

    virtual void draw (int lifespan, int direction, float size, olc::PixelGameEngine* engine)
    {
      olc::vf2d move;
      olc::vf2d translate_s (topLeft.x + size / 2.0f, topLeft.y + size / 2.0f);
      //rotate((beginIndex * 2.0f * M_PI) / 4.0f);
      olc::vf2d translate_2 (-size / 2.0f, -size / 2.0f);
      int n = 0;
      for (float scl = 1.0f; scl > 0.0f; scl -= 1.0f / 3.0f + 1.0f / 100.0f)
      {
        //scale(scl);
        olc::Pixel color = colors[n++ % colors.size ()];

        if (direction == 1)
        {
          move.x =
            Common_GL_Tools::map (static_cast<float> (lifespan), 0.0f, 255.0f, begin.x, end.x);
          move.y =
            Common_GL_Tools::map (static_cast<float> (lifespan), 0.0f, 255.0f, begin.y, end.y);
          engine->DrawLine (translate_s + translate_2 + begin, translate_s + translate_2 + move, color, 0xFFFFFFFF);
        } // end IF
        else
        {
          move.x =
            Common_GL_Tools::map (static_cast<float> (lifespan), 0.0f, 255.0f, end.x, begin.x);
          move.y =
            Common_GL_Tools::map (static_cast<float> (lifespan), 0.0f, 255.0f, end.y, begin.y);
          engine->DrawLine (translate_s + translate_2 + move, translate_s + translate_2 + end, color, 0xFFFFFFFF);
        } // end ELSE
      } // end FOR
    }

    olc::vf2d               topLeft;
    int                     beginIndex;
    std::vector<olc::Pixel> colors;
    olc::vf2d               begin;
    olc::vf2d               end;
  };

  class module
  {
   public:
    module (const olc::vi2d& index_, float size, olc::vf2d& margin, std::vector<olc::Pixel>& colors, olc::PixelGameEngine* engine)
     : index (index_)
     , position (index_.x * size + margin.x,
                 index_.y * size + margin.y)
     , lifespan (0)
     , speed (ENGINE_PGE_429_DEFAULT_SPEED)
     , direction (1)
     , delay (0)
     , element_ (NULL)
    {
      static olc::vf2d center_s (engine->ScreenWidth () / 2.0f, engine->ScreenHeight () / 2.0f);
      float distance =
        position.dist (center_s) / std::sqrt (static_cast<float> (engine->ScreenWidth ()  * engine->ScreenWidth () +
                                                                  engine->ScreenHeight () * engine->ScreenHeight ()));
      delay = static_cast<int> (1500.0f * (distance * distance)); //Math.floor(random() * 00);

      std::vector<olc::Pixel> colors_a = colors;
      static std::random_device rd;
      static std::default_random_engine rng (rd ());
      std::shuffle (colors_a.begin (), colors_a.end (), rng);

      if (Common_Tools::testRandomProbability (0.5f))
        element_ = new arc (position, colors_a);
      else
        element_ = new line (position, size, colors_a);
    }

    ~module ()
    {
      delete element_;
    }

    void draw (float size, olc::PixelGameEngine* engine)
    {
      if (delay > 0)
      {
        --delay;
        return;
      } // end IF

      if (direction == 1 && lifespan >= 255)
      {
        direction = -1;
        lifespan = 255;
      } // end IF
      else if (direction == -1 && lifespan <= 0)
      {
        direction = 1;
        lifespan = 0;
        delay = 500;
        element_->initialize (size);
      } // end ELSE IF

      element_->draw (lifespan, direction, size, engine);
      lifespan += direction * speed;
    }

    olc::vi2d index;
    olc::vf2d position;
    int       lifespan;
    int       speed;
    int       direction;
    int       delay;
    element*  element_;
  };

  void initialize ();

  std::vector<module*> modules_;
  float                size_;
};

#endif // PGE_429_H
