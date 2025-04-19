#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define Dt (1.0)
#define NITER	100

vec2
screen2world (vec2 fragCoord)
{
  return (2.0*fragCoord.xy - iResolution.xy)/iResolution.y;
}

vec2
world2screen (vec2 pos)
{
  return (pos*iResolution.y + iResolution.xy) * 0.5;
}

vec2
screen2uv (vec2 fragCoord)
{
  return fragCoord / iResolution.xy;
}

vec2
uv2screen (vec2 uv)
{
  return uv * iResolution.xy;
}

vec2
world2uv (vec2 pos)
{
  return world2screen(pos) / iResolution.xy;
}

vec2
uv2world (vec2 uv)
{
  return screen2world(uv2screen(uv));
}

vec2
implicitSolveV (vec2 pos)
{
  vec2 posInit = pos;
  for(int i=0; i<NITER; i++)
    pos = posInit - Dt*texture(iChannel1,  world2uv(pos)).xy;

  return pos;
}

void
main ()
{
  vec2 pos = screen2world(gl_FragCoord.xy);
  if (iMouse.x < 10.0)
  {
    if (length(vec2(cos(iTime), 0.7*sin(2.0*iTime))*vec2(1.2,0.6) - pos) < .06)
    {
      gl_FragColor = vec4(1.,1.,1.,1.);
      return;
    }
  }
  else
  {
    if (iMouse.z > 0.0)
      if (length(screen2world(iMouse.xy)-pos) < .06)
      {
        gl_FragColor = vec4(1.,1.,1.,1.);
        return;
      }
  }

  vec2 advPos = implicitSolveV(pos);
  vec4 newVal = texture(iChannel0, world2uv(advPos));
  gl_FragColor = vec4(0.99, 0.99, 1.0, 1.0) * newVal *.99;
  //gl_FragColor = vec4(world2uv(advPos), 0.0, 0.0);
}
