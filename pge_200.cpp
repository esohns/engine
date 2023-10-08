#include "stdafx.h"

#include "pge_200.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_200::PGE_200 ()
 : olc::PixelGameEngine ()
 , objects_ ()
 , count_ (0)
 , noise_ ()
 , noiseInit_ ()
 , noiseSpeed_ (0.0f)
 , objectNum_ (1)
 , RShift_ ()
 , topNum_ ()
 , freqCount_ (1)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 200");
}

bool
PGE_200::OnUserCreate ()
{
  for (int i = 0; i < objectNum_; i++)
  {
    topNum_.push_back (Common_Tools::getRandomNumber (3, 12));
    noiseInit_.push_back (Common_Tools::getRandomNumber (0.0, 100.0));
    RShift_.push_back (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () / 2)));
  } // end FOR
  noiseSpeed_ = Common_Tools::getRandomNumber (10.0f, 100.0f) + 1.0f;

  return true;
}

bool
PGE_200::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    count_ = 0;
    objectNum_ = 1;

    topNum_.clear ();
    noiseInit_.clear ();
    RShift_.clear ();
    OnUserCreate ();

    objects_.clear ();
  } // end IF

  olc::PixelGameEngine::Clear (olc::BLACK);

  if (count_ % freqCount_ == 0)
    for (int i = 0; i < objectNum_; i++)
    {
      objects_.insert (objects_.begin (), 
                       object ( olc::PixelGameEngine::ScreenWidth () / 2.0f * (1.0f + Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseInit_[i] + frame_count_i / 100.0, 0.0, 0.0)),
                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f) * 1.8f - 0.9f),
                               olc::PixelGameEngine::ScreenHeight () / 2.0f * (1.0f + Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseInit_[i] + 10.0f + frame_count_i / 100.0, 0.0, 0.0)),
                                                                                                            -1.0f, 1.0f, 0.0f, 1.0f) * 1.8f - 0.9f),
                               5.0f * static_cast<float> (M_PI) * Common_GL_Tools::map (static_cast<float> (noise_.GetValue (noiseInit_[i] + frame_count_i / 120.0, 0.0, 0.0)),
                                                                                        -1.0f, 1.0f, 0.0f, 1.0f),
                               noiseInit_[i] + count_ / noiseSpeed_,
                               RShift_[i],
                               topNum_[i]));
    } // end FOR

  for (size_t i = 0; i < objects_.size (); i++)
  {
    objects_[i].update (this,
                        &noise_);
    if (objects_[i].rAng_ > static_cast<float> (M_PI) / 2.0f)
      objects_.pop_back ();
  } // end FOR

  if (objects_.size () >= 2)
  {
    for (size_t i = 0; i < objects_.size () - 1; i++)
      for (int j = 0; j < objects_[i].topNum_; j++)
      {
        int next_j = j + 1;
        if (next_j == objects_[i].topNum_)
          next_j = next_j - objects_[i].topNum_;

        olc::PixelGameEngine::DrawLine (static_cast<int32_t> (objects_[i].xy_[j].x), static_cast<int32_t> (objects_[i].xy_[j].y),
                                        static_cast<int32_t> (objects_[i + 1].xy_[j].x), static_cast<int32_t> (objects_[i + 1].xy_[j].y),
                                        olc::WHITE, 0xFFFFFFFF);
        olc::PixelGameEngine::DrawLine (static_cast<int32_t> (objects_[i + 1].xy_[j].x), static_cast<int32_t> (objects_[i + 1].xy_[j].y),
                                        static_cast<int32_t> (objects_[i + 1].xy_[next_j].x), static_cast<int32_t> (objects_[i + 1].xy_[next_j].y),
                                        olc::WHITE, 0xFFFFFFFF);
        olc::PixelGameEngine::DrawLine (static_cast<int32_t> (objects_[i + 1].xy_[next_j].x), static_cast<int32_t> (objects_[i + 1].xy_[next_j].y),
                                        static_cast<int32_t> (objects_[i].xy_[next_j].x), static_cast<int32_t> (objects_[i].xy_[next_j].y),
                                        olc::WHITE, 0xFFFFFFFF);
        olc::PixelGameEngine::DrawLine (static_cast<int32_t> (objects_[i].xy_[next_j].x), static_cast<int32_t> (objects_[i].xy_[next_j].y),
                                        static_cast<int32_t> (objects_[i].xy_[j].x), static_cast<int32_t> (objects_[i].xy_[j].y),
                                        olc::WHITE, 0xFFFFFFFF);
      } // end FOR
  } // end IF

  ++count_;
  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
