#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
  float d = 0.;
  vec3 e = vec3(vec2(1.)/iResolution.xy,0.);
  vec2 q = gl_FragCoord.xy/iResolution.xy;
  vec4 c = texture(iChannel0, q);

  bool boundary = false;
  vec2 dir = vec2(0.);
  if (gl_FragCoord.y < 1.) { dir.y = 1.; boundary = true; }
  if (gl_FragCoord.y > iResolution.y-1.) { dir.y = -1.; boundary = true; }
  if (gl_FragCoord.x < 1.) { dir.x = 1.; boundary = true; }
  if (gl_FragCoord.x > iResolution.x-1.) { dir.x = -1.; boundary = true; }
  if (iFrame > 2 && boundary)
  {
    d = texture(iChannel0, q+e.xy*dir).x;
    gl_FragColor = vec4(d, c.x, 0, 0);
    return;
  }

  float p11 = c.y;

  float p10 = texture(iChannel0, q-e.zy).x;
  float p01 = texture(iChannel0, q-e.xz).x;
  float p21 = texture(iChannel0, q+e.xz).x;
  float p12 = texture(iChannel0, q+e.zy).x;

  float t = iTime + 30.;
  float scale = 64.;
  float dist = cos(t*.03);
  float phase = t * .7 / (dist+.001);
  vec2 center = iResolution.xy*.5;
  vec2 dpos = vec2(cos(phase),sin(phase))*scale*dist;
  d = smoothstep(.7*scale,.5,length(center + dpos*.7 - gl_FragCoord.xy));
  d += smoothstep(.5*scale,.5,length(center - dpos*1.4 - gl_FragCoord.xy));

  d += -(p11-.5)*2. + (p10 + p01 + p21 + p12 - 2.);
  d *= .99;
  d *= float(iFrame>=2);
  d = d*.5 + .5;

  gl_FragColor = vec4(d, c.x, 0, 0);
}
