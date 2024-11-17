#include "stdafx.h"

#include "pge_420.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_19.h"

PGE_420::PGE_420 ()
 : olc::PixelGameEngine ()
 , cells_ ()
 , state0_ (0)
 , states_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 420");
}

bool
PGE_420::OnUserCreate ()
{
  state0_ = Common_Tools::getRandomNumber (0, 4);

  // transition matrix 
  std::vector<float> row_a = {0.8f, 0.05f, 0.05f, 0.05f, 0.05f};
  states_.push_back (row_a);
  row_a = {0.05f, 0.8f, 0.05f, 0.05f, 0.05f};
  states_.push_back (row_a);
  row_a = {0.05f, 0.05f, 0.8f, 0.05f, 0.05f};
  states_.push_back (row_a);
  row_a = {0.05f, 0.05f, 0.05f, 0.8f, 0.05f};
  states_.push_back (row_a);
  row_a = {0.05f, 0.05f, 0.05f, 0.05f, 0.8f};
  states_.push_back (row_a);

  float half_width_f = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  float half_height_f = olc::PixelGameEngine::ScreenHeight () / 2.0f;
  for (float rad = ENGINE_PGE_420_DEFAULT_OUTER_RADIUS; rad > ENGINE_PGE_420_DEFAULT_INNER_RADIUS; rad -= ENGINE_PGE_420_DEFAULT_THICKNESS * 1.1f)
    cells_.push_back (cell (half_width_f + rad, half_height_f, rad, ENGINE_PGE_420_DEFAULT_SPEED, state0_));

  return true;
}

bool
PGE_420::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  if (frame_count_i > 1000)
    goto done; // --> done

  for (std::vector<cell>::iterator iterator = cells_.begin ();
       iterator != cells_.end ();
       ++iterator)
    (*iterator).update (states_,
                        frame_count_i,
                        this);

done:
  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
