uniform vec2 iResolution;
uniform float iTime;

#define RESOLUTION  iResolution
#define TIME        iTime
#define PI          3.141592654
#define PI_2        (0.5*PI)
#define TAU         (2.0*PI)
#define ROT(a)      mat2(cos(a), sin(a), -sin(a), cos(a))
#define BPM         (157.0/4.0)
#define PCOS(a)     0.5*(cos(a)+1.0)

const float planeDist = 1.0-0.80;
#define     furthest 16
#define     fadeFrom max(furthest-4, 0)
#define     fadeDist planeDist*float(furthest - fadeFrom)

const float overSample  = 4.0;
const float ringDistance= 0.075*overSample/4.0;
const float noOfRings   = 20.0*4.0/overSample;
const float glowFactor  = 0.05;

vec4
alphaBlend (vec4 back, vec4 front)
{
  float w = front.w + back.w*(1.0-front.w);
  vec3 xyz = (front.xyz*front.w + back.xyz*back.w*(1.0-front.w))/w;
  return w > 0.0 ? vec4(xyz, w) : vec4(0.0);
}

vec3
alphaBlend (vec3 back, vec4 front)
{
  return mix(back, front.xyz, front.w);
}

float
tanh_approx (float x)
{
  //  return tanh(x);
  float x2 = x*x;
  return clamp(x*(27.0 + x2)/(27.0+9.0*x2), -1.0, 1.0);
}

float
hash (float co)
{
  return fract(sin(co*12.9898) * 13758.5453);
}

vec3
offset (float z)
{
  float a = z;
  vec2 p = -0.15*(vec2(cos(a), sin(a*sqrt(2.0))) + vec2(cos(a*sqrt(0.75)), sin(a*sqrt(0.5))));
  return vec3(p, z);
}

vec3
doffset (float z)
{
  float eps = 0.05;
  return 0.5*(offset(z + eps) - offset(z - eps))/(2.0*eps);
}

vec3
ddoffset (float z)
{
  float eps = 0.05;
  return 0.5*(doffset(z + eps) - doffset(z - eps))/(2.0*eps);
}

vec3
skyColor (vec3 ro, vec3 rd)
{
  return vec3(0.0);
}

float
mod1 (inout float p, float size)
{
  float halfsize = size*0.5;
  float c = floor((p + halfsize)/size);
  p = mod(p + halfsize, size) - halfsize;
  return c;
}

float
atan_approx (float y, float x)
{
  float cosatan2 = x / (abs(x) + abs(y));
  float t = PI_2 - cosatan2 * PI_2;
  return y < 0.0 ? -t : t;
}

vec2
toPolar (vec2 p)
{
  return vec2(length(p), atan_approx(p.y, p.x));
}

vec3
glow (vec2 pp, float h)
{
  float hh = fract(h*8677.0);
  float b = TAU*h+0.5*TIME*(hh > 0.5 ? 1.0 : -1.0);
  float a = pp.y+b;
  float d = max(abs(pp.x)-0.001, 0.00125);
  return 
    (   smoothstep(0.667*ringDistance, 0.2*ringDistance, d)
      * smoothstep(0.1, 1.0, cos(a))
      * glowFactor
      * ringDistance
      / d
    )
    * (cos(a+b+vec3(0,1,2))+vec3(1.0))
    ;
}

vec3
glowRings (vec2 p, float hh)
{
  vec2 pp = toPolar(p);
//  pp.y += TAU*hh;

  vec3 col = vec3(0.0);
  float h = 1.0;
  const float nr = 1.0/overSample;

  for (float i = 0.0; i < overSample; ++i)
  {
    vec2 ipp = pp;
    ipp.x -= ringDistance*(nr*i);
    float rn = mod1(ipp.x, ringDistance); 
    h = hash(rn+123.0*i);
    col += glow(ipp, h)*step(rn, noOfRings);
  }
  
  col += (0.01*vec3(1.0, 0.25, 0.0))/length(p);

  return col;
}

