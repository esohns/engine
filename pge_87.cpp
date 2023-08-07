#include "stdafx.h"

#include "pge_87.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_87::PGE_87 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , lines_ ()
{
  sAppName = "Example 87";
}

bool
PGE_87::OnUserCreate ()
{
  point point_s;
  for (int i = 32; i--;)
  {
    point_s.position_.x =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, 31.0f, 50.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 50));
    for (int j = 32; j--;)
    {
      point_s.position_.y =
        Common_GL_Tools::map (static_cast<float> (j), 0.0f, 31.0f, 25.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 150));
      if (i & 31 - j)
      {
        point_s.prev_ = point_s.position_;
        point_s.pinned_ = j == 0;
        points_.push_back (point_s);
      } // end IF
    } // end FOR
  } // end FOR

  for (size_t j = points_.size (); j--; )
    for (size_t i = j; i--; )
    {
      float d = points_[j].position_.dist (points_[i].position_);
      if (0.0f < d && d < 45.0f)
      {
        line line_s;
        line_s.p1_ = &points_[j];
        line_s.p2_ = &points_[i];
        line_s.length_ = d;
        lines_.push_back (line_s);
      } // end IF
    } // end FOR

  return true;
}

bool
PGE_87::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  updatePoints ();
  if (olc::PixelGameEngine::GetMouse (0).bHeld) cutLines ();
  updateLines ();
  render ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_87::updatePoints ()
{
  for (std::vector<point>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
  {
    if ((*iterator).pinned_) continue;

    olc::vf2d v = (*iterator).position_ - (*iterator).prev_;
    v *= ENGINE_PGE_87_DEFAULT_FRICTION;
    (*iterator).prev_ = (*iterator).position_;
    v.y += ENGINE_PGE_87_DEFAULT_GRAVITY;
    (*iterator).position_ += v;

    if ((*iterator).position_.x < 0)
    {
      (*iterator).position_.x = 0;
      (*iterator).prev_.x = (*iterator).position_.x + v.x * ENGINE_PGE_87_DEFAULT_BOUNCE;
    } // end IF
    else if ((*iterator).position_.x > static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1))
    {
      (*iterator).position_.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1);
      (*iterator).prev_.x = (*iterator).position_.x + v.x * ENGINE_PGE_87_DEFAULT_BOUNCE;
    } // end ELSE IF
    if ((*iterator).position_.y > static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 25))
    {
      (*iterator).position_.y = static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 25);
      (*iterator).prev_.y = (*iterator).position_.y + v.y * ENGINE_PGE_87_DEFAULT_BOUNCE;
    } // end IF
  } // end FOR
}

void
PGE_87::cutLines ()
{
  olc::vf2d mv (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  lines_.erase (std::remove_if (lines_.begin (), lines_.end (),
                                [mv] (line x) { olc::vf2d v = x.p1_->position_ + x.p2_->position_;
                                                v /= 2.0f;
                                                float d = v.dist (mv);
                                                return d < 15.0f; }),
                lines_.end ());
}

void
PGE_87::updateLines ()
{
  for (int j = 2; j--;)
  {
    for (std::vector<line>::iterator iterator = lines_.begin();
         iterator != lines_.end();
         ++iterator)
    {
      float d = (*iterator).p1_->position_.dist ((*iterator).p2_->position_);
      float percent = ((*iterator).length_ - d) / d / 2.0f;
      olc::vf2d offset = (*iterator).p2_->position_ - (*iterator).p1_->position_;
      offset *= percent;
      if (!(*iterator).p1_->pinned_) (*iterator).p1_->position_ -= offset;
      if (!(*iterator).p2_->pinned_) (*iterator).p2_->position_ += offset;
    } // end FOR
    //std::reverse (lines_.begin (), lines_.end ());
  } // end FOR
}

void
PGE_87::render ()
{
  for (std::vector<point>::iterator iterator = points_.begin();
       iterator != points_.end();
       ++iterator)
    olc::PixelGameEngine::FillRect ((*iterator).position_, {3, 3}, olc::BLACK);
  for (std::vector<line>::iterator iterator = lines_.begin();
       iterator != lines_.end();
       ++iterator)
    olc::PixelGameEngine::DrawLine ((*iterator).p1_->position_, (*iterator).p2_->position_, olc::BLACK, 0xFFFFFFFF);
}
