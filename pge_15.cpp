#include "stdafx.h"

#include "PGE_15.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_15::PGE_15 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 15";
}

PGE_15::~PGE_15 ()
{
}

bool
PGE_15::OnUserCreate ()
{
  columns_ = std::floor (ENGINE_PGE_15_DEFAULT_WIDTH / ENGINE_PGE_15_DEFAULT_SCALE);
  rows_ = std::floor (ENGINE_PGE_15_DEFAULT_HEIGHT / ENGINE_PGE_15_DEFAULT_SCALE);

  flowField_ = new olc::vd2d[columns_ * rows_];

  for (int i = 0; i < ENGINE_PGE_15_DEFAULT_PARTICLES; i++)
  {
    particles_.push_back (new particle (ENGINE_PGE_15_DEFAULT_SCALE));
  }

  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_15::OnUserUpdate (float fElapsedTime)
{
  double yoff = 0.0;
  for (ACE_UINT32 y = 0; y < rows_; y++)
  {
    double xoff = 0.0;
    for (ACE_UINT32 x = 0; x < columns_; x++)
    {
      double angle_d =
        ((1.0 + module.GetValue (xoff, yoff, z_)) / 2.0) * 2.0 * M_PI * 4.0;
      olc::vd2d v = { std::cos (angle_d), std::sin (angle_d) };
      v = v.norm ();
      flowField_[x + y * columns_] = v;
      xoff += step_;
    } // end FOR
    yoff += step_;

    z_ += 0.0003;
  } // end FOR

  for (std::vector<particle*>::iterator i = particles_.begin ();
       i < particles_.end ();
       i++)
  {
    (*i)->follow (flowField_);
    (*i)->update ();
    (*i)->edges ();
    (*i)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
