#include "stdafx.h"

#include "pge_125.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_125::PGE_125 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , noise_ ()
 , c_ (0.0f)
 , d_ (0.0f)
 , h_ (0.0f)
{
  sAppName = "Example 125";
}

bool
PGE_125::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  h_ = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  d_ = h_ / 20.0f;

  return true;
}

bool
PGE_125::OnUserUpdate (float fElapsedTime)
{
  if (points_.size () < ENGINE_PGE_125_DEFAULT_WIDTH)//ENGINE_PGE_125_DEFAULT_WIDTH
    points_.push_back ({h_, h_});

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_125_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_125_DEFAULT_ALPHA_DECAY : 0);

  int i = 0;
  for (std::vector<olc::vf2d>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
  {
    olc::vf2d p ((*iterator).x - h_, (*iterator).y - h_);
    if (p.mag () < 33.0f)
    {
      (*iterator).x =
        Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_125_DEFAULT_WIDTH));
      (*iterator).y = 0.0f;
    } // end IF

    float n =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((*iterator).x / static_cast<float> (ENGINE_PGE_125_DEFAULT_WIDTH) - c_,
                                                                 (*iterator).y / static_cast<float> (ENGINE_PGE_125_DEFAULT_WIDTH) - static_cast<float> (i) * 0.01f,
                                                                 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * static_cast<float> (M_PI) * 2.0f;
    float dx = std::cos (n) * 9.0f - ((*iterator).x - h_) / d_;
    float dy = std::sin (n) * 9.0f - ((*iterator).y - h_) / d_;
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> ((*iterator).x),
                                    static_cast<int32_t> ((*iterator).y),
                                    static_cast<int32_t> ((*iterator).x + dx),
                                    static_cast<int32_t> ((*iterator).y + dy),
                                    olc::WHITE, 0xFFFFFFFF);
    //olc::PixelGameEngine::Draw (static_cast<int32_t> ((*iterator).x), static_cast<int32_t> ((*iterator).y), olc::WHITE);
    (*iterator).x += dx;
    (*iterator).y += dy;

    ++i;
  } // end FOR

  c_ += 0.01f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
