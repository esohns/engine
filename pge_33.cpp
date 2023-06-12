#include "stdafx.h"

#include "pge_33.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines.h"
#include "engine_common.h"

PGE_33::PGE_33 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , quadTree_ (NULL)
{
  sAppName = "Example 33";
}

PGE_33::~PGE_33 ()
{
  for (particles_t::const_iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;

  delete quadTree_;
}

bool
PGE_33::OnUserCreate ()
{
  for (int i = 0; i < 1000; i++)
    particles_.push_back (new particle (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth ()),
                                        Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight ())));

  return true;
}

bool
PGE_33::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  delete quadTree_; quadTree_ = NULL;
  rectangle boundary (300, 200, 600, 400);
  quadTree_ = new quad_tree (boundary, 4);

  for (particles_t::iterator iterator = particles_.begin();
       iterator != particles_.end();
       ++iterator)
  {
    point point_s (static_cast<int> ((*iterator)->position_.x),
                   static_cast<int> ((*iterator)->position_.y),
                   (*iterator));
    quadTree_->insert (point_s);

    (*iterator)->move ();
    (*iterator)->render (this);
    (*iterator)->setHighlight (false);
  } // end FOR

  for (particles_t::iterator iterator = particles_.begin();
       iterator != particles_.end();
       ++iterator)
  {
    circle range (static_cast<int> ((*iterator)->position_.x),
                  static_cast<int> ((*iterator)->position_.y),
                  (*iterator)->radius_ * 2);
    quad_tree::points_t points_a;
    quadTree_->query (range, points_a);
    for (quad_tree::points_t::iterator iterator_2 = points_a.begin ();
         iterator_2 != points_a.end ();
         ++iterator_2)
    {
      particle* other_p = reinterpret_cast<particle*> ((*iterator_2).userData_);
      if ((*iterator) != other_p && (*iterator)->intersects (*other_p))
        (*iterator)->setHighlight (true);
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
