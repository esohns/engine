#include "stdafx.h"

#include "pge_64.h"

#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_64::PGE_64 ()
 : olc::PixelGameEngine ()
 , ripples_ ()
{
  sAppName = "Example 64";
}

PGE_64::~PGE_64 ()
{
  for (std::vector<ripple*>::iterator iterator = ripples_.begin();
       iterator != ripples_.end();
       ++iterator)
    delete *iterator;
}

bool
PGE_64::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_64::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear ({0,0,0, ENGINE_PGE_64_DEFAULT_ALPHA_DECAY});
  int pixels = GetDrawTargetWidth() * GetDrawTargetHeight();
  olc::Pixel* p = GetDrawTarget()->GetData();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_64_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_64_DEFAULT_ALPHA_DECAY : 0);

  static int frame_count_i = 1;
  static bool init_b = true;
  if (frame_count_i % 100 == 0 || init_b)
  {
    ripples_.push_back (new ripple (this));
    init_b = false;
  } // end IF

  std::vector<ripple*> ripples_a;
  for (int i = static_cast<int> (ripples_.size ()) - 1; i >= 0; --i)
  {
    ripple* ripple_p = ripples_[i];
    ripple_p->add (frame_count_i);
    ripple_p->update ();
    ripple_p->show (this);

    if (ripple_p->lifetime_)
      ripples_a.push_back (ripple_p);
    else
      delete ripple_p;
  } // end FOR
  ripples_ = ripples_a;

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
