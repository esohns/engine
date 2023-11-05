#include "stdafx.h"

#include "pge_224.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_224::PGE_224 ()
 : olc::PixelGameEngine ()
 , columns_ (0)
 , rows_ (0)
 , particles_ ()
 , mouseForce_ (new force (0.0f, 0.0f, 0.0f))
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 224");
}

PGE_224::~PGE_224 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete* iterator;
  mouseForce_.reset ();
}

bool
PGE_224::OnUserCreate ()
{
  columns_ = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_224_DEFAULT_DIFF;
  rows_ = olc::PixelGameEngine::ScreenHeight () / ENGINE_PGE_224_DEFAULT_DIFF;

  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < columns_; j++)
    {
      particle* p = new particle (static_cast<float> (10 + j * ENGINE_PGE_224_DEFAULT_DIFF),
                                  static_cast<float> (10 + i * ENGINE_PGE_224_DEFAULT_DIFF));
      p->addForce (mouseForce_);

      std::shared_ptr<force> force_p (new force (static_cast<float> (10 + j * ENGINE_PGE_224_DEFAULT_DIFF),
                                                 static_cast<float> (10 + i * ENGINE_PGE_224_DEFAULT_DIFF),
                                                 -0.001f));
      force_p->type_ = force::LINEAR;
      p->addForce (force_p);
      particles_.push_back (p);
    } // end FOR

  return true;
}

bool
PGE_224::OnUserUpdate (float fElapsedTime)
{
  olc::HWButton mouse_s = olc::PixelGameEngine::GetMouse (0);
  if (mouse_s.bPressed)
  {
    mouseForce_->k_ =
      olc::PixelGameEngine::GetKey (olc::Key::CTRL).bHeld ? -500.0f : 500.0f;
    mouseForce_->position_.x = static_cast<float> (olc::PixelGameEngine::GetMouseX ());
    mouseForce_->position_.y = static_cast<float> (olc::PixelGameEngine::GetMouseY ());
  } // end IF
  if (mouse_s.bHeld)
  {
    mouseForce_->position_.x = static_cast<float> (olc::PixelGameEngine::GetMouseX ());
    mouseForce_->position_.y = static_cast<float> (olc::PixelGameEngine::GetMouseY ());
  } // end IF
  if (mouse_s.bReleased)
    mouseForce_->k_ = 0.0f;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_224_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_224_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator)->draw (this);
    (*iterator)->update ();
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
