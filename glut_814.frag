uniform vec2 iResolution;
uniform float iTime;

#define CORRECT_STREAMLINES

#define saturate(x) clamp(x, 0., 1.)

const float PI = 3.1419;

float rand (float n) { return fract(sin(n) * 43758.5453123); }

vec2
 force (vec2 p, vec2 pole)
{
  p -= pole;
  return p / dot(p,p);
}

float
calcVelocity (vec2 p)
{
  vec2 velocity = vec2(0);
  vec2 pole;
  vec2 f;
  float o, r, m;
  float flip = 1.;
  float j = 0.;
  const float limit = 15.;
  for (float i = 0.; i < limit; i++)
  {
    r = rand(i / limit) - .5;
    m = rand(i + 1.) - .5;
    m *= (iTime+(23.78 * 1000.)) * 2.;
    o = i + r + m;
    pole = vec2(sin(o / limit * PI * 2.),
                cos(o / limit * PI * 2.));
    f = force(p, pole);
    flip *= -1.;
    velocity -= f * flip;
    j += atan(f.x, f.y) * flip;
  }  
  velocity = normalize(velocity);
#ifdef CORRECT_STREAMLINES
  return j;
#endif
  return atan(velocity.x, velocity.y);
}

vec2
dir (float a)
{
  return vec2(sin(a), cos(a));
}

float
calcDerivitive (float a, vec2 p)
{
  vec2 v = dir(a);
  float n = 2. / iResolution.x;
  float d = 0.;
  d += length(v - dir(calcVelocity(p + vec2(0,n))));
  d += length(v - dir(calcVelocity(p + vec2(n,0))));
  d += length(v - dir(calcVelocity(p + vec2(n,n))));
  d += length(v - dir(calcVelocity(p + vec2(n,-n))));
  d /= 4.;
  return d;
}

float spacing = 1./30.;

void
main ()
{
  vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.x;
  p *= 3.;
  float a = calcVelocity(p);
  float deriv = calcDerivitive(a, p);
  a /= PI * 2.;

  float lines = fract(a / spacing);
  lines = min(lines, 1. - lines) * 2.;
  lines /= deriv / spacing;
  lines -= iResolution.x * .0005;
  lines = saturate(lines);

  float disc = length(p) - 1.;
  disc /= fwidth(disc);
  disc = saturate(disc);
  lines = mix(1. - lines, lines, disc);
  lines = pow(lines, 1./2.2);

  gl_FragColor = vec4(lines);
}
