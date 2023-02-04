#include "stdafx.h"

#include "PGE_12.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_12::PGE_12 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 11";
}

PGE_12::~PGE_12 ()
{
}

void
PGE_12::getSignalData (const std::string& filename_in)
{
  float x, y;
  std::ifstream datafile (filename_in);

  vSignalX.clear ();
  vSignalY.clear ();

  if (datafile.is_open ())
  {
    while (datafile.good ())
    {
      datafile >> x >> y;
      vSignalX.push_back ({x, 0.0f});
      vSignalY.push_back ({y, 0.0f});
    }
    datafile.close ();
  } // end IF
}

std::vector<PGE_12::t_fourier>
PGE_12::computeDFT (const std::vector<std::complex<float> >& input_in)
{
  std::vector<t_fourier> output;

  const size_t N = input_in.size ();
  struct t_fourier f;
  float phi;

  for (size_t k = 0; k < N; k++)
  {
    f.com = {0.0f, 0.0f};
    for (size_t n = 0; n < N; n++)
    {
      phi = -2.0f * M_PI * n * k / N;
      f.com += input_in[n] * std::exp (std::complex<float> (0, -phi));
    } // end FOR
    f.com /= (float)N;
    f.nFreq = k;
    f.fAmp = std::sqrt (f.com.real () * f.com.real () + f.com.imag () * f.com.imag ());
    f.fPhase = std::atan2 (f.com.imag (), f.com.real ());
    output.push_back (f);
  } // end FOR

  // *NOTE*: sort in decending amplitude size (optional)
  std::sort (output.begin (), output.end (), [](const t_fourier& a, const t_fourier& b) { return a.fAmp > b.fAmp; });

  return output;
}

void
PGE_12::createDFTFromPicture ()
{
  getSignalData (sPictures[nPic]);

  vFourierX = computeDFT (vSignalX);
  vFourierY = computeDFT (vSignalY);
}

olc::vf2d
PGE_12::drawEpicycle (olc::vf2d origin_in, float rotation_in, const std::vector<struct t_fourier>& vFourier_in)
{
  for (auto& vF : vFourier_in)
  {
    if (vF.nFreq != 0)
    {
      olc::vf2d pre_orig = origin_in;

      origin_in +=
        {vF.fAmp * std::cos (vF.nFreq * fDelta + vF.fPhase + rotation_in),
         vF.fAmp * std::sin (vF.nFreq * fDelta + vF.fPhase + rotation_in)};

      DrawCircle (pre_orig, std::fabs (vF.fAmp), olc::VERY_DARK_GREY);

      DrawLine (pre_orig, origin_in);
    } // end IF
  } // end FOR

  return origin_in;
}

bool
PGE_12::OnUserCreate ()
{
  y_circle_orig = { 100.0f, (float)ScreenHeight() / 2 };
  x_circle_orig = { (float)ScreenWidth() / 2, 50.0f };

  createDFTFromPicture ();
 
  return true;
}

bool
PGE_12::OnUserUpdate (float fElapsedTime)
{
  if (GetKey (olc::Key::SPACE).bPressed)
  {
    nPic += 1;
    if (nPic == 5) nPic = 0;
    createDFTFromPicture ();
    vCurrentPath.clear();
    vPreviousPath.clear();
    fDelta = 0;
  } // end IF

  fTick += fElapsedTime;
  if (fTick > 0.05f)      //Slow down update
  {
    Clear(olc::BLACK);

    olc::vf2d vx1 = drawEpicycle (x_circle_orig, 0.0f, vFourierX);
    olc::vf2d vy1 = drawEpicycle (y_circle_orig, (M_PI / 2), vFourierY);

    //Add last X,Y value to path vector
    olc::vf2d v2 = { vx1.x, vy1.y };
    vCurrentPath.insert(vCurrentPath.begin(), v2); //Save Position

    //Draw virtual pen between last epicycle and path
    DrawLine(vx1, v2, olc::GREEN);
    DrawLine(vy1, v2, olc::GREEN);

    //Draw previous path
    if (!vPreviousPath.empty())
    {
      for (int i = 0; i < vPreviousPath.size() - 1; i++)
      {
        DrawLine(vPreviousPath[i], vPreviousPath[i + 1], olc::DARK_GREEN);
      }
      //need to join the last and first points at the end..
      DrawLine(vPreviousPath.front(), vPreviousPath.back(), olc::DARK_GREEN);
    }

    //Draw current path
    for (int i = 0; i < vCurrentPath.size() - 1; i++)
    {
      DrawLine(vCurrentPath[i], vCurrentPath[i + 1], olc::GREEN);
    }

    //Draw text
    DrawString(50, 20, "Fourier Epicycles", olc::WHITE, 2);
    DrawString(100, 430, "Use Space to change drawing", olc::WHITE, 2);
    DrawString(50, 50, "Epicycles used: " + std::to_string(vFourierX.size()));

    //Move delta by fraction of total size.
    fDelta += (2 * M_PI) / vFourierX.size ();
    //Remove excess points on wave
    if (fDelta > (2 * M_PI))
    {
      vPreviousPath = vCurrentPath;
      vCurrentPath.clear();
      fDelta = 0;
    }

    fTick = 0.0f;
  }

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
