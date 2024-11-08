#include "stdafx.h"

#include "pge_399.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

PGE_399::PGE_399 ()
 : olc::PixelGameEngine ()
 , nodes_ ()
 , links_ ()
 , speedLimit_ (0.0f)
{
  sAppName = "Example 399";
}

bool
PGE_399::OnUserCreate ()
{
  speedLimit_ = (olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_399_DEFAULT_GRID_COUNT) * 1.5f;
  createNodes ();
  createLinks ();

  return true;
}

bool
PGE_399::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    grabNodesNearMouse ();

  for (std::vector<struct link>::iterator iterator = links_.begin ();
       iterator != links_.end ();
       ++iterator)
    (*iterator).update ();
  for (std::vector<struct node>::iterator iterator = nodes_.begin ();
       iterator != nodes_.end ();
       ++iterator)
    (*iterator).update (speedLimit_);
  for (std::vector<struct link>::iterator iterator = links_.begin ();
       iterator != links_.end ();
       ++iterator)
    (*iterator).show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_399::createNodes ()
{
  nodes_.clear ();

  struct node node_s;
  node_s.velocity_ = {0.0f, 0.0f};
  node_s.force_ = {0.0f, 0.0f};
  for (int j = 0; j < ENGINE_PGE_399_DEFAULT_GRID_COUNT; j++)
    for (int i = 0; i < ENGINE_PGE_399_DEFAULT_GRID_COUNT; i++)
    {
      node_s.position_.x =
        Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_399_DEFAULT_GRID_COUNT - 1), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
      node_s.position_.y =
        Common_GL_Tools::map (static_cast<float> (j), 0.0f, static_cast<float> (ENGINE_PGE_399_DEFAULT_GRID_COUNT - 1), 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1));
      node_s.pinned_ =
        i == 0 || j == 0 || i == ENGINE_PGE_399_DEFAULT_GRID_COUNT - 1 || j == ENGINE_PGE_399_DEFAULT_GRID_COUNT - 1;
      
      nodes_.push_back (node_s);
    } // end FOR
}

void
PGE_399::createLinks ()
{
  links_.clear ();

  struct link link_s;
  for (std::vector<struct node>::iterator iterator = nodes_.begin ();
       iterator != nodes_.end ();
       ++iterator)
  {
    std::vector<struct node*> rest_a;
    for (std::vector<struct node>::iterator iterator_2 = iterator + 1;
         iterator_2 != nodes_.end ();
         ++iterator_2)
      rest_a.push_back (&(*iterator_2));

    std::vector<struct node*> neighbors_a;
    static float max_distance_f =
      olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_399_DEFAULT_GRID_COUNT - 1);
    for (std::vector<struct node*>::iterator iterator_2 = rest_a.begin ();
         iterator_2 != rest_a.end ();
         ++iterator_2)
      if ((*iterator).position_.dist ((*iterator_2)->position_) <= max_distance_f)
        neighbors_a.push_back (*iterator_2);

    for (std::vector<struct node*>::iterator iterator_2 = neighbors_a.begin ();
         iterator_2 != neighbors_a.end ();
         ++iterator_2)
      if (!(*iterator).pinned_ || !(*iterator_2)->pinned_)
      {
        link_s.p1_ = &(*iterator);
        link_s.p2_ = (*iterator_2);
        links_.push_back (link_s);
      } // end IF
  } // end FOR
}

void
PGE_399::grabNodesNearMouse ()
{
  olc::vf2d mouse_position_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                              static_cast<float> (olc::PixelGameEngine::GetMouseY ()));

  std::vector<struct node*> neighbors_a;
  static float max_distance_f =
    olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_399_DEFAULT_GRID_COUNT - 1);
  for (std::vector<struct node>::iterator iterator = nodes_.begin ();
        iterator != nodes_.end ();
        ++iterator)
    if (!(*iterator).pinned_ && (*iterator).position_.dist (mouse_position_s) <= ENGINE_PGE_399_DEFAULT_MOUSE_RADIUS)
      neighbors_a.push_back (&(*iterator));

  for (std::vector<struct node*>::iterator iterator = neighbors_a.begin ();
       iterator != neighbors_a.end ();
       ++iterator)
    (*iterator)->position_ = mouse_position_s;
}
