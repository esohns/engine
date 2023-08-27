#include "stdafx.h"

#include "pge_150.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_150::PGE_150 ()
 : olc::PixelGameEngine ()
 , c_ (0.0f)
 , h_ (ENGINE_PGE_150_DEFAULT_W / 2.0f)
 , fish_ (ENGINE_PGE_150_DEFAULT_W, {h_, h_})
 , noise_ ()
{
  sAppName = "Example 150";
}

bool
PGE_150::OnUserCreate ()
{
  //olc::PixelGameEngine::SetPixelBlend (0.8f);
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_150::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_150_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_150_DEFAULT_ALPHA_DECAY : 0);

  float f = std::sin (c_);

  for (int i = 0; i < static_cast<int> (fish_.size ()); i++)
  {
    fish_[i].x =
      static_cast<float> (static_cast<int32_t> (fish_[i].x + ENGINE_PGE_150_DEFAULT_W) % ENGINE_PGE_150_DEFAULT_W);
    int32_t X = static_cast<int32_t> (fish_[i].x);
    fish_[i].y =
      static_cast<float> (static_cast<int32_t> (fish_[i].y + ENGINE_PGE_150_DEFAULT_W) % ENGINE_PGE_150_DEFAULT_W);
    int32_t Y = static_cast<int32_t> (fish_[i].y);
    float n =
      //Common_GL_Tools::map (static_cast<float> (noise_.GetValue (fish_[i].x / static_cast<float> (ENGINE_PGE_150_DEFAULT_W - i),
      Common_GL_Tools::map (static_cast<float> (noise_.Evaluate (fish_[i].x / static_cast<float> (ENGINE_PGE_150_DEFAULT_W - i),
                                                                 fish_[i].y / static_cast<float> (ENGINE_PGE_150_DEFAULT_W),
                                                                 c_)), -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI);
    float d = fish_[i].dist ({h_, h_}) / f;
    fish_[i].x += std::sin (n) * 3.0f;
    fish_[i].y += std::cos (n) * 3.0f;
    olc::PixelGameEngine::DrawLine (X, Y,
                                    static_cast<int32_t> (fish_[i].x), static_cast<int32_t> (fish_[i].y),
                                    olc::WHITE, 0xFFFFFFFF);
    fish_[i].x += (fish_[i].x - h_) / d;
    fish_[i].y += (fish_[i].y - h_) / d;
  } // end FOR

  c_ += 0.03f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
