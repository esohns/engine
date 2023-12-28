#include "stdafx.h"

#include "pge_289.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_289::PGE_289 ()
 : olc::PixelGameEngine ()
 , agents_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 289");
}

bool
PGE_289::OnUserCreate ()
{

  return true;
}

bool
PGE_289::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  tryCreateAgent ();

  for (std::vector<PGE_289::agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
     (*iterator).update (agents_);

  std::vector<PGE_289::agent> agents_a;
  for (int i = static_cast<int> (agents_.size ()) - 1; i >= 0; i--)
    if (!agents_[i].isInScreen ())
    {
      std::vector<agent>::iterator iterator = agents_.begin ();
      std::advance (iterator, i);
      agents_.erase (iterator);
    } // end IF

  for (std::vector<PGE_289::agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
     (*iterator).show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_289::tryCreateAgent ()
{
  int32_t w = Common_Tools::getRandomNumber (ENGINE_PGE_289_DEFAULT_MIN_SIZE, ENGINE_PGE_289_DEFAULT_MAX_SIZE);
  int32_t h = Common_Tools::getRandomNumber (ENGINE_PGE_289_DEFAULT_MIN_SIZE, ENGINE_PGE_289_DEFAULT_MAX_SIZE);
  float direction = Common_Tools::testRandomProbability (0.5f) ? 0.0f : static_cast<float> (M_PI_2);
  int32_t x, y;
  if (direction == 0.0f)
  {
    x = -w / 2;
    y = Common_Tools::getRandomNumber (10 + h / 2, ENGINE_PGE_289_DEFAULT_HEIGHT - h / 2);
  } // end IF
  else
  {
    x = Common_Tools::getRandomNumber (10 + w / 2, ENGINE_PGE_289_DEFAULT_WIDTH - w / 2);
    y = -h / 2;
  } // end ELSE

  bool collide = isCollide (agents_, x, y, w, h);
  if (!collide) agents_.push_back (agent (x, y, w, h, direction));
}

bool
PGE_289::isCollide (std::vector<PGE_289::agent>& agents, int32_t x, int32_t y, int32_t w, int32_t h)
{
  for (std::vector<PGE_289::agent>::iterator iterator = agents.begin();
       iterator != agents.end();
       ++iterator)
    if (std::abs ((*iterator).position_.x - x) < ((*iterator).width_ + w) / 2 &&
        std::abs ((*iterator).position_.y - y) < ((*iterator).height_ + h) / 2)
      return true;

  return false;
}
