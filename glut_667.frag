#version 130

uniform float iTime;
uniform int iFrame;
uniform bvec4 iKeyboard; // space, ctrl
uniform sampler2D iChannel0;

#define UNIQUE_SETUPS 16
const ivec2 playlist[UNIQUE_SETUPS] = ivec2[UNIQUE_SETUPS](

    ivec2(5, 4), // S **** 5.6.6     truncated icosahedron
    ivec2(3, 2), // L **   3.3.3.3   octahedron
    ivec2(4, 6), // M **** 4.6.8     truncated cuboctahedron
    ivec2(5, 5), // S ***  3.4.5.4   rhombicosidodecahedron
    ivec2(3, 0), // L **   3.3.3     tetrahedron    
    ivec2(5, 6), // M **** 4.6.10    truncated icosidodecahedron
    ivec2(3, 6), // L ***  4.6.6     truncated octahedron 
    ivec2(5, 2), // S ***  3.5.3.5   icosidodecahedron
    ivec2(4, 1), // L **   4.4.4     cube
    ivec2(5, 0), // M ***  3.3.3.3.3 icosahedron
    ivec2(4, 3), // S *    3.8.8     truncated cube
    ivec2(4, 5), // M ***  3.4.4.4   rhombicuboctahedron
    ivec2(5, 3), // S **   3.10.10   truncated dodecahedron
    ivec2(3, 5), // M ***  3.4.3.4   cuboctahedron 
    ivec2(5, 1), // M ***  5.5.5     dodecahedron
    ivec2(3, 3)  // M *    3.6.6     truncated tetrahedron

);

const float transition_duration = 1.25;

const float demo_example_duration = (60.0 / float(UNIQUE_SETUPS));

bool
key_press (int key)
{
  if (key == 83) // 'S' --> space
    return iKeyboard.x;
  else if (key == 68) // 'D' --> ctrl
    return iKeyboard.y;

  return false;
}

void
main ()
{
  ivec2 fc = ivec2(gl_FragCoord.xy);
  if (fc.x > 3 || fc.y > 0)
    return;

  vec4 cur_setup = texelFetch(iChannel0, ivec2(0,0), 0);
  vec4 next_setup = texelFetch(iChannel0, ivec2(1,0), 0);
  vec4 queued_setup = texelFetch(iChannel0, ivec2(2,0), 0);
  vec4 time = texelFetch(iChannel0, ivec2(3,0), 0);

  bool is_paused = (iFrame != 0 && time.x == iTime);

  if (iFrame == 0 || cur_setup.x == 0.)
  {
    cur_setup = vec4(vec2(playlist[0]), 2.0, iTime);
    next_setup = vec4(0);
    queued_setup = vec4(0);
    time = vec4(0, 0, demo_example_duration, 0);
  }

  vec3 modified_setup = (queued_setup.x != 0. ? queued_setup.xyz :
                           next_setup.x != 0. ? next_setup.xyz   : cur_setup.xyz);

  float reset_demo = 1.0;
  bool changed = false;
  bool reset_w = false;

// Demo automatically cycles through a "playlist" of 16 polyhedra.
//
// Use the mouse to rotate the orb or click in the lower-left corner
// to restore automatic rotation.
//
// KEYS: (these break out of demo mode):
//
//   A/S prev/next in playlist
//
//   Q/W change symmetry (tetrahedral/octahedral/icosahedral)
//   E/R modify vertex location (7 possibilities)
//   T/Y modify random seed (2^16 possibilities!)
//
// To return to demo mode, press D (won't do anything if paused)
  if (key_press(81))
  {
    modified_setup.x = 3.0 + mod(modified_setup.x + 2.0, 3.0);
    changed = reset_w = true;
  }
  else if (key_press(87))
  {
    modified_setup.x = 3.0 + mod(modified_setup.x + 1.0, 3.0);
    changed = reset_w = true;
  }
  else if (key_press(69))
  {
    modified_setup.y = mod(modified_setup.y + 6.0, 7.0);
    changed = reset_w = true;
  }
  else if (key_press(82))
  {
    modified_setup.y = mod(modified_setup.y + 1.0, 7.0);
    changed = reset_w = true;
  }
  else if (key_press(84))
  {
    modified_setup.z = mod(modified_setup.z + 65535., 65536.);
    changed = true;
  }
  else if (key_press(89))
  {
    modified_setup.z = mod(modified_setup.z + 1., 65536.);
    changed = true;
  }
  else if (key_press(65))
  {
    time.w = mod(time.w + float(UNIQUE_SETUPS) - 1.0, float(UNIQUE_SETUPS));
    modified_setup.xy = vec2(playlist[int(time.w)]);
    changed = true;
  }
  else if (key_press(83)) // 'S'
  {
    time.w = mod(time.w + 1.0, float(UNIQUE_SETUPS));
    modified_setup.xy = vec2(playlist[int(time.w)]);
    changed = true;
  }
  else if (key_press(68)) // 'D'
  {
    time.z = iTime + demo_example_duration;
  }
  else if (time.z >= 0.)
  {
    float demo_transition_time = time.z - transition_duration;
    if (iTime > demo_transition_time)
    {
      time.w = mod(time.w + 1.0, float(UNIQUE_SETUPS));
      cur_setup.w = demo_transition_time;
      next_setup = cur_setup;
      next_setup.xy = vec2(playlist[int(time.w)]);
      if (time.w == 0.)
        next_setup.z = mod(next_setup.z + 1., 65536.);
      next_setup.w = time.z;
      queued_setup = vec4(0);
      time.z += demo_example_duration;
    }
  }

  if (reset_w)
  {
    time.w = 0.0;
    ivec2 s = ivec2(modified_setup.xy);
    for (int i=0; i<UNIQUE_SETUPS; ++i)
    {
      if (playlist[i] == s)
      {
        time.w = float(i);
        break;
      }
    }
  }

  if (changed)
  {
    if (next_setup.x != 0.)
      queued_setup.xyz = modified_setup;
    else
    {
      cur_setup.w = iTime;
      next_setup = vec4(modified_setup, iTime + transition_duration);
    }

    time.z = -1.0;
  }

  if (next_setup.x != 0.)
  {
    if (is_paused)
    {
        if (queued_setup.x != 0.)
        {
          cur_setup = queued_setup;
          next_setup = vec4(0);
          queued_setup = vec4(0);
        }
        else
        {
          cur_setup = next_setup;
          next_setup = vec4(0);
        }
    }
    else if (next_setup.w < iTime)
    {
      cur_setup = next_setup;
      if (queued_setup.x != 0.)
      {
        cur_setup.w = iTime;
        next_setup = vec4(queued_setup.xyz, iTime + transition_duration);
        queued_setup = vec4(0);
      }
      else
        next_setup = vec4(0);
    }
  }

  if (fc == ivec2(0, 0))
  {
    gl_FragColor = cur_setup;
  }
  else if (fc == ivec2(1, 0))
  {
    gl_FragColor = next_setup;
  }
  else if (fc == ivec2(2, 0))
  {
    gl_FragColor = queued_setup;
  }
  else
  {
    time.x = time.y;
    time.y = iTime;
    gl_FragColor = time;
  }
}
