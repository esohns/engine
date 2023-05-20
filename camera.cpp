#include "GL/freeglut.h"

#include "camera.h"

Camera::Camera ()
 : keyboard (Camera::QWERTY),
   X (0.0f),
   Y (0.0f),
   Z (0.0f),
   mouse_x (0),
   mouse_y (0),
   psi (0.0f),
   theta (0.0f),
   rotation_speed (0.0f),
   translation_speed (0.0f),
   time (0)
{
  for (int i = 0 ; i < 255 ; i++) { keys[i] = false; }
  switch (keyboard)
  {
    case AZERTY:
      key_map[FORWARD]  = static_cast<int> ('z');
      key_map[BACKWARD] = static_cast<int> ('s');
      key_map[LEFT]     = static_cast<int> ('q');
      key_map[RIGHT]    = static_cast<int> ('d');
      break;
    case QWERTY:
      key_map[FORWARD]  = static_cast<int> ('w');
      key_map[BACKWARD] = static_cast<int> ('s');
      key_map[LEFT]     = static_cast<int> ('a');
      key_map[RIGHT]    = static_cast<int> ('d');
      break;
  }
}

void
Camera::init (KEYBOARD p_keyboard,
              float p_X,
              float p_Y,
              float p_Z,
              float p_psi,
              float p_theta,
              float p_rotation_speed,
              float p_translation_speed,
              int p_window_width,
              int p_window_height)
{
  keyboard = p_keyboard;
  X = p_X;
  Y = p_Y;
  Z = p_Z;
  mouse_x = p_window_width / 2;
  mouse_y = p_window_height / 2;
  psi = p_psi;
  theta = p_theta;
  rotation_speed = p_rotation_speed;
  translation_speed = p_translation_speed;
}

void
Camera::rotation (int x, int y)
{
  theta -= static_cast<float> (x - mouse_x) * rotation_speed;
  psi   += static_cast<float> (y - mouse_y) * rotation_speed;
  if (psi <= 0.1f)                                   psi = 0.1f;
  else if (psi >= 0.95f * static_cast<float> (M_PI)) psi = 0.95f * static_cast<float> (M_PI);
  mouse_x = x;
  mouse_y = y;
}

void
Camera::translation ()
{
  float t = static_cast<float> (glutGet (GLUT_ELAPSED_TIME) - time);
  time    = glutGet (GLUT_ELAPSED_TIME);
  if (keys[key_map[FORWARD]])
  {
    X += sinf (theta) * sinf (psi) * translation_speed * t;
    Y += cosf (psi)                * translation_speed * t;
    Z += cosf (theta) * sinf (psi) * translation_speed * t;
  }
  if (keys[key_map[BACKWARD]])
  {
    X -= sinf (theta) * sinf (psi) * translation_speed * t;
    Y -= cosf (psi)                * translation_speed * t;
    Z -= cosf (theta) * sinf (psi) * translation_speed * t;
  }
  if (keys[key_map[LEFT]])
  {
    X -= sinf (theta - static_cast<float> (M_PI/2.0)) * sinf (psi) * translation_speed * t;
    Z -= cosf (theta - static_cast<float> (M_PI/2.0)) * sinf (psi) * translation_speed * t;
  }
  if (keys[key_map[RIGHT]])
  {
    X -= sinf (theta + static_cast<float> (M_PI/2.0)) * sinf (psi) * translation_speed * t;
    Z -= cosf (theta + static_cast<float> (M_PI/2.0)) * sinf (psi) * translation_speed * t;
  }
}
