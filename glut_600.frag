#version 130

// glut_600_common.glsl
#define PI 3.14159
#define FOUR_PI 4.0 * PI
#define GAMMA 2.2
#define INV_GAMMA (1.0/GAMMA)

const float minDot = 1e-3;

float
dot_c (vec3 a, vec3 b)
{
  return max(dot(a, b), minDot);
}

vec3
gamma (vec3 col)
{
  return pow(col, vec3(INV_GAMMA));
}

vec3
inv_gamma (vec3 col)
{
  return pow(col, vec3(GAMMA));
}

float
saturate (float x)
{
  return clamp(x, 0.0, 1.0);
}
// glut_600_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define CAMERA_DIST 2.5

void
main ()
{
  if((gl_FragCoord.x == 0.5) && (gl_FragCoord.y < 4.0))
  {
    vec4 oldMouse = texelFetch(iChannel0, ivec2(0.5), 0).xyzw;
    vec4 mouse = (iMouse / iResolution.xyxy); 
    vec4 newMouse = vec4(0);

    float mouseDownLastFrame = texelFetch(iChannel0, ivec2(0.5, 3.5), 0).x;
    if(iMouse.z > 0.0 && mouseDownLastFrame > 0.0)
    {
      vec2 mouseMove = mouse.xy-oldMouse.zw;
      newMouse = vec4(oldMouse.xy + vec2(5.0, 3.0)*mouseMove, mouse.xy);
    }
    else
    {
      newMouse = vec4(oldMouse.xy, mouse.xy);
    }
    newMouse.x = mod(newMouse.x, 2.0*PI);
    newMouse.y = min(0.99, max(-0.99, newMouse.y));

    if(gl_FragCoord.xy == vec2(0.5, 0.5))
    {
      if(iFrame < 5)
      {
        newMouse = vec4(1.15, 0.2, 0.0, 0.0);
      }
      gl_FragColor = vec4(newMouse);
    }

    if(gl_FragCoord.xy == vec2(0.5, 1.5))
    {
      vec3 cameraPos = CAMERA_DIST * vec3(sin(newMouse.x), -sin(newMouse.y), -cos(newMouse.x));
      gl_FragColor = vec4(cameraPos, 1.0);
    }

    if(gl_FragCoord.xy == vec2(0.5, 2.5))
    {
      float resolutionChangeFlag = 0.0;
      vec2 oldResolution = texelFetch(iChannel0, ivec2(0.5, 2.5), 0).yz;
      if(iResolution.xy != oldResolution)
      {
        resolutionChangeFlag = 1.0;
      }
      gl_FragColor = vec4(resolutionChangeFlag, iResolution.xy, 1.0);
    }

    if(gl_FragCoord.xy == vec2(0.5, 3.5))
    {
      if(iMouse.z > 0.0)
      {
        gl_FragColor = vec4(vec3(1.0), 1.0);
      }
      else
      {
        gl_FragColor = vec4(vec3(0.0), 1.0);
      }
    }
  }
}
