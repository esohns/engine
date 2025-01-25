#ifndef ENGINE_COMMON_H
#define ENGINE_COMMON_H

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#endif // GLM_SUPPORT

#include "common_gl_defines.h"

struct Engine_GL_Camera
{
  Engine_GL_Camera ()
    : position (0.0f, 0.0f, 0.0f)
    , looking_at (0.0f, 0.0f, -1.0f)
    , up (0.0f, 1.0f, 0.0f)
    , old_mouse_position (0.0f, 0.0f)
    // *NOTE*: use these to 'manually' compute the different view matrices
    // *TODO*: remove these ASAP
    , zoom (0.0f)
    , rotation (0.0f, 0.0f, 0.0f)
    , translation (0.0f, 0.0f, 0.0f)
  {}

  glm::vec3 position;
  glm::vec3 looking_at;
  glm::vec3 up;
  inline glm::mat4 getViewMatrix () { return glm::lookAt (position, position + looking_at, up); }

  void reset ()
  {
    position = { 0.0f, 0.0f, 0.0f };
    looking_at = { 0.0f, 0.0f, -1.0f };
    up = { 0.0f, 1.0f, 0.0f };

    //old_mouse_position = {0.0f, 0.0f};
  }

  inline void forward (float speed_in)
  {
    position += looking_at * (speed_in * COMMON_GL_CAMERA_DEFAULT_ZOOM_FACTOR);
  }
  inline void backward (float speed_in)
  {
    position -= looking_at * (speed_in * COMMON_GL_CAMERA_DEFAULT_ZOOM_FACTOR);
  }
  void left (float speed_in)
  {
    glm::vec3 right = glm::cross (looking_at, up);
    position +=
      right * (speed_in * COMMON_GL_CAMERA_DEFAULT_TRANSLATION_FACTOR);
  }
  void right (float speed_in)
  {
    glm::vec3 right = glm::cross (looking_at, up);
    position -=
      right * (speed_in * COMMON_GL_CAMERA_DEFAULT_TRANSLATION_FACTOR);
  }

  glm::vec2 old_mouse_position;
  void mouseLook (int mouseX_in, int mouseY_in)
  {
    glm::vec2 current_mouse_position (mouseX_in, mouseY_in);

    static bool first_b = true;
    if (first_b)
    {
      old_mouse_position = current_mouse_position;
      first_b = false;
    } // end IF

    glm::vec2 mouse_delta = old_mouse_position - current_mouse_position;
    mouse_delta *= COMMON_GL_CAMERA_DEFAULT_MOUSE_LOOK_FACTOR;
    looking_at = glm::rotate (looking_at, glm::radians (mouse_delta.x), up);

    old_mouse_position = current_mouse_position;
  }

  float                     zoom;
  // *TODO*: remove these ASAP
  glm::vec3                 rotation;
  glm::vec3                 translation;
};

#endif // ENGINE_COMMON_H
