#include "stdafx.h"

#include "pge.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "engine_common.h"

PGE::PGE ()
{
  sAppName = "Example";
}

bool
PGE::OnUserCreate()
{
  // Called once at the start, so create things here
  return true;
}

bool
PGE::OnUserUpdate (float fElapsedTime)
{
  // called once per frame
  for (int x = 0; x < ScreenWidth (); x++)
    for (int y = 0; y < ScreenHeight (); y++)
      Draw (x, y, olc::Pixel (rand () % 255, rand () % 255, rand () % 255));
  return true;
}
