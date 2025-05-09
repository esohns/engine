uniform vec2 iResolution;
uniform float iTime;

#define PI 3.14159
#define A2B PI/360.
#define MaxIter 14

const float DRadius = 0.7, Width = 1.4, Gamma = 2.2;
const vec3 BackgroundColor = vec3(1.);
const vec3 CurveColor = vec3(0.);

float lambda, ca, sa, lscl;
float aaScale;
float Angle = 60.;
vec2 csa;

void
init ()
{
  Angle = 90. * 0.5 * (1. + sin(iTime + 0.1 * PI));
  float ang = A2B * Angle;
  ca = cos(ang), sa = sin(ang);
  csa = vec2(ca, -sa);
  lambda = 0.5 / (ca * ca);
  lscl = 2. / lambda;
}

float
d2hline (vec2 p)
{
  float t = max(-1., min(1., p.x));
  p.x -= t;
  return length(p);
}

float
DE (vec2 p)
{
  float d = 1., r = dot(p, p);
  for (int i = 0; i < MaxIter; i++)
  {
    p.x = abs(p.x);
    p.x -= 1. - lambda;
    float t = 2. * min(0., dot(p, csa));
    p -= csa * t;
    p.x -= lambda;
    p *= lscl;
    d *= lscl;
    p.x += 1.;
    r = dot(p, p);
  }
  return d2hline(p) / d;
}

float
coverageFunction (float t)
{
  return 1. - smoothstep(-1., 1., t);
	//t=clamp(t,-1.,1.); return (t*t*t*t-5.)*t*1./8.+0.5;
}

float
coverageLine (float d, float lineWidth, float pixsize)
{
  d = d * 1. / pixsize;
  float v1 = (d - 0.5 * lineWidth) / DRadius;
  float v2 = (d + 0.5 * lineWidth) / DRadius;
  return coverageFunction(v1) - coverageFunction(v2);
}

vec3
color (vec2 pos)
{
  float pixsize = dFdx(pos.x);
  float v = coverageLine(abs(DE(pos)), Width, pixsize);
  return pow(mix(pow(BackgroundColor, vec3(Gamma)), pow(CurveColor, vec3(Gamma)), v), vec3(1. / Gamma));
}

void
main ()
{
  const float scaleFactor = 2.1;
  vec2 uv = scaleFactor * (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;
  uv.y += 0.5;
  init();
  gl_FragColor = vec4(color(uv), 1.0);
}
