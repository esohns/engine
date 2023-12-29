#include "stdafx.h"

#include "pge_290.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_290::PGE_290 ()
 : olc::PixelGameEngine ()
 , x_ ()
 , y_ ()
 , vx_ ()
 , vy_ ()
 , minDistSq_ (0.0f)
 , maxDistSq_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 290");
}

bool
PGE_290::OnUserCreate ()
{
  for (int i = ENGINE_PGE_290_DEFAULT_NUM; i--; )
  {
    float a = (static_cast<float> (M_PI) / 180.0f) * (i / 2.0f);
    vx_[i] = std::cos (a);
    vy_[i] = std::sin (a);
    x_[i] = ENGINE_PGE_290_DEFAULT_WIDTH / 2.0f + (vx_[i] * ENGINE_PGE_290_DEFAULT_WIDTH) / 2.0f;
    y_[i] = ENGINE_PGE_290_DEFAULT_HEIGHT / 2.0f + (vy_[i] * ENGINE_PGE_290_DEFAULT_HEIGHT) / 2.0f;
  } // end FOR

  minDistSq_ = static_cast<float> (std::pow (16, 2));
  maxDistSq_ = static_cast<float> (std::pow (20, 2));

  return true;
}

bool
PGE_290::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = ENGINE_PGE_290_DEFAULT_NUM; i--; )
    for (int j = i; j--; )
    {
      float d = (x_[i] - x_[j]) * (x_[i] - x_[j]) + (y_[i] - y_[j]) * (y_[i] - y_[j]);
      if (minDistSq_ < d && d < maxDistSq_)
        olc::PixelGameEngine::DrawLine (static_cast<int32_t> (x_[i]), static_cast<int32_t> (y_[i]), static_cast<int32_t> (x_[j]), static_cast<int32_t> (y_[j]),
                                        olc::WHITE, 0xFFFFFFFF);
    } // end FOR

  for (int i = ENGINE_PGE_290_DEFAULT_NUM; i--; )
  {
    x_[i] += vx_[i];
    y_[i] += vy_[i];
    if (x_[i] < 0.0f || x_[i] > static_cast<float> (ENGINE_PGE_290_DEFAULT_WIDTH - 1)) vx_[i] *= -1.0f;
    if (y_[i] < 0.0f || y_[i] > static_cast<float> (ENGINE_PGE_290_DEFAULT_HEIGHT - 1)) vy_[i] *= -1.0f;
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
