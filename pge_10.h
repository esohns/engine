#ifndef PGE_10_H
#define PGE_10_H

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_10
 : public olc::PixelGameEngine
{
 public:
  PGE_10 ();
  ~PGE_10 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct Renderable
  {
    void Load(const std::string& sFile)
    {
      sprite = new olc::Sprite(sFile);
      decal = new olc::Decal(sprite);
    }

    ~Renderable()
    {
      delete decal;
      delete sprite;
    }

    olc::Sprite* sprite = nullptr;
    olc::Decal* decal = nullptr;
  };

  struct vec2d
  {
    float x, y;

    void normalize()
    {
      float mag = sqrt((x * x) + (y * y));
      x = x / mag;
      y = y / mag;
    }
  };

   static int randBetween (int min, int max)
   {
     int range = max - min;
     return (rand() % range) + min;
   }

   static float randBetween (float min, float max)
   {
     float range = max - min;
     float value = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
     return (value * range) + min;
   }

   class Particle
   {
    public:
     void Update (olc::PixelGameEngine* pge, float dElapsedTime, vec2d emitterDelta, bool moveWithEmitter)
     {
       if (position.x > pge->ScreenWidth()
           || position.x < 0
           || position.y > pge->ScreenHeight()
           || position.y < 0
           || current_lifetime > max_lifetime)
       {
         alive = false;
       } // end IF

      if (alive)
      {
        if (moveWithEmitter)
        {
          position.x = position.x + emitterDelta.x;
          position.y = position.y + emitterDelta.y;
        } // end IF

        current_lifetime += dElapsedTime;

        velocity.x = velocity.x + (acceleration.x * dElapsedTime);
        velocity.y = velocity.y + (acceleration.y * dElapsedTime);

        position.x = position.x + (velocity.x * dElapsedTime);
        position.y = position.y + (velocity.y * dElapsedTime);

        float normalizedLifetime = current_lifetime / max_lifetime;

        //int alpha = int((4 * normalizedLifetime * (1 - normalizedLifetime)) * 256);
        //int alpha = int((1 - (4 * normalizedLifetime * (1 - normalizedLifetime))) * 256);
        int alpha = int((1 - normalizedLifetime) * 256);
        
        color.a = alpha;
        color.r = int(alpha * (0.80));
        color.g = int(alpha * (0.25));
        color.b = alpha;

        scale = (0.75f + (.25f * normalizedLifetime));
        //pge->DrawDecal({ position.x, position.y }, sprite->decal, { scale, scale }, color);
      } // end IF
    }

    void Draw (olc::PixelGameEngine* pge)
    {
      if (alive)
        pge->DrawDecal ({ position.x, position.y }, sprite->decal, { scale, scale }, color);
    }

    Renderable* sprite;
    olc::Decal* decal = NULL;
    olc::Pixel color;
    float scale = 1.0f; // 1.0 = 100% // 2.0 = 200%

    // Control the movement of the particle
    vec2d velocity;
    vec2d acceleration;
    vec2d position;

    // Control how long the particle lives
    float max_lifetime = 1.0f;
    float current_lifetime = 0.0f;
    bool alive = false;
   };

    class ParticleEngine
    {
     public:
      void Load ()
      {
        darkCircle.Load ("./bluedark.png");
      }

      void Update (olc::PixelGameEngine* pge, float dElapsedTime)
      {
        GenerateParticles (pge);

        for (int i = static_cast<int> (particles.size ()) - 1;
             i >= 0;
             i--)
        {
          if (particles[i].alive)
          {
            particles[i].Update(pge, dElapsedTime, emitterDelta, particlesMoveWithEmitter);
          } // end IF
          else
          {
            particles.erase (particles.begin () + i);
          } // end ELSE
        } // end FOR
      }

      void Draw (olc::PixelGameEngine* pge)
      {
        for (Particle& particle : particles)
          particle.Draw (pge);
      }

      void GenerateParticle (olc::PixelGameEngine* pge)
      {
        float nextFloat = static_cast <float> (rand ()) / static_cast <float> (RAND_MAX);
        float angle = nextFloat * float(twoPi);

        float x = cosf (angle);
        float y = sinf (angle);

        Particle p;
        int radius = 40;
        p.position.x = (x * radius) + emitterPosition.x;
        p.position.y = (y * radius) + emitterPosition.y;

        int dir = rand() % 2;
        if (dir == 0)
          dir = -1;
        p.velocity = { randBetween(10.0f, 30.0f) * x * dir, randBetween(10.0f, 30.0f) * y * dir };
        p.acceleration = { randBetween(30.0f, 50.0f) * x * dir, randBetween(30.0f, 50.0f) * y * dir };


        p.color = olc::Pixel(255, 255, 255, 255);
        p.alive = true;
        p.scale = randBetween(0.5f, 0.7f);


        p.sprite = &darkCircle;
        p.max_lifetime = randBetween(0.2f, 0.7f);


        particles.push_back(p);
      }

      void GenerateParticles(olc::PixelGameEngine* pge) {
        if (particles.size() < maxParticles)
        {
          size_t missing = maxParticles - particles.size();
          for (size_t i = 0; i < missing; i++)
            GenerateParticle(pge);
        }

      }

      void moveEmitter(vec2d newPosition)
      {
        emitterDelta.x = newPosition.x - emitterPosition.x;
        emitterDelta.y = newPosition.y - emitterPosition.y;

        emitterPosition = newPosition;
      }

    public:
     double twoPi = M_PI * 2;
     std::vector<Particle> particles;
     Renderable darkCircle;
     int maxParticles = 1200;

     vec2d emitterDelta;
     vec2d emitterPosition;
     bool particlesMoveWithEmitter = false;
   };

   ParticleEngine particleEngine;
   Renderable diamond;
   Renderable star;
   Renderable fire;
   Renderable smoke;
   Renderable circle;
   Renderable blue;
   Renderable* img[6];
  };

#endif // PGE_5_H
