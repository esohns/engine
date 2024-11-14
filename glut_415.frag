//precision highp float;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec3 u_mouse;

#define mouse vec3(u_mouse.xy/R,u_mouse.z)
#define FC gl_FragCoord.xy
#define R u_resolution
#define T u_time
#define N normalize
#define S smoothstep
#define MN min(R.x,R.y)
#define rot(a) mat2(cos((a)-vec4(0,11,33,0)))
#define hue(a) (.25+.6*cos(6.3*(a)+vec3(0,83,21)))
#define lum(a) dot(vec3(.2126,.7152,.0722),a)
#define anim pow(S(.0,1.,sin(T)),2.)

float
tor (vec3 p, vec2 s)
{
  vec2 c = vec2 (length (p.xy) - s.x, p.z);
  return length (c) - s.y;
}

float
map (vec3 p)
{
  float d = mix (length (p) - (1. - sin (T * 10. + 40. * dot (p.xz, p.yz)) * .02), tor (p, vec2 ((1. - sin (T * 10. + 40. * atan (p.x, p.y)) * .02), .5)), anim);
  return d * .2;
}

vec3
norm (vec3 p)
{
  float h = 1e-3; vec2 k = vec2 (-1, 1);
  return N (k.xyy * map (p + k.xyy * h) +
            k.yxy * map (p + k.yxy * h) +
            k.yyx * map (p + k.yyx * h) +
            k.xxx * map (p + k.xxx * h));
}

void
cam (inout vec3 p)
{
  if (mouse.z > .0)
  {
    p.yz *= rot (-mouse.y * 6.2 + 3.14);
    p.xz *= rot (3.14 - mouse.x * 6.2);
  }
  else
    p.xz *= rot (-T * .731);
}

float
shadow (vec3 p, vec3 rd)
{
  float shd = 1., maxd = length (rd - p);
  vec3 l = N (rd - p);
  for (float i = 1e-3; i < maxd;)
  {
    float d = map (p + l * i);
    if (d < 1e-3)
    {
      shd = .0;
      break;
    }
    shd = min (shd, 24. * i / d);
    i += d;
  }

  return shd;
}

void
main ()
{
  vec2 uv = (FC - .5 * R) / MN;
  vec3 col = vec3 (0), p = vec3 (0, 0, -5), rd = N (vec3 (uv, 1.3)), lp = vec3 (-10, 10, -10);
  cam (p); cam (rd); cam (lp);
  float dd = .0, at = .0;
  for (int i = 0; i++ < 400;)
  {
    float d = map (p);
    if (abs (d) < 1e-3)
    {
      vec3 n = norm (p), l = N (lp - p);
      float shd = shadow (p + n * 3e-3, lp);
      col += (.2 + shd * max (.0, dot (l, n))) * mix (hue (T * 1. + dot (p.xz, p.yz) * 4.), hue (T * .157 + atan (p.x, p.y) * 4./ 6.28318), anim);
      col += pow (clamp (dot (reflect (rd, n), l) * .5 + .5, .0, 1.), 63.);
      col /= 1. + exp (-col * col);
      col += lum (col);
      col = sqrt (col);
      at *= 4.;
      col += at * at;
      break;
    }
    if (d > 3.)
    {
      col = step (max (abs (uv.x) - 1., abs (uv.y) - .125), .0) * mix (vec3 (.1, .2, .3), vec3 (.01, .02, .03), 1.25 * length (uv));
      break;
    }

    p += rd * d;
    dd += d;
    at += .05 * (.05 / dd);
  }

  gl_FragColor = vec4 (col, 1.);
}
