#include "stdafx.h"

#include "pge_10.h"

#include <algorithm>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "common_ui_defines.h"

#include "defines.h"
#include "engine_common.h"

//////////////////////////////////////////

PGE_10::PGE_10 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 10";
}

PGE_10::~PGE_10 ()
{
}

bool
PGE_10::OnUserCreate ()
{
  // Load all particle images
  //diamond.Load("./gfx/particles/diamond.png");
  //star.Load("./gfx/particles/star.png");
  //fire.Load("./gfx/particles/fire.png");
  //smoke.Load("./gfx/particles/smoke2.png");
  circle.Load("./circle.png");
  //blue.Load("./gfx/particles/bluedark.png");


  img[0] = &diamond;
  img[1] = &star;
  img[2] = &fire;
  img[3] = &smoke;
  img[4] = &circle;
  img[5] = &blue;

  particleEngine.Load();

  // Seed rand so it's not the same random numbers each time.
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  // Create 200 random particles
  vec2d start = { float(rand() % 156) + 50, float(rand() % 60) + 180 };

  for (int i = 0; i < 100; i++)
  {
    Particle p;
    p.position = start;
    p.velocity = { 40, -60 };
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    p.acceleration = { -r * 80 , r2 * 60 };
    p.color = olc::Pixel(rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    p.alive = true;
    p.scale = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    int whichImg = rand() % 6;
    //p.sprite = img[whichImg];
    p.sprite = img[4];
    p.max_lifetime = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 5.0f;

    particleEngine.particles.push_back(p);
  }
  return true;
}

bool
PGE_10::OnUserUpdate (float fElapsedTime)
{
  particleEngine.moveEmitter ({ float(GetMouseX()), float(GetMouseY()) });

  if (GetMouse(0).bHeld)
    particleEngine.particlesMoveWithEmitter = true;
  else
    particleEngine.particlesMoveWithEmitter = false;

  //SetPixelMode(olc::Pixel::ALPHA);
  //SetPixelMode(olc::Pixel::NORMAL);
  particleEngine.Update (this, fElapsedTime);
  particleEngine.Draw (this);

  //std::cout << particleEngine.particles.size() << '\n';

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
