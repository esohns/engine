#include "stdafx.h"

#include "pge_194.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_194::PGE_194 ()
 : olc::PixelGameEngine ()
 , agents_ ()
 , highlightAgents_ (ENGINE_PGE_194_HIGHLIGHT_AGENTS)
 , weights_ ()
 , trail_ (NULL)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 194");
}

PGE_194::~PGE_194 ()
{
  delete [] trail_;
}

bool
PGE_194::OnUserCreate ()
{
  weights_.push_back (1.0f / 16.0f);
  weights_.push_back (1.0f / 8.0f);
  weights_.push_back (1.0f / 16.0f);
  weights_.push_back (1.0f / 8.0f);
  weights_.push_back (1.0f / 4.0f);
  weights_.push_back (1.0f / 8.0f);
  weights_.push_back (1.0f / 16.0f);
  weights_.push_back (1.0f / 8.0f);
  weights_.push_back (1.0f / 16.0f);

  trail_ = new float [ENGINE_PGE_194_DEFAULT_W * ENGINE_PGE_194_DEFAULT_W];
  ACE_OS::memset (trail_, 0, sizeof (float) * ENGINE_PGE_194_DEFAULT_W * ENGINE_PGE_194_DEFAULT_W);

  return true;
}

bool
PGE_194::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    for (int i = 0; i < 100; i++)
    {
      struct agent agent_s;
      agent_s.position.x = static_cast<float> (olc::PixelGameEngine::GetMouseX ());
      agent_s.position.y = static_cast<float> (olc::PixelGameEngine::GetMouseY ());
      agent_s.heading =
        Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)) + static_cast<float> (M_PI) / 12.0f;
      agent_s.last_option = 0;
      agents_.push_back (agent_s);
    } // end FOR
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::H).bPressed)
    highlightAgents_ = !highlightAgents_;
  if (olc::PixelGameEngine::GetKey (olc::SPACE).bPressed)
    resetSimulation (0.5f, 0.5f);

  sim_step ();
  render ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_194::sense_relative_angle (struct agent& agent, float theta)
{
  return trail_[index (std::min (std::max (static_cast<int> (agent.position.x + std::cos (agent.heading + theta) * ENGINE_PGE_194_SENSOR_DISTANCE), 0), ENGINE_PGE_194_DEFAULT_W - 1),
                       std::min (std::max (static_cast<int> (agent.position.y + std::sin (agent.heading + theta) * ENGINE_PGE_194_SENSOR_DISTANCE), 0), ENGINE_PGE_194_DEFAULT_W - 1))];
}

