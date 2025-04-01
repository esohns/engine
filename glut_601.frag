#version 130

// glut_601_common.glsl
float gTime;

float
distLine (vec2 a, vec2 b)
{
  b = a - b;
  float h = clamp(dot(a, b) / dot(b, b), 0., 1.);
  return length(a - b*h);
}

vec2
hash22 (vec2 p, float repScale)
{
  p = mod(p, repScale);

  float n = sin(dot(p, vec2(113, 1)));

  return (fract(vec2(262144, 32768)*n) - .5)*2. - 1.;
}

vec2
hash22G (vec2 p, float repScale)
{
  p = mod(p, repScale);

  float n = sin(dot(p, vec2(113, 1)));

#ifdef STATIC
  return (fract(vec2(262144, 32768)*n) - .5)*2. - 1.; 
#else
  p = fract(vec2(262144, 32768)*n); 
  return sin( p*6.2831853 + gTime); 
#endif
}

float
gradN2D (vec2 f, float repScale)
{
  f *= repScale;

  const vec2 e = vec2(0, 1);

  vec2 p = floor(f);
  f -= p;

  vec2 w = f*f*(3. - 2.*f);

  float c = mix(mix(dot(hash22G(p + e.xx, repScale), f - e.xx), dot(hash22G(p + e.yx, repScale), f - e.yx), w.x),
                mix(dot(hash22G(p + e.xy, repScale), f - e.xy), dot(hash22G(p + e.yy, repScale), f - e.yy), w.x), w.y);

  return c*.5 + .5;
}

float
fBm (vec2 p, float repScale, float time)
{
  gTime = time;

  return gradN2D(p, repScale)*.57 + gradN2D(p, repScale*2.)*.28 + gradN2D(p, repScale*4.)*.15;
}
// glut_601_common.glsl

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;

  float c = fBm(uv, 6., iTime/1.5);
  float c2 = fBm(uv, 6.*2., 0.);

  gl_FragColor = vec4((c - .5)*6.2831*2., c, c2, 1.0);
}
