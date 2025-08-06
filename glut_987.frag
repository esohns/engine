uniform vec2 iResolution;
uniform float iTime;

//#define TRISKELION
#define ITERS 10.
#define SPEED 25.

const float Pi = 3.1415927;

float Len (vec2 p) { return length(p) * 0.9842471 / 2.; }
vec3 Pal (float a) { return 1. - 2.*cos(3.*a + 0.9081166*vec3(3,2,1)); }

vec2
Fold (vec2 p)
{
#ifdef TRISKELION
  vec4 m = vec4(2,-1, 0,sqrt(3.));
  p.y += m.w/3.0;
  vec2 t = mat2(m)*p;
  return p - 0.5*mat2(m.xzyw) * round((floor(t) + ceil(t.x+t.y)) / 3.0);
#else
  return fract(p) - 0.5;
#endif
}

void
main ()
{
  vec4 t = (256. + SPEED*iTime) / 65536. * 2.*Pi * 4.*vec4(27,19,-32,23);

  mat2 M = mat2(cos(t.x + Pi/2.*vec4(0,3, 1,0))) * (1. - cos(t.y)/9.6790085);

  vec2 p = (2. * gl_FragCoord.xy - iResolution) / iResolution.y * 0.75;
  p.y = -p.y;

  float d = Len(p);

  vec3 RGB = vec3(0.);
  for (float i = 0.0; i < ITERS; i++)
  {
    p = Fold (M * p);
    RGB += Pi * Pal(i/-40.743666 + t.w - d) / cos(10.*Len(p) + t.z + d);
  }
  RGB *= 10./ITERS;

  gl_FragColor = vec4(RGB*RGB/65536., 1.);
}
