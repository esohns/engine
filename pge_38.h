#ifndef PGE_38_H
#define PGE_38_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_2.h"

class PGE_38
 : public olc::PixelGameEngine
{
 public:
  PGE_38 ();
  virtual ~PGE_38 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct barrier
  {
    int32_t rx;
    int32_t ry;
    int32_t rw;
    int32_t rh;
  };
   
  typedef std::vector<olc::vf2d> genes_t;
  class dna
  {
   public:
    dna (genes_t* genes_in)
     : genes_ ()
    {
      if (genes_in)
        genes_ = *genes_in;
      else
      {
        for (int i = 0; i < ENGINE_PGE_38_DEFAULT_LIFESPAN; ++i)
        {
          olc::vf2d gene = {Common_Tools::getRandomNumber (-1.0f, 1.0f),
                            Common_Tools::getRandomNumber (-1.0f, 1.0f)};
          gene = gene.norm ();
          gene *= ENGINE_PGE_38_DEFAULT_MAXFORCE;
          genes_.push_back (gene);
        } // end FOR
      } // end ELSE
    }
    inline ~dna () {}

    genes_t crossover (const dna& partner_in)
    {
      genes_t genes_a;
      float mid_f =
        Common_Tools::getRandomNumber (0.0f, static_cast<float> (genes_.size () - 1));
      for (int i = 0; i < genes_.size (); i++)
      {
        if (static_cast<float> (i) > mid_f)
          genes_a.push_back (genes_[i]);
        else
          genes_a.push_back (partner_in.genes_[i]);
      } // end FOR

      return genes_a;
    }

    void mutation ()
    {
      for (int i = 0; i < genes_.size (); i++)
        if (Common_Tools::testRandomProbability (ENGINE_PGE_38_DEFAULT_MUTATION_RATE))
        {
          olc::vf2d gene = {Common_Tools::getRandomNumber (-1.0f, 1.0f),
                            Common_Tools::getRandomNumber (-1.0f, 1.0f)};
          gene = gene.norm ();
          gene *= ENGINE_PGE_38_DEFAULT_MAXFORCE;
          genes_[i] = gene;
        } // end IF
    }

   public:
    genes_t genes_;
  };

  class rocket
  {
   public:
    rocket (olc::PixelGameEngine* engine_in,
            dna* dna_in)
     : position_ (engine_in->ScreenWidth () / 2, engine_in->ScreenHeight () - 1)
     , acceleration_ ({0.0f, 0.0f})
     , velocity_ ({0.0f, 0.0f})
     , dna_ (dna_in)
     , completed_ (false)
     , crashed_ (false)
     , fitness_ (0.0f)
    {
      if (!dna_in)
      {
        dna_ = new dna (NULL);
      } // end IF
    }
    ~rocket ()
    {
      delete dna_;
    }

    void applyForce (const olc::vf2d& force_in)
    {
      acceleration_ += force_in;
    }

    void calculateFitness (olc::PixelGameEngine* engine_in,
                           const olc::vi2d& target_in)
    {
      olc::vf2d position_s = {static_cast<float> (position_.x), static_cast<float> (position_.y)};
      float d =
        position_s.dist ({static_cast<float> (target_in.x), static_cast<float> (target_in.y)});
      fitness_ =
        Common_GL_Tools::map (d, 0.0f, static_cast<float> (engine_in->ScreenWidth ()), static_cast<float> (engine_in->ScreenWidth ()), 0.0f);
      if (completed_)
        fitness_ *= 10.0f;
      if (crashed_)
        fitness_ /= 10.0f;
    }

    void update (const olc::vi2d& target_in,
                 const barrier& barrier_in,
                 olc::PixelGameEngine* engine_in,
                 int32_t index_in)
    {
      olc::vf2d position_s = {static_cast<float> (position_.x), static_cast<float> (position_.y)};
      float d =
        position_s.dist ({static_cast<float> (target_in.x), static_cast<float> (target_in.y)});
      if (d < static_cast<float> (ENGINE_PGE_38_DEFAULT_TARGET_RADIUS * 2.0f))
        completed_ = true;

      if (position_.x > barrier_in.rx && position_.x < barrier_in.rx + barrier_in.rw &&
          position_.y > barrier_in.ry && position_.y < barrier_in.ry + barrier_in.rh)
        crashed_ = true;

      if (position_.x >= engine_in->ScreenWidth () || position_.x < 0)
        crashed_ = true;
      if (position_.y >= engine_in->ScreenHeight () || position_.y < 0)
        crashed_ = true;

      if (!completed_ && !crashed_)
      {
        applyForce (dna_->genes_[index_in]);
        velocity_ += acceleration_;
        if (velocity_.mag () > ENGINE_PGE_38_DEFAULT_MAXSPEED)
        {
          velocity_ = velocity_.norm ();
          velocity_ *= static_cast<float> (ENGINE_PGE_38_DEFAULT_MAXSPEED);
        } // end IF

        position_ += velocity_;

        acceleration_ = {0.0f, 0.0f};
      } // end IF
    }

    void display (olc::PixelGameEngine* engine_in)
    {
      static Common_GL_Color_t green = {0, 255, 0, 255};
      static Common_GL_Color_t red = {255, 0, 0, 255};
      Common_GL_Color_t lerped = Common_GL_Tools::lerpRGB (red, green, fitness_);

      olc::vf2d direction = velocity_;
      direction = direction.norm ();
      direction *= static_cast<float> (ENGINE_PGE_38_DEFAULT_ROCKET_RADIUS);
      olc::vf2d p1 = position_ + direction;
      olc::vf2d x = position_ + -direction;
      olc::vf2d dot_direction = direction.perp ();
      dot_direction = dot_direction.norm ();
      dot_direction *= static_cast<float> (ENGINE_PGE_38_DEFAULT_ROCKET_RADIUS);
      olc::vf2d p2 = x + dot_direction;
      olc::vf2d p3 = x + -dot_direction;
      //engine_in->DrawLine (p2, p1, { lerped.r, lerped.g, lerped.b, 255 }, 0xFFFFFFFF);
      //engine_in->DrawLine (p3, p1, { lerped.r, lerped.g, lerped.b, 255 }, 0xFFFFFFFF);
      //engine_in->DrawLine (p2, p3, { lerped.r, lerped.g, lerped.b, 255 }, 0xFFFFFFFF);
      engine_in->FillTriangle (p1, p2, p3, {lerped.r, lerped.g, lerped.b, 255});
      //engine_in->FillCircle (position_, ENGINE_PGE_38_DEFAULT_RADIUS, { lerped.r, lerped.g, lerped.b, 255 });
    }

   public:
    olc::vi2d position_;
    olc::vf2d acceleration_;
    olc::vf2d velocity_;
    dna*      dna_;
    bool      completed_;
    bool      crashed_;
    float     fitness_;
  };

  class population
  {
   public:
    population (olc::PixelGameEngine* engine_in)
     : rockets_ ()
     , matingpool_ ()
    {
      for (int i = 0; i < ENGINE_PGE_38_DEFAULT_ROCKETS; i++)
        rockets_.push_back (new rocket (engine_in, NULL));
    }
    ~population ()
    {
      for (rockets_t::iterator iterator = rockets_.begin ();
           iterator != rockets_.end ();
           ++iterator)
        delete *iterator;
    }

    void evaluate (olc::PixelGameEngine* engine_in,
                   const olc::vi2d& target_in)
    {
      matingpool_.clear ();
      for (int i = 0; i < ENGINE_PGE_38_DEFAULT_ROCKETS; i++)
      {
        int32_t n = static_cast<int32_t> (rockets_[i]->fitness_ * 100);
        for (int j = 0; j < n; j++)
          matingpool_.push_back (rockets_[i]);
      } // end FOR
    }

    void selection (olc::PixelGameEngine* engine_in)
    {
      rockets_t new_rockets_a;
      for (int i = 0; i < rockets_.size (); i++)
      {
        int32_t indexA = Common_Tools::getRandomNumber (0, static_cast<int32_t> (matingpool_.size () - 1));
        int32_t indexB = indexA;
        while (indexB == indexA)
          indexB = Common_Tools::getRandomNumber (0, static_cast<int32_t> (matingpool_.size () - 1));
        dna* parentA = matingpool_[indexA]->dna_;
        dna* parentB = matingpool_[indexB]->dna_;
        genes_t child = parentA->crossover (*parentB);
        dna* dna_p = new dna (&child);
        dna_p->mutation ();
        new_rockets_a.push_back (new rocket (engine_in, dna_p));
      } // end FOR

      for (rockets_t::iterator iterator = rockets_.begin ();
           iterator != rockets_.end ();
           ++iterator)
        delete *iterator;
      rockets_ = new_rockets_a;
    }

    void update (const olc::vf2d& target_in,
                 const barrier& barrier_in,
                 olc::PixelGameEngine* engine_in,
                 int32_t index_in)
    {
      float maxfitness_f = 0.0f;
      for (int i = 0; i < ENGINE_PGE_38_DEFAULT_ROCKETS; i++)
      {
        rockets_[i]->update (target_in,
                             barrier_in,
                             engine_in,
                             index_in);
        rockets_[i]->calculateFitness (engine_in,
                                       target_in);
        if (rockets_[i]->fitness_ > maxfitness_f)
          maxfitness_f = rockets_[i]->fitness_;
      } // end FOR
      for (int i = 0; i < ENGINE_PGE_38_DEFAULT_ROCKETS; i++)
        rockets_[i]->fitness_ /= maxfitness_f;
    }

    void display (olc::PixelGameEngine* engine_in)
    {
      for (int i = 0; i < ENGINE_PGE_38_DEFAULT_ROCKETS; i++)
        rockets_[i]->display (engine_in);
    }

   private:
    typedef std::vector<rocket*> rockets_t;
    rockets_t rockets_;
    rockets_t matingpool_;
  };

 private:
  barrier     barrier_;
  uint32_t    frameCounter_;
  population* population_;
  olc::vi2d   target_;
};

#endif // PGE_38_H
