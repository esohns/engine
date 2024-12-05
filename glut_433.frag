//precision highp float;

uniform vec2 resolution;
uniform float time;

#define FC gl_FragCoord.xy
#define R resolution
#define T (20.+time)
#define N normalize
#define S smoothstep
#define MN min(R.x,R.y)
#define rot(a) mat2(cos((a)-vec4(0,11,33,0)))

// Exponential smooth min for scalars
float
sminq (float a, float b, float k)
{
  k *= 4.;
  float h = max (k - abs (a - b), .0) / k;

  return min (a, b) - h * h * k * .25;
}

// Smooth min for scalars
// which can be used as smooth union
// by making b and k negative.
float
smin (float a, float b, float k)
{
  float h = clamp (.5 + .5 * (b - a) / k, .0, 1.);

  return mix (b, a, h) - k * h * (1. - h);
}

// Smooth min for 3D vectors
vec3
smin (vec3 a, vec3 b, float k)
{
  vec3 h = clamp (.5 + .5* (b - a) / k, .0, 1.);

  return mix (b, a, h) - k * h * (1. - h);
}

// Signed distance function of a box
float
box (vec3 p, vec3 s)
{
  p = abs (p) - s;

  return length (max (p, .0)) + min (.0, max (max (p.x, p.y), p.z));
}

// Returns a psydo-random value for a given 2D vector
float
rnd (vec2 p)
{
  p = fract (p * vec2 (12.9898, 78.233));
  p += dot (p, p + 34.56);

  return fract (p.x * p.y);
}

// A kaleidoscopic iterated function system
// generates points in a fractal set
vec3
kifs (vec3 p)
{
  float n = rnd (p.xz);
  for (int i=0; i<2; i++)
  {
    p.yz *= rot (T + rnd (vec2 (i)));
    p.xy *= rot (-T * .2);
    p.xz *= rot (T + rnd (vec2 (-i, i + 1)));
    p = smin (p, -p, -1.);
    p -= vec3 (1.8, .1, 2.);
  }

  return p;
}

// Defines the scene you look at
// using the kifs fractal to generate
// multiple objects despite defining
// it only once
vec2
map (vec3 p)
{
  vec3 q = kifs (p);
  float a = smin (box (q, vec3 (.5)) - .05, -(length (q) - .6), -.125),
        b = p.y + .5;
  
  // Adds a smooth gradient to the result
  // which can be used to smoothly transition
  // from the floor's color to the object's color
  float d = sminq (a, b, .25);
  
  return vec2 (d, S (.05, .3, a - b));
}

// Returns the normal vector of the 
// surface at the given point
#define ZERO min(T,.0)
vec3
norm (vec3 p)
{
  float h = 1e-3; vec2 k = vec2 (-1,1);

  return N (k.xyy * map (p + k.xyy * h).x +
            k.yxy * map (p + k.yxy * h).x +
            k.yyx * map (p + k.yyx * h).x +
            k.xxx * map (p + k.xxx * h).x);
}

// Returns the amount of shadow
// at the given point and the given
// light position
float
shadow (vec3 p, vec3 lp)
{
  float shd = 1., maxd = length (lp - p);
  vec3 l = N (lp - p);
  for (float i = 1e-3; i < maxd;)
  {
    float d = map (p + l * i).x;
    if (d<1e-3)
    {
      shd = .0;
      break;
    }
    shd = min (shd, 128. * d / i);
    i += d;
  }

  return shd;
}

// Rotates the camera position
// to a fixed angle
void
cam (vec3 p)
{
  p.yz *= rot (.42);
}

// Defines the entry function of the shader
void
main ()
{
  vec2 uv = (FC - .5 * R) / MN;
  vec3 col = vec3 (0),
  p = vec3 (0, 0, -7),
  rd = N (vec3 (uv, .2));
  cam (rd); cam (p);
  float dd = .0;
  for (int i = 0; i++ < 400;)
  {
    vec2 d = map (p);
    if (d.x < 1e-3)
    {
      vec3 n = norm (p),
      lp = vec3 (-1, 2, -3),
      l = N (lp - p);
      float dif = clamp (dot (l, n), .0, 1.),
            shd = shadow (p + n * 5e-2, lp),
            spe = clamp (dot (N (lp - rd), n), .0, 1.),
            rfl = .35 * pow (S (.0, 1., dot (reflect (rd, n), l)), 6.);
      col += .1 + dif * shd * spe + rfl;
      col *= 1.2 * mix (vec3 (1, .95, .8), vec3 (.1, .3, .25), d.y);
      break;
    }
    if (d.x > 5.)
      break;
    p += rd * d.x;
    dd += d.x;
  }
  // Adds some depth fog
  col = mix (vec3 (0), col, exp (-125e-5 * dd * dd * dd));
  col *= 1. - .01 * dot (p, p);
  col += sin (FC.x * 114.0) * sin (FC.y * 211.1) / 512.0;
  col = max (col, .04);
  gl_FragColor = vec4 (col,1);
}
