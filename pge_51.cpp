#include "stdafx.h"

#include "pge_51.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_51::PGE_51 ()
 : olc::PixelGameEngine ()
 , path_ ()
{
  sAppName = "Example 51";
}

bool
PGE_51::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  return true;
}

bool
PGE_51::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    olc::PixelGameEngine::Clear (olc::BLACK);

  path_ = circlePoints ();
  for (int i = 0; i < 6; i++)
    path_ = complexifyPath (path_);

  for (int i = 0; i < path_.size () - 1; i++)
    olc::PixelGameEngine::DrawLine (path_[i], path_[i + 1], {255, 255, 255, 15}, 0xFFFFFFFF);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

std::vector<olc::vf2d>
PGE_51::circlePoints ()
{
  std::vector<olc::vf2d> result;

  static float r = olc::PixelGameEngine::ScreenWidth () / 3.2f;

  //float theta1 = Common_Tools::getRandomNumber (0.0f, static_cast<float> (2.0 * M_PI));
  std::normal_distribution<float> distribution (0.0f, static_cast<float> (M_PI / 4.0));
  float theta1 = Common_Tools::getRandomNumber (distribution);
  std::normal_distribution<float> distribution_2 (0.0f, static_cast<float> (M_PI / 3.0));
  float theta2 = theta1 + Common_Tools::getRandomNumber (distribution_2);

  static float half_width_f = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  static float half_height_f = olc::PixelGameEngine::ScreenHeight () / 2.0f;
  olc::vf2d v1 = {half_width_f  + r * std::cos (theta1),
                  half_height_f + r * std::sin (theta1)};
  olc::vf2d v2 = {half_width_f  + r * std::cos (theta2),
                  half_height_f + r * std::sin (theta2)};
  result.push_back (v1);
  result.push_back (v2);

  return result;
}

std::vector<olc::vf2d>
PGE_51::complexifyPath (std::vector<olc::vf2d>& path_in)
{
  std::vector<olc::vf2d> result;

  for (int i = 0; i < path_in.size () - 1; i++)
  {
    olc::vf2d mid = path_in[i] + path_in[i + 1];
    mid *= 0.5f;
    float distance = path_in[i].dist (path_in[i + 1]);
    float standardDeviation = 0.125f * (distance ? distance : 0.0001f);
    std::normal_distribution<float> distribution (mid.x, standardDeviation);
    std::normal_distribution<float> distribution_2 (mid.y, standardDeviation);
    olc::vf2d v = {Common_Tools::getRandomNumber (distribution),
                   Common_Tools::getRandomNumber (distribution_2)};
    result.push_back (path_in[i]);
    result.push_back (v);
  } // end FOR
  result.push_back (path_in[path_in.size () - 1]);

  return result;
}
