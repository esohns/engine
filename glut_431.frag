//precision highp float;

uniform vec2 resolution;
uniform float time;

#define FC gl_FragCoord.xy
#define R resolution
#define T ((3600.+time)*.8)
#define S smoothstep
#define SE(v,s) S(s+1./MN,s-1./MN,v)
#define MN min(R.x,R.y)
#define lum(a) vec3(dot(vec3(.21,.71,.07),a))
#define hue(a) (.25+.6*cos(3.14*clamp(a,2.8,3.6)+vec3(0,83,21)))
#define rot(a) mat2(cos((a)-vec4(0,11,33,0)))

float
rnd (vec2 p)
{
  p = fract (p * vec2 (12.9898, 78.233));
  p += dot (p, p + 34.56);

  return fract (p.x * p.y);
}

float
noise (vec2 p)
{
  vec2 i = floor (p),
       f = fract (p),
       u = f * f * (3. - 2. * f);
  float a = rnd (i),
        b = rnd (i + vec2 (1, 0)),
        c = rnd (i + vec2 (0, 1)),
        d = rnd (i + 1.);

  return mix (mix (a, b, u.x), mix (c, d, u.x), u.y);
}

float
fbm (vec2 p)
{
  float t = .0, a = 1.;
  mat2 m = mat2 (1.2, -.8, .8, 1.2);
  for (int i = 0; i < 6; i++)
  {
    t += a * noise (p);
    p = 2. * p * m;
    a *= .5;
  }

  return t;
}

vec2
smin (vec2 a, vec2 b, float k)
{
  vec2 h = clamp (.5 + .5 * (b-a) / k, .0, 1.);
  return mix (b, a, h) - k * h * (1. - h);
}

float
logo (vec2 uv)
{
  vec3 col = vec3 (0);
  uv *= 5.;
  vec2 p = vec2 (sin (T + uv.y), cos (T + uv.x)) * .75;
  return 1. - SE (cos (1.9 - sin (length (p) / (.55 + length (uv)))), .1);
}

float
placeLogo ()
{
  vec2 p = FC / R;
  p *= vec2 (R.x / R.y, 1);
  p -= vec2 (R.x / R.y, 0);
  p += vec2 (.08, -.075);
  return logo (p * 12.);
}

void
main ()
{
  vec2 uv = (FC - .5 * R) / MN, p = (FC / MN);
  vec3 col = vec3 (0);
  // move and squish the domain
  p += vec2 (2.75, -2.75);
  p *= rot (-1.5707 * 2.5);
  p = smin (p, -uv, 1.5);
  p *= rot (.39);
  p.x += .8;
  p *= .5;
  // warp the domain multiple times to generate
  // the wavy noise pattern
  // see: https://iquilezles.org/articles/warp/
  float k = fbm (p * noise (uv * .2 * fbm (p - T * .2) + T * .02) + .125 * fbm (T * .01 - p * 2. + .5 * fbm ( T * .03 - p * 3. + .5 * fbm (p * 6. - T * .2))));
  // colorize noise
  vec3 d = mix (vec3 (0), hue (2.8 + k), k);
  col += lum (d) + d;
  col /= 1. + exp (-col * col * col);
  col = pow (col, vec3 (.4545));
  // vignette
  p = (FC - .5 * R) / R;
  col = mix (vec3 (0, .05, .2), col, pow (S (.45, .0, dot (p * p, p * p)), 12.));
  // raise darkest value to dark gray
  col = max (col, .1);
  // add little animation to the lower right corner
  col = max (col, vec3 (.1, .2, .4) * placeLogo ());
  gl_FragColor = vec4 (col, 1);
}
