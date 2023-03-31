#include "stdafx.h"

#include "pge_13.h"

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

PGE_13::PGE_13 ()
 : olc::PixelGameEngine ()
{
  sAppName = "Example 13";
}

PGE_13::~PGE_13 ()
{
}

void
PGE_13::add (const olc::vf2d& pos, const olc::vf2d& vel, const olc::Pixel& col, bool isX)
{
  struct particle p;
  p.pos = pos;
  p.vel = vel;
  p.acc = { 0.0f, 0.0f };
  p.col = col;
  p.bIsExplosion = isX;
  p.fLifespan = 0xFF;

  p.id = fireworks.size();
  fireworks.emplace_back(p);
}

void
PGE_13::explode (const olc::vf2d& pos)
{
  olc::Pixel colour = olc::Pixel (rand () % 256, rand () % 256, rand () % 256);
  for (float angle = 0.0f; angle < 2.0f * (float)M_PI; angle += 1.0f / (2.0f * (float)M_PI))
  {
    olc::vf2d RandVel;

    RandVel.x = sin(angle);
    RandVel.y = cos(angle);
    RandVel *= (rand() % 40);

    add (pos, RandVel, colour, true);
  } // end FOR
}

bool
PGE_13::OnUserCreate ()
{
  srand (time (NULL));

  olc::vf2d RandParticle, RandVel;
  RandParticle.y = ScreenHeight();
  RandVel.x = 0.0f;

  for (int i = 0; i < 7; i++)
  {
    RandParticle.x = rand() % ScreenWidth ();
    RandVel.y = -(150 + (rand () % 30));
    add (RandParticle, RandVel);
  }

  return true;
}

bool
PGE_13::OnUserUpdate (float fElapsedTime)
{
  Clear (olc::BLACK);

  for (auto it = fireworks.begin(); it != fireworks.end(); /* NOTHING */)
  {
    if ((*it).bIsExplosion && (*it).fLifespan <= 10.0f)
      it = fireworks.erase (it);
    else
      ++it;
  }

  for (auto& particle : fireworks)
  {
    particle.acc.y += gravity;
    particle.vel += particle.acc * fElapsedTime;
    particle.pos += particle.vel * fElapsedTime;
    particle.acc *= 0.0f;

    if (particle.bIsExplosion)
    {
      particle.fLifespan -= 0.1f;
      particle.col = olc::Pixel(particle.col.r, particle.col.g, particle.col.b, particle.fLifespan);
    }
    else if (particle.vel.y >= -20.0f)
    {
      explode (particle.pos);

      particle.pos.x = rand() % ScreenWidth();
      particle.pos.y = ScreenHeight();
      particle.vel.y = -(150.0f + (rand() % 30));
    }

    Draw (particle.pos, particle.col);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
