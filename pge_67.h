#ifndef PGE_67_H
#define PGE_67_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_3.h"

class PGE_67
 : public olc::PixelGameEngine
{
 public:
  PGE_67 ();
  virtual ~PGE_67 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class sand_painter
  {
   public:
    sand_painter ()
     : p_ (Common_Tools::getRandomNumber (0.0f, 1.0f))
     , color_ (olc::BLACK)
     , g_ (Common_Tools::getRandomNumber (0.01f, 0.1f))
    {}

    void render (float x, float y, float ox, float oy,
                 olc::PixelGameEngine* engine_in)
    {
      engine_in->Draw (static_cast<int32_t> (ox + (x - ox) * std::sin (p_)),
                       static_cast<int32_t> (oy + (y - oy) * std::sin (p_)),
                       {color_.r, color_.g, color_.b, 28});

      g_ += Common_Tools::getRandomNumber (-0.05f, 0.05f);
      float maxg = 0.22f;
      if (g_ < -maxg) g_ = -maxg;
      if (g_ > maxg) g_ = maxg;

      float w = g_ / 10.0f;
      for (int i = 0; i < 11; i++)
      {
        float alpha_f = 0.1f - i / 110.0f;
        engine_in->Draw (static_cast<int32_t> (ox + (x - ox) * std::sin (p_ + std::sin (i * w))),
                         static_cast<int32_t> (oy + (y - oy) * std::sin (p_ + std::sin (i * w))),
                         {color_.r, color_.g, color_.b, static_cast<uint8_t> (256 * alpha_f)});
        engine_in->Draw (static_cast<int32_t> (ox + (x - ox) * std::sin (p_ - std::sin (i * w))),
                         static_cast<int32_t> (oy + (y - oy) * std::sin (p_ - std::sin (i * w))),
                         {color_.r, color_.g, color_.b, static_cast<uint8_t> (256 * alpha_f)});
      } // end FOR
    }

    float      p_;
    olc::Pixel color_;
    float      g_;
  };
   
  class friend_
  {
   public:
    friend_ (float x_in, float y_in, int32_t id_in)
     : position_ (x_in, y_in)
     , velocity_ (0.0f, 0.0f)
     , id_ (id_in)
     , numberOfConnections_ (0)
     , connections_ ()
     , color_ (olc::WHITE)
     , sands_ ()
     , length_ (ENGINE_PGE_66_DEFAULT_MIN_LENGTH + Common_Tools::getRandomNumber (0.0f, 50.0f))
    {
      ACE_OS::memset (connections_, 0, sizeof (int32_t) * ENGINE_PGE_66_DEFAULT_MAX_CONNECTIONS);
      for (int i = 0; i < ENGINE_PGE_66_DEFAULT_MAX_PAINTERS; i++)
        sands_[i] = new sand_painter ();
    }

    ~friend_ ()
    {
      for (int i = 0; i < ENGINE_PGE_66_DEFAULT_MAX_PAINTERS; i++)
        delete sands_[i];
    }

    void update ()
    {
      position_ += velocity_;

      velocity_ *= ENGINE_PGE_66_DEFAULT_FRICTION;
    }

    void expose (olc::PixelGameEngine* engine_in)
    {
      for (int dx = -2; dx < 3; dx++)
      {
        float alpha_f = 0.5f - std::abs (dx) / 5.0f;
        engine_in->Draw (static_cast<int32_t> (position_.x + dx), static_cast<int32_t> (position_.y), {0, 0, 0, static_cast<uint8_t> (256 * alpha_f)});
        engine_in->Draw (static_cast<int32_t> (position_.x + dx - 1), static_cast<int32_t> (position_.y - 1), {255, 255, 255, static_cast<uint8_t> (256 * alpha_f)});
      } // end FOR

      for (int dy = -2; dy < 3; dy++)
      {
        float alpha_f = 0.5f - std::abs (dy) / 5.0f;
        engine_in->Draw (static_cast<int32_t> (position_.x), static_cast<int32_t> (position_.y + dy), {0, 0, 0, static_cast<uint8_t> (256 * alpha_f)});
        engine_in->Draw (static_cast<int32_t> (position_.x - 1), static_cast<int32_t> (position_.y + dy - 1), {255, 255, 255, static_cast<uint8_t> (256 * alpha_f)});
      } // end FOR
    }

    void exposeConnections (const std::vector<friend_*>& friends_in,
                            olc::PixelGameEngine* engine_in)
    {
      for (int i = 0; i < numberOfConnections_; i++)
      {
        float ox = friends_in[connections_[i]]->position_.x;
        float oy = friends_in[connections_[i]]->position_.y;

        for (int j = 0; j < ENGINE_PGE_66_DEFAULT_MAX_PAINTERS; j++)
          sands_[j]->render (position_.x, position_.y, ox, oy, engine_in);
      } // end FOR
    }

    bool friendOf (int x)
    {
      for (int i = 0; i < numberOfConnections_; i++)
        if (connections_[i] == x) return true;
      return false;
    }

    void connectTo (int32_t f)
    {
      if (numberOfConnections_ < ENGINE_PGE_66_DEFAULT_MAX_CONNECTIONS)
        if (!friendOf (f))
        {
          connections_[numberOfConnections_] = f;
          numberOfConnections_++;
        } // end IF
    }

    void findHappyPlace (const std::vector<friend_*>& friends_in)
    {
      float ax = 0.0f;
      float ay = 0.0f;

      for (int i = 0; i < ENGINE_PGE_66_DEFAULT_NUMBER_OF_FRIENDS; i++)
        if (friends_in[i] != this)
        {
          float ddx = friends_in[i]->position_.x - position_.x;
          float ddy = friends_in[i]->position_.y - position_.y;
          float d = std::sqrt (ddx * ddx + ddy * ddy);
          float t = std::atan2 (ddy, ddx);

          if (friendOf (i))
          {
            if (d > length_)
            {
              ax += 4.0f * std::cos (t);
              ay += 4.0f * std::sin (t);
            } // end IF
          } // end IF
          else
          {
            if (d < length_)
            {
              ax += (length_ - d) * std::cos (t + static_cast<float> (M_PI));
              ay += (length_ - d) * std::sin (t + static_cast<float> (M_PI));
            } // end IF
          } // end ELSE
        } // end IF

      velocity_.x += ax / 42.22f;
      velocity_.y += ay / 42.22f;
    }

    olc::vf2d     position_;
    olc::vf2d     velocity_;
    int32_t       id_;
    int32_t       numberOfConnections_;
    int32_t       connections_[ENGINE_PGE_66_DEFAULT_MAX_CONNECTIONS];
    olc::Pixel    color_;
    sand_painter* sands_[ENGINE_PGE_66_DEFAULT_MAX_PAINTERS];
    float         length_;
  };

  std::vector<friend_*> friends_;

  void reset ();
};

#endif // PGE_67_H
