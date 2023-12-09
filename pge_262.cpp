#include "stdafx.h"

#include "pge_262.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_262::PGE_262 ()
 : olc::PixelGameEngine ()
 , m_ (0)
 , maxStep_ (0)
 , cc_ ()
 , step_ (NULL)
 , limit_ (NULL)
 , nlimit_ (NULL)
 , q_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 262");
}

PGE_262::~PGE_262 ()
{
  delete [] step_;
  delete [] limit_;
  delete [] nlimit_;
}

bool
PGE_262::OnUserCreate ()
{
  m_ = ENGINE_PGE_262_DEFAULT_P * ENGINE_PGE_262_DEFAULT_P;
  cc_.reserve (m_);
  maxStep_ = ENGINE_PGE_262_DEFAULT_SIZE / 35;
  step_ = new int[ENGINE_PGE_262_DEFAULT_N * m_];
  limit_ = new int[ENGINE_PGE_262_DEFAULT_N * m_];
  nlimit_ = new int[m_];

  for (int k = 0; k < m_; k++)
    for (int i = 0; i < ENGINE_PGE_262_DEFAULT_N; i++)
      q_.push_back (quadrat (0, 0, 0, 0, false));

  for (int k = 0; k < m_; k++)
  {
    cc_.push_back (245);
    nlimit_[k] = ENGINE_PGE_262_DEFAULT_N;
    int w = ENGINE_PGE_262_DEFAULT_SIZE / ENGINE_PGE_262_DEFAULT_P;
    int a = Common_Tools::getRandomNumber (0, 8);
    bool r = false;
    if (Common_Tools::testRandomProbability (0.5f))
      r = true;
    for (int i = 0; i < ENGINE_PGE_262_DEFAULT_N; i++)
    {
      uint8_t c = 245 - 12 * i;
      q_[i * m_ + k] = quadrat (w, w, a, c, r);
      w = int(6 * w / 7);
      step_[i * m_ + k] = 0;
      limit_[i * m_ + k] = 3; //*(i+1);
    } // end FOR
  } // end FOR

  for (int k = 0; k < m_; k++)
  {
    int r = Common_Tools::getRandomNumber (0, 300);
    for (int i = 0; i < ENGINE_PGE_262_DEFAULT_N; i++)
      for (int l = 0; l < r; l++)
        q_[i * m_ + k].move (maxStep_);
  } // end FOR

  return true;
}

bool
PGE_262::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (int k = 0; k < m_; k++)
  {
    olc::vf2d pos (static_cast<float> ((olc::PixelGameEngine::ScreenWidth () - 2) / ENGINE_PGE_262_DEFAULT_P * (k % ENGINE_PGE_262_DEFAULT_P)),
                   static_cast<float> (((olc::PixelGameEngine::ScreenHeight () - 2) / ENGINE_PGE_262_DEFAULT_P) * (k / ENGINE_PGE_262_DEFAULT_P)));
    olc::Pixel color (cc_[k], cc_[k], cc_[k], 255);
    olc::PixelGameEngine::FillRect (static_cast<int32_t> (pos.x), static_cast<int32_t> (pos.y),
                                    olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_262_DEFAULT_P, olc::PixelGameEngine::ScreenWidth() / ENGINE_PGE_262_DEFAULT_P,
                                    color);

    for (int i = 0; i < nlimit_[k]; i++)
    {
      q_[i * m_ + k].display (pos, this);
      q_[i * m_ + k].move (maxStep_);
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
