#include "stdafx.h"

#include "pge_387.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_387::PGE_387 ()
 : olc::PixelGameEngine ()
 , boids_ ()
 , quadtree_ (NULL)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 387");
}

PGE_387::~PGE_387 ()
{
  delete quadtree_;
}

bool
PGE_387::OnUserCreate ()
{
  rectangle boundary (0.0f, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1), static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
  quadtree_ = new quadtree (boundary, ENGINE_PGE_387_DEFAULT_CAPACITY);

  for (int i = 0; i < ENGINE_PGE_387_DEFAULT_NUMBER_OF_BOIDS; i++)
    boids_.push_back (boid (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                            Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));

  return true;
}

bool
PGE_387::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_387_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_387_DEFAULT_ALPHA_DECAY : 0);

  static bool draw_tree_b = false;
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
    draw_tree_b = !draw_tree_b;

  quadtree_->clear ();
  for (std::vector<boid>::iterator iterator = boids_.begin ();
       iterator != boids_.end ();
       ++iterator)
  {
    point p ((*iterator).position_.x, (*iterator).position_.y, &(*iterator));
    quadtree_->insert (p);
  } // end FOR

  for (std::vector<boid>::iterator iterator = boids_.begin ();
       iterator != boids_.end ();
       ++iterator)
  {
    circle range ((*iterator).position_.x, (*iterator).position_.y, ENGINE_PGE_387_DEFAULT_PERCEPTION_RADIUS);
    std::vector<boid*> neighbors;
    quadtree_->query (range, neighbors);

    (*iterator).flock (neighbors);
    (*iterator).update (this);
    (*iterator).draw (this);
  } // end FOR

  if (unlikely (draw_tree_b))
    quadtree_->draw (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
