#include "stdafx.h"

#include "pge_437.h"

#include "ace/Log_Msg.h"

#include "common_math_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_20.h"

PGE_437::PGE_437 ()
 : olc::PixelGameEngine ()
 , dots_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 437");
}

bool
PGE_437::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  int j0 = 0;
  for (int i = 0; i < ENGINE_PGE_437_DEFAULT_NSTEPS; i++)
  {
    int32_t r = ENGINE_PGE_437_DEFAULT_RMIN + i * ENGINE_PGE_437_DEFAULT_RSTEP;
    float d1 = 2.0f * ENGINE_PGE_437_DEFAULT_RSTEP;
    float d2 = 2.0f * r * std::sin (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_437_DEFAULT_NDIVS));
    float f1 = r - (r + ENGINE_PGE_437_DEFAULT_RSTEP) * std::cos (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_437_DEFAULT_NDIVS));
    float f2 =     (r - ENGINE_PGE_437_DEFAULT_RSTEP) * std::sin (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_437_DEFAULT_NDIVS));
    float d3 = std::sqrt ((f1 * f1) + (f2 * f2));
    float f3 = r - (r - ENGINE_PGE_437_DEFAULT_RSTEP) * std::cos (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_437_DEFAULT_NDIVS));
    float d4 = std::sqrt ((f3 * f3) + (f2 * f2));
    float dMin = 2.0f;
    float dMax = std::min (std::min (std::min (d1, d2), d3), d4) * 3.0f / 4.0f;
    for (int j = j0; j < ENGINE_PGE_437_DEFAULT_NDIVS; j += 2)
    {
      float theta = j * (2.0f * static_cast<float> (M_PI)) / static_cast<float> (ENGINE_PGE_437_DEFAULT_NDIVS);
      int32_t x = static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth ()  / 2.0f + r * std::cos (theta));
      int32_t y = static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / 2.0f + r * std::sin (theta));
      dots_.push_back ({x, y, r, theta, dMin, dMax});
    } // end FOR
    j0 = 1 - j0;
  } // end FOR

  return true;
}

bool
PGE_437::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_437_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_437_DEFAULT_ALPHA_DECAY : 0);
  //olc::PixelGameEngine::Clear (olc::BLACK);

  static int frame_count_i = 1;

  float animBlock =
    (ENGINE_PGE_437_DEFAULT_N_ANIM - ENGINE_PGE_437_DEFAULT_N_TRANSITION) / (static_cast<int> (dots_.size ()) - 1);
  for (int i = 0; i < static_cast<int> (dots_.size ()); i++)
  {
    float t;
    int frameInCycle = frame_count_i % (ENGINE_PGE_437_DEFAULT_N_FRAMES);
    if (frameInCycle < ENGINE_PGE_437_DEFAULT_N_WAIT)
      t = 0.0f;
    else if (frameInCycle < ENGINE_PGE_437_DEFAULT_N_WAIT + ENGINE_PGE_437_DEFAULT_N_ANIM)
    {
      float startTransition = i * animBlock;
      float endTransition = startTransition + ENGINE_PGE_437_DEFAULT_N_TRANSITION;
      float f = static_cast<float> (frameInCycle - ENGINE_PGE_437_DEFAULT_N_WAIT);
      if (f < startTransition)
        t = 0.0f;
      else if (f < endTransition)
        t = ((f - startTransition) / static_cast<float> (ENGINE_PGE_437_DEFAULT_N_TRANSITION));
      else
        t = 1.0f;
    } // end ELSE IF
    else if (frameInCycle < (2 * ENGINE_PGE_437_DEFAULT_N_WAIT + ENGINE_PGE_437_DEFAULT_N_ANIM))
      t = 1.0f;
    else
    {
      float startTransition = (static_cast<int> (dots_.size ()) - i - 1) * animBlock;
      float endTransition = startTransition + ENGINE_PGE_437_DEFAULT_N_TRANSITION;
      float f = frameInCycle - 2.0f * ENGINE_PGE_437_DEFAULT_N_WAIT - ENGINE_PGE_437_DEFAULT_N_ANIM;
      if (f < startTransition)
        t = 1.0f;
      else if (f < endTransition)
        t = 1.0f - ((f - startTransition) / static_cast<float> (ENGINE_PGE_437_DEFAULT_N_TRANSITION));
      else
        t = 0.0f;
    } // end ELSE

    struct dot& dot_r = dots_[i];
    float tPattern =
      (frame_count_i % static_cast<int> (ENGINE_PGE_437_DEFAULT_N_FRAMES_PATTERN)) / static_cast<float> (ENGINE_PGE_437_DEFAULT_N_FRAMES_PATTERN);
    float val1 =
      (std::sin (2.0f * static_cast<float> (M_PI) * tPattern - (dot_r.r / 18.0f) - (2.0f * dot_r.theta)) + 1.0f) / 2.0f;
    float val2 =
      (std::sin (2.0f * static_cast<float> (M_PI) * tPattern + (dot_r.r / 18.0f) - (2.0f * dot_r.theta)) + 1.0f) / 2.0f;
    float val = Common_Math_Tools::lerp (val1, val2, t);
    float f1 =
      2.0f * (dot_r.r - ENGINE_PGE_437_DEFAULT_RMIN) / static_cast<float> (ENGINE_PGE_437_DEFAULT_RSTEP * ENGINE_PGE_437_DEFAULT_NSTEPS) - 1.0f;
    val *= 1.0f - (f1 * f1); // make it smaller when closer to the borders
    float d = Common_GL_Tools::map (val, 0.0f, 1.0f, dot_r.dMin, dot_r.dMax);
    olc::PixelGameEngine::FillCircle (dot_r.x, dot_r.y, static_cast<int32_t> (d / 2.0f), olc::Pixel (250, 250, 250, 255));
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
