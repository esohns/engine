#ifndef PGE_174_H
#define PGE_174_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_7.h"

class PGE_174
 : public olc::PixelGameEngine
{
 public:
  PGE_174 ();
  inline virtual ~PGE_174 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
    int32_t   r_;
    float     d_;

    particle (olc::PixelGameEngine* engine_in)
     : position_ (static_cast<float> (engine_in->GetMouseX ()),
                  static_cast<float> (engine_in->GetMouseY ()))
     , velocity_ (Common_Tools::getRandomNumber (-5.0f, 5.0f),
                  Common_Tools::getRandomNumber (-5.0f, 5.0f))
     , acceleration_ ()
     , r_ (ENGINE_PGE_174_DEFAULT_RADIUS)
     , d_ (40.0f)
    {}
  };

  std::vector<particle> particles_;

  void collideAll (std::vector<particle*>&);
  void collide (particle&, particle&);
};

#endif // PGE_174_H
