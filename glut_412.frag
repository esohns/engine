//precision mediump float;

uniform vec2 aspect;
uniform float time;

varying vec2 uv;

#define t time
#define nsin(v) (sin(v)*.5+.5)
#define PI 3.141592
#define TAU 2.*PI

mat2
rot2 (float a)
{
  return mat2 (cos (a), -sin (a), sin (a), cos (a));
}

vec3
circ (vec2 p, float s)
{
  float d = length (p) - s;
  return vec3 (p / d, d);
}

vec2
distort (vec2 p, float a, int b, int c, float d, vec2 o, float ro)
{
  float f = 1. / a;
  float r = length (p);
  float ph = b==0 ? atan (p.y, p.x) : (b==1 ? atan (r / f) : (b==2 ? atan (r / 2. * f) * 2. : b==3 ? r / f : (b==4 ? asin (r / (2. * f) * 2.) : asin (r / f))));
  float th = c==0 ? atan (p.y, p.x) : (c==1 ? atan (r / f) : (c==2 ? atan (r / 2. * f) * 2. : c==3 ? r / f : (c==4 ? asin (r / (2. * f) * 2.) : asin (r / f))));
  vec2 rd = vec2 (sin (th) * cos (ph), sin (th) * sin (ph));

  p += o;
  p *= rot2 (ro);

  return p + rd * d;
}

void
main ()
{
  vec2 p = (uv * 2. - 1.) * aspect;
  vec3 col = vec3 (.0);

  vec2 q = distort (p, .1 * TAU + .9 * TAU * nsin (t - uv.y), 0, 3, .1 + nsin (t) * .3, vec2 (0, sin (t)), t * -PI - p.x);
  vec3 a = circ (q, .4);
  vec3 as = smoothstep (0., fwidth (a.z), a - (sin (t - a.z * PI + length (q)) * PI));
  col -= nsin (as.x - a.z) * vec3 (a.x, .9, (as.x));
  col += nsin (as.y + a.z) * vec3 (1. - as.y, .2, nsin (t - a.z / TAU) * .5);

  q = distort (p, .1 * TAU + .2 * TAU * nsin (t - uv.y), 0, 1, .1 + nsin (t) * as.x, vec2 (sin (t), cos (t - p.x)), 0.);
  a = circ (q, .2);
  as = smoothstep (0., fwidth (a.z), a - (sin (t - a.z * PI + length (q)) * PI));
  col += nsin (as.x-a.z) * vec3 (a.x, .9, pow (a.z, as.x)) * as.z;

  col = pow (col, vec3 (.454545));

  gl_FragColor = vec4 (col, 1.);
}
