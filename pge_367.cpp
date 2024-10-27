#include "stdafx.h"

#include "pge_367.h"

#include <numeric>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_367::PGE_367 ()
 : olc::PixelGameEngine ()
 , active_ ()
 , w_ (0.0f)
 , columns_ (0)
 , rows_ (0)
 , grid_ ()
 , thetaDivs_ (0)
 , theta0_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 367");
}

bool
PGE_367::OnUserCreate ()
{
  w_ = ENGINE_PGE_367_DEFAULT_R / std::sqrt (2.0f);

  columns_ = static_cast<int> (std::floor (olc::PixelGameEngine::ScreenWidth () / w_));
  rows_ = static_cast<int> (std::floor (olc::PixelGameEngine::ScreenHeight () / w_));

  for (int i = 0; i < rows_; i++)
  {
    std::vector<olc::vf2d> newRow;
    for (int j = 0; j < columns_; j++)
      newRow.push_back (olc::vf2d (std::numeric_limits<float>::quiet_NaN (), std::numeric_limits<float>::quiet_NaN ()));
    grid_.push_back (newRow);
  } // end FOR

  static std::vector<int> divs_a = { 4, 6, 8, 10, 12, 14, 16 };
  int index_i = Common_Tools::getRandomNumber (0, static_cast<int> (divs_a.size () - 1));
  thetaDivs_ = divs_a[index_i];
  theta0_ =
    (!Common_Tools::testRandomProbability (0.5f) || (thetaDivs_ % 4 == 0 && thetaDivs_ > 4)) ? 0.0f : static_cast<float> (M_PI) / static_cast<float> (thetaDivs_);

  olc::vf2d p = { olc::PixelGameEngine::ScreenWidth () / 2.0f, olc::PixelGameEngine::ScreenHeight () / 2.0f };
  int j = static_cast<int> (p.x / w_);
  int i = static_cast<int> (p.y / w_);
  grid_[i][j] = p;

  struct active_position active_s;
  active_s.position = p;
  active_s.hasBranched = false;
  active_s.thetaPossibilities.resize (thetaDivs_);
  std::iota (active_s.thetaPossibilities.begin (), active_s.thetaPossibilities.end (), 0);
  static std::random_device rd;
  static std::default_random_engine rng (rd ());
  std::shuffle (active_s.thetaPossibilities.begin (), active_s.thetaPossibilities.end (), rng);
  active_.push_back (active_s);

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_367::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    grid_.clear ();
    active_.clear ();
    OnUserCreate ();
  } // end IF

  if (active_.size () > 0)
  {
    std::list<active_position>::iterator iterator = active_.begin ();
    std::advance (iterator, Common_Tools::getRandomNumber (0, static_cast<int> (active_.size () - 1)));

    int theta_possibility = (*iterator).thetaPossibilities.back ();
    (*iterator).thetaPossibilities.pop_back ();
    float theta =
      ((2.0f * static_cast<float> (M_PI)) / static_cast<float> (thetaDivs_)) * theta_possibility + theta0_;
    olc::vf2d sample = { std::cos (theta), std::sin (theta) };
    sample = sample.norm ();
    sample *= static_cast<float> (ENGINE_PGE_367_DEFAULT_R);
    sample += (*iterator).position;

    int column = static_cast<int> (sample.x / w_);
    int row = static_cast<int> (sample.y / w_);

    if (column > ENGINE_PGE_367_DEFAULT_MARGIN && row > ENGINE_PGE_367_DEFAULT_MARGIN &&
        column < columns_ - 1 - ENGINE_PGE_367_DEFAULT_MARGIN && row < rows_ - 1 - ENGINE_PGE_367_DEFAULT_MARGIN &&
        std::isnan (grid_[row][column].x) && std::isnan (grid_[row][column].y))
    {
      bool ok = true;

      for (int i = std::max (row - 1, 0); i <= std::min (row + 1, rows_ - 1); i++)
        for (int j = std::max (column - 1, 0); j <= std::min (column + 1, columns_ - 1); j++)
        {
          olc::vf2d& neighbor = grid_[i][j];
          if (!std::isnan (neighbor.x) && !std::isnan (neighbor.y))
          {
            float d = distToSquared (sample, neighbor);
            if (d < static_cast<float> (ENGINE_PGE_367_DEFAULT_R * ENGINE_PGE_367_DEFAULT_R))
            {
              ok = false;
              break;
            }
          } // end IF

          if (!ok)
            break;
        } // end FOR

      if (ok)
      {
        grid_[row][column] = sample;
        (*iterator).hasBranched = true;

        struct active_position active_s;
        active_s.position = sample;
        active_s.hasBranched = false;
        active_s.thetaPossibilities.resize (thetaDivs_);
        std::iota (active_s.thetaPossibilities.begin (), active_s.thetaPossibilities.end (), 0);
        static std::random_device rd;
        static std::default_random_engine rng (rd ());
        std::shuffle (active_s.thetaPossibilities.begin (), active_s.thetaPossibilities.end (), rng);
        active_.push_back (active_s);

        olc::PixelGameEngine::DrawLine (sample, (*iterator).position, olc::BLACK, 0xFFFFFFFF);
      } // end IF
    } // end IF

    if ((*iterator).thetaPossibilities.empty ())
    {
      if (!(*iterator).hasBranched)
      {
        static olc::Pixel palette_a[3] =
          {{0x2B, 0x67, 0xAF, 255}, {0xEF, 0x56, 0x2F, 255}, {0xF9, 0xD5, 0x31, 255}};
        int index_i = Common_Tools::getRandomNumber (0, 3 - 1);
        olc::PixelGameEngine::FillCircle ((*iterator).position, static_cast<int32_t> (ENGINE_PGE_367_DEFAULT_R * 3.0f / 8.0f), palette_a[index_i]);
      } // end IF

      active_.erase (iterator);
    } // end IF
  } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
