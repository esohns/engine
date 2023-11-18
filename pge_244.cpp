#include "stdafx.h"

#include "pge_244.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_11.h"

PGE_244::PGE_244 ()
 : olc::PixelGameEngine ()
 , blocks_ ()
 , countdown_ (0)
 , cursors_ ()
 , drawCircle_ (true)
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 244");
}

PGE_244::~PGE_244 ()
{
  for (std::vector<block*>::iterator iterator = blocks_.begin ();
       iterator != blocks_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_244::OnUserCreate ()
{
  drawCircle_ = true;
  setup ();

  return true;
}

bool
PGE_244::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<block*>::iterator iterator = blocks_.begin ();
       iterator != blocks_.end ();
       ++iterator)
    (*iterator)->show (this);
  for (std::vector<cursor>::iterator iterator = cursors_.begin ();
       iterator != cursors_.end ();
       ++iterator)
  {
    (*iterator).update (&noise_);
    (*iterator).show (this);
  } // end FOR

  std::vector<cursor> alive_a;
  for (std::vector<cursor>::iterator iterator = cursors_.begin();
       iterator != cursors_.end();
       ++iterator)
    if (!(*iterator).done_)
      alive_a.push_back (*iterator);
  cursors_ = alive_a;

  if (cursors_.size () == 0)
  {
    countdown_--;
    if (countdown_ < 0)
    {
      drawCircle_ = !drawCircle_;
      setup ();
    } // end IF
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_244::setup ()
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  cursors_.clear ();
  for (std::vector<block*>::iterator iterator = blocks_.begin ();
       iterator != blocks_.end ();
       ++iterator)
    delete *iterator;
  blocks_.clear ();

  if (drawCircle_)
    circleSetup ();
  else
    rectSetup ();
  countdown_ = 100;
}

void
PGE_244::circleSetup ()
{
  float r = std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * 0.33f;
  float n = std::floor (r * 2.0f * static_cast<float> (M_PI) / 10.0f);
  for (int i = 0; i < n; i++)
  {
    float l = std::max (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * (1.0f + 0.1f * i);
    float a = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    block* newBlock = new block (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth ())),
                                 Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight ())),
                                 r * std::cos (i * 2.0f * static_cast<float> (M_PI) / n) + olc::PixelGameEngine::ScreenWidth () / 2.0f,
                                 r * std::sin (i * 2.0f * static_cast<float> (M_PI) / n) + olc::PixelGameEngine::ScreenHeight () / 2.0f,
                                 false);
    cursors_.push_back (cursor (l * std::cos (a), l * std::sin (a), newBlock));
    blocks_.push_back (newBlock);
  } // end FOR
}

void
PGE_244::rectSetup ()
{
  float s = std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * 0.33f;
  int n = static_cast<int> (std::floor (s / 10.0f));
  olc::vf2d v1 (olc::PixelGameEngine::ScreenWidth () / 2.0f - s / 2.0f,
                olc::PixelGameEngine::ScreenHeight () / 2.0f - s / 2.0f);
  olc::vf2d v2 (olc::PixelGameEngine::ScreenWidth () / 2.0f + s / 2.0f,
                olc::PixelGameEngine::ScreenHeight () / 2.0f - s / 2.0f);
  olc::vf2d v3 (olc::PixelGameEngine::ScreenWidth () / 2.0f + s / 2.0f,
                olc::PixelGameEngine::ScreenHeight () / 2.0f + s / 2.0f);
  olc::vf2d v4 (olc::PixelGameEngine::ScreenWidth () / 2.0f - s / 2.0f,
                olc::PixelGameEngine::ScreenHeight () / 2.0f + s / 2.0f);
  lineSetup (v1, v2, n, 0);
  lineSetup (v2, v3, n, n);
  lineSetup (v3, v4, n, n * 2);
  lineSetup (v4, v1, n, n * 3);
}

void
PGE_244::lineSetup (olc::vf2d& v1, olc::vf2d& v2, int n, int nOff)
{
  for (int i = 0; i < n; i++)
  {
    float l = std::max (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * (1.0f + 0.1f * (i + nOff));
    float a = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    olc::vf2d v = v1.lerp (v2, i / static_cast<float> (n));
    block* newBlock = new block (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth ())),
                                 Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight ())),
                                 v.x,
                                 v.y,
                                 true);
    cursors_.push_back (cursor (l * std::cos (a), l * std::sin (a), newBlock));
    blocks_.push_back (newBlock);
  } // end FOR
}