vec4
plane (vec3 ro, vec3 rd, vec3 pp, vec3 off, float aa, float n)
{
  float h = hash(n+123.4);

  vec3 hn;
  vec2 p = (pp-off*vec3(1.0, 1.0, 0.0)).xy;
  float l = length(p);
  float fade = smoothstep(0.1, 0.15, l);
  if (fade < 0.1) return vec4(0.0);
  vec4 col = vec4(0.0);

  col.xyz = glowRings(p*mix(0.5, 4.0, h), h);
  float i = max(max(col.x, col.y), col.z);

  col.w = (tanh_approx(0.5+max((i), 0.0))*fade);
  return col;
}

vec3
color (vec3 ww, vec3 uu, vec3 vv, vec3 ro, vec2 p)
{
  float lp = length(p);
  vec2 np = p + 1.0/RESOLUTION.xy;
  const float rdd_per   = 10.0;
  float rdd =  (1.75+0.75*pow(lp,1.5)*tanh_approx(lp+0.9*PCOS(rdd_per*p.x)*PCOS(rdd_per*p.y)));
//  float rdd = 2.0;

  vec3 rd = normalize(p.x*uu + p.y*vv + rdd*ww);
  vec3 nrd = normalize(np.x*uu + np.y*vv + rdd*ww);

  float nz = floor(ro.z / planeDist);

  vec3 skyCol = skyColor(ro, rd);

  vec4 acol = vec4(0.0);
  const float cutOff = 0.95;
  bool cutOut = false;

  float maxpd = 0.0;

  for (int i = 1; i <= furthest; ++i)
  {
    float pz = planeDist*nz + planeDist*float(i);

    float pd = (pz - ro.z)/rd.z;

    if (pd > 0.0 && acol.w < cutOff)
    {
      vec3 pp = ro + rd*pd;
      maxpd = pd;
      vec3 npp = ro + nrd*pd;

      float aa = 3.0*length(pp - npp);

      vec3 off = offset(pp.z);

      vec4 pcol = plane(ro, rd, pp, off, aa, nz+float(i));

      float nz = pp.z-ro.z;
      float fadeIn = smoothstep(planeDist*float(furthest), planeDist*float(fadeFrom), nz);
      float fadeOut = smoothstep(0.0, planeDist*0.1, nz);
      pcol.w *= fadeOut*fadeIn;
      pcol = clamp(pcol, 0.0, 1.0);

      acol = alphaBlend(pcol, acol);
    }
    else
    {
      cutOut = true;
      acol.w = acol.w > cutOff ? 1.0 : acol.w;
      break;
    }
  }

  vec3 col = alphaBlend(skyCol, acol);
//  col += cutOut ? vec3(1.0, -1.0, 0.0) : vec3(0.0);
  return col;
}

vec3
effect (vec2 p, vec2 q)
{
  float tm  = planeDist*TIME*BPM/60.0;
  vec3 ro   = offset(tm);
  vec3 dro  = doffset(tm);
  vec3 ddro = ddoffset(tm);

  vec3 ww = normalize(dro);
  vec3 uu = normalize(cross(normalize(vec3(0.0,1.0,0.0)+ddro), ww));
  vec3 vv = cross(ww, uu);

  vec3 col = color(ww, uu, vv, ro, p);

  col -= 0.075*vec3(2.0, 3.0, 1.0);
  col *= sqrt(2.0);
  col = clamp(col, 0.0, 1.0);
  col = sqrt(col);

  return col;
}

void
main ()
{
  vec2 q = gl_FragCoord.xy/RESOLUTION.xy;
  vec2 p = -1. + 2. * q;
  p.x *= RESOLUTION.x/RESOLUTION.y;

  vec3 col = effect(p, q);

  gl_FragColor = vec4(col, 1.0);
}
