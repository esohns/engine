#version 130

uniform vec2 iResolution;
uniform float iTime;

#define VARIANT

#define TIME        iTime
#define RESOLUTION  iResolution
#define ROT(a)      mat2(cos(a), sin(a), -sin(a), cos(a))

const float PI = acos(-1.), TAU = 2.*PI;

mat2 g_rot;
float g_scale;

vec3
aces_approx (vec3 v)
{
  v = max(v, 0.0);
  v *= 0.6;
  float a = 2.51;
  float b = 0.03;
  float c = 2.43;
  float d = 0.59;
  float e = 0.14;
  return clamp((v*(a*v+b))/(v*(c*v+d)+e), 0.0, 1.0);
}

vec3
offset (float t)
{
  t*= 0.25;
  return 0.2*vec3(sin(TAU*t), sin(0.5*t*TAU), cos(TAU*t));
}

vec3
doffset (float t)
{
  const float dt = 0.01;
  return (offset(t+dt)-offset(t-dt))/(2.*dt);
}

float
pmin (float a, float b, float k)
{
  float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
  return mix(b, a, h) - k*h*(1.0-h);
}

float
pmax (float a, float b, float k)
{
  return -pmin(-a, -b, k);
}

vec3
palette (float a)
{
  return 1.+sin(vec3(0,1,2)+a);
}

float
apollonian (vec4 p, float s, float w, out float off)
{
  float scale = 1.0;

  for(int i=0; i<6 ;i++)
  {
    p        = -1.0 + 2.0*fract(0.5*p+0.5);
    float r2 = dot(p,p);
    float k  = s/r2;
    p       *= k;
    scale   *= k;
  }
  vec4 sp = p/scale;
  vec4 ap = abs(sp)-w;
  float d = pmax(ap.w, ap.y, w*10.);
#ifdef VARIANT
  off = length(sp.xz);
#else
  d = min(d, pmax(ap.x, ap.z, w*10.));
  off = length(sp.xy);
#endif
  return d;
}

float
df (vec3 p, float w, out float off)
{
  vec4 p4 = vec4(p, 0.1);
  p4.yw *= g_rot;
  p4.zw *= transpose(g_rot);
  return apollonian(p4, g_scale, w, off);
}

vec3
glowmarch (vec3 col, vec3 ro, vec3 rd, float tinit)
{
  float t = tinit;
  for (int i = 0; i < 60; ++i)
  {
    vec3 p = ro + rd*t;
    float off;
    float d = df(p, 6E-5+t*t*2E-3, off);
    vec3 gcol = 1E-9*(palette((log(off)))+5E-2)/max(d*d, 1E-8);
    col += gcol*smoothstep(0.5, 0., t);
    t += 0.5*max(d, 1E-4);
    if (t > 0.5) break;
  }
  return col;
}

vec3
render (vec3 col, vec3 ro, vec3 rd)
{
  col = glowmarch(col, ro, rd, 1E-2);
  return col;
}

vec3
effect (vec2 p, vec2 pp, vec2 q)
{
  float tm  = mod(TIME+50.,1600.0);
  g_scale = mix(1.85, 1.5, 0.5-0.5*cos(TAU*tm/1600.));
  g_rot = ROT(tm*TAU/800.0);
  tm *= 0.025;
  vec3 ro   = offset(tm);
  vec3 dro  = doffset(tm);
  vec3 ww = normalize(dro);
  vec3 uu = normalize(cross(vec3(0,1,0), ww));
  vec3 vv = cross(ww, uu);
  vec3 rd = normalize(-p.x*uu + p.y*vv + 2.*ww);

  vec3 col = vec3(0.0);
  col += 1E-1*palette(5.0+0.1*p.y)/max(1.125-q.y+0.1*p.x*p.x, 1E-1); 
  col = render(col, ro, rd); 
  col *= smoothstep(1.707, 0.707, length(pp));
  col -= vec3(2.0, 3.0, 1.0)*4E-2*(0.25+dot(pp,pp));
  col = aces_approx(col);
  col = sqrt(col);
  return col;
}

void
main ()
{
  vec2 q = gl_FragCoord.xy/RESOLUTION;
  vec2 p = -1. + 2. * q;
  vec2 pp = p;
  p.x *= RESOLUTION.x/RESOLUTION.y;
  vec3 col = effect(p, pp, q);
  gl_FragColor = vec4(col,1.0);
}