void
PGE_194::step_sense_and_rotate ()
{
  for (std::vector<struct agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
  {
    float sense_left   = sense_relative_angle (*iterator, ENGINE_PGE_194_SENSOR_ANGLE);
    float sense_middle = sense_relative_angle (*iterator, 0.0f);
    float sense_right  = sense_relative_angle (*iterator, -ENGINE_PGE_194_SENSOR_ANGLE);

    float modified_turning =
      (ENGINE_PGE_194_RAMDOM_TURNING ? (Common_Tools::getRandomNumber (0.0f, 1.0f) * 0.5f + 0.5f)
                                     : 1.0f) * ENGINE_PGE_194_TURNING_SPEED;
    int option = -1;
    if (sense_middle > sense_left && sense_middle > sense_right)
    {
      option = 0;
    } // end IF
    else if (sense_left > sense_right)
    {
      option = 1;
      iterator->heading += modified_turning;
    } // end ELSE IF
    else if (sense_right > sense_left)
    {
      option = 2;
      iterator->heading -= modified_turning;
    } // end ELSE IF
    else
    {
      option = 3;
      iterator->heading +=
        (Common_Tools::getRandomNumber (0.0f, 1.0f) * 2.0f - 1.0f) * ENGINE_PGE_194_TURNING_SPEED;
    } // end ELSE
    iterator->last_option = option;
  } // end FOR
}

void
PGE_194::step_move ()
{
  for (std::vector<struct agent>::iterator iterator = agents_.begin ();
       iterator != agents_.end ();
       ++iterator)
  {
    iterator->position.x += ENGINE_PGE_194_SPEED * std::cos (iterator->heading);
    iterator->position.y += ENGINE_PGE_194_SPEED * std::sin (iterator->heading);
    if (ENGINE_PGE_194_WRAP_AROUND)
    {
      if (iterator->position.x < 0.0f)
        iterator->position.x += static_cast<float> (ENGINE_PGE_194_DEFAULT_W);
      if (iterator->position.x > static_cast<float> (ENGINE_PGE_194_DEFAULT_W))
        iterator->position.x -= static_cast<float> (ENGINE_PGE_194_DEFAULT_W);
      if (iterator->position.y < 0.0f)
        iterator->position.y += static_cast<float> (ENGINE_PGE_194_DEFAULT_W);
      if (iterator->position.y > static_cast<float> (ENGINE_PGE_194_DEFAULT_W))
        iterator->position.y -= static_cast<float> (ENGINE_PGE_194_DEFAULT_W);
    } // end IF
  } // end FOR
}

void
PGE_194::step_deposit ()
{
  for (std::vector<struct agent>::iterator iterator = agents_.begin();
       iterator != agents_.end();
       ++iterator)
  {
    int x = static_cast<int> (iterator->position.x);
    int y = static_cast<int> (iterator->position.y);
    if (x <= 0 || y <= 0 || x >= ENGINE_PGE_194_DEFAULT_W - 1 || y >= ENGINE_PGE_194_DEFAULT_W - 1)
      continue;
    trail_[index (x, y)] += ENGINE_PGE_194_DEPOSIT_AMOUNT;
  } // end FOR
}

void
PGE_194::step_diffuse_and_decay ()
{
  float* old_trail_p = new float[ENGINE_PGE_194_DEFAULT_W * ENGINE_PGE_194_DEFAULT_W];
  ACE_OS::memcpy (old_trail_p, trail_, sizeof (float) * ENGINE_PGE_194_DEFAULT_W * ENGINE_PGE_194_DEFAULT_W);

  for (int y = 1; y < ENGINE_PGE_194_DEFAULT_W - 1; ++y)
    for (int x = 1; x < ENGINE_PGE_194_DEFAULT_W - 1; ++x)
    {
      float diffused_value =
        old_trail_p[index (x - 1, y - 1)] * weights_[0] +
        old_trail_p[index (x,     y - 1)] * weights_[1] +
        old_trail_p[index (x + 1, y - 1)] * weights_[2] +
        old_trail_p[index (x - 1, y)]     * weights_[3] +
        old_trail_p[index (x,     y)]     * weights_[4] +
        old_trail_p[index (x + 1, y)]     * weights_[5] +
        old_trail_p[index (x - 1, y + 1)] * weights_[6] +
        old_trail_p[index (x    , y + 1)] * weights_[7] +
        old_trail_p[index (x + 1, y + 1)] * weights_[8];
      trail_[index (x, y)] = std::min (1.0f, diffused_value * ENGINE_PGE_194_DECAY_FACTOR);
    } // end FOR

  delete [] old_trail_p;
}

void
PGE_194::sim_step ()
{
  step_sense_and_rotate ();
  step_move ();
  step_deposit ();
  step_diffuse_and_decay ();
}

void
PGE_194::render ()
{
  uint8_t max_brightness = highlightAgents_ ? 50 : 255;
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();

  for (int y = 0; y < ENGINE_PGE_194_DEFAULT_W; ++y)
    for (int x = 0; x < ENGINE_PGE_194_DEFAULT_W; ++x)
    {
      int index = x + y * ENGINE_PGE_194_DEFAULT_W;
      uint8_t brightness = static_cast<uint8_t> (trail_[index] * max_brightness);
      p[index] = {brightness, brightness, brightness, 255};
    } // end FOR

  if (highlightAgents_)
    for (std::vector<struct agent>::iterator iterator = agents_.begin();
         iterator != agents_.end();
         ++iterator)
    {
      olc::Pixel color;
      switch (iterator->last_option)
      {
        case 0: color = {150, 50, 50, 255}; break;   // straight
        case 1: color = {50, 150, 50, 255}; break;   // right
        case 2: color = {50, 50, 150, 255}; break;   // left
        case 3: color = {255, 255, 255, 255}; break; // indecisive
      } // end SWITCH
      p[static_cast<int> (iterator->position.x) + static_cast<int> (iterator->position.y) * ENGINE_PGE_194_DEFAULT_W] = color;
    } // end FOR
}

void
PGE_194::regenerate (float xtrans, float ytrans)
{
  agents_.clear ();

  struct agent agent_s;
  agent_s.last_option = 0;
  for (int i = 0; i < ENGINE_PGE_194_NUMBER_OF_AGENTS; ++i)
  {
    if (ENGINE_PGE_194_START_IN_CIRCLE)
    {
      agent_s.position.x = ENGINE_PGE_194_DEFAULT_W * xtrans;
      agent_s.position.y = ENGINE_PGE_194_DEFAULT_W * ytrans;
      float t =
        2.0f * static_cast<float> (M_PI) * i / static_cast<float> (ENGINE_PGE_194_NUMBER_OF_AGENTS);
      agent_s.heading = t + static_cast<float> (M_PI) / 12.0f;
    } // end IF
    else
    {
      agent_s.position.x =
        Common_Tools::getRandomNumber (0.0f, 1.0f) * ENGINE_PGE_194_DEFAULT_W;
      agent_s.position.y =
        Common_Tools::getRandomNumber (0.0f, 1.0f) * ENGINE_PGE_194_DEFAULT_W;
      agent_s.heading =
        Common_Tools::getRandomNumber (0.0f, 1.0f) * 2.0f * static_cast<float> (M_PI);
    } // end ELSE
    agents_.push_back (agent_s);
  } // end FOR
}

void
PGE_194::resetSimulation (float xtrans, float ytrans)
{
  ACE_OS::memset (trail_, 0, sizeof (float) * ENGINE_PGE_194_DEFAULT_W * ENGINE_PGE_194_DEFAULT_W);

  regenerate (xtrans, ytrans);
}
