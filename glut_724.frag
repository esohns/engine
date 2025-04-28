uniform vec2 iResolution;
uniform float iTime;

#define FAR 30.
#define INFINITY 1e32
#define t iTime
#define FOV 60.0
#define FOG .4
#define PI 3.14159265

float glow = 0.;
int iterations = 0;
    
void
pR (inout vec2 p, float a)
{
  p = cos(a) * p + sin(a) * vec2(p.y, -p.x);
}

vec3
opU2 (vec3 d1, vec3 d2)
{
  return (d1.x < d2.x) ? d1 : d2;
}

float
vmax (vec3 v)
{
  return max(max(v.x, v.y), v.z);
}

float
pModPolar (inout vec2 p, float repetitions)
{
  float angle = 2. * PI / repetitions;
  float a = atan(p.y, p.x) + angle / 2.;
  float r = length(p);
  float c = floor(a / angle);
  a = mod(a, angle) - angle / 2.;
  p = vec2(cos(a), sin(a)) * r;
  if (abs(c) >= (repetitions / 2.))
    c = abs(c);
  return c;
}

float
fBox (vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return length(max(d, vec3(0))) + vmax(min(d, vec3(0)));
}

vec3
dF (vec3 p)
{
  p = p.yzx;

  vec3 obj = vec3(FAR, -1.0, 0.0),
       obj2 = obj;

  p.z += sin(t / 2.) * 3.;
  p.z *= sin(t) * 1.;

  pR(p.xy, -t * 2. + p.z);

  p.x += sin(p.z) / 2.;

  vec3 orgP = p;

  pModPolar(p.xy, 4.);

  p.x -= .9;

  obj = vec3(
        fBox(p, vec3(0.4, 0.7, 13.4)),
        2.0,
        0.0
    );

  p = orgP;

  obj2 = vec3(
        fBox(p, vec3(1.1, 1.1, 20.)),
        1.0,
        0.0
    );

  return opU2(obj, obj2);
}

float t_min = 0.001;
float t_max = 50.;
const int MAX_ITERATIONS = 40;

vec3
trace (vec3 ro, vec3 rd)
{
  vec3 mp = dF(ro);
  glow = 0.;
  float minDist = INFINITY;
  float function_sign = (mp.x < 0.) ? -1. : 1.;

  float h = .001;
  float t_ = 0.;
    
  for (int i = 0; i < 60; i++)
  {
    if (abs(h) < .001 || t_ > FAR)
      break;
    mp = dF(ro + rd * t_);
    minDist = min(minDist, mp.x * 9.);
    glow = pow(1. / minDist, 1.) / 4.;
    h = function_sign * mp.x;
        t_ += h;
  }
  mp.x = t_;
  return mp;
}

float
softShadow (vec3 ro, vec3 lp, float k)
{
  const int maxIterationsShad = 18;
  vec3 rd = (lp - ro);

  float shade = 2.0;
  float dist = .05;
  float end = max(length(rd), 0.001);
  float stepDist = end / float(maxIterationsShad);

  rd /= end;
  for (int i = 0; i < maxIterationsShad; i++)
  {
    float h = dF(ro + rd * dist).x;
    shade = min(shade, smoothstep(0.0, 1.0, k * h / dist));
    dist += min(h, stepDist * 2.);
    if (h < 0.001 || dist > end)
      break;
  }

  return min(max(shade, 0.1), 1.0);
}

#define E .1
vec3
getNormalHex (vec3 pos)
{
  float d = dF(pos).x;
  return normalize(
        vec3(
            dF(
                pos + vec3(E, 0, 0)).x - d,
                dF(pos + vec3(0, E, 0)).x - d,
                dF(pos + vec3(0, 0, E)).x - d
          )
      );
}

float
getAO (vec3 hitp, vec3 normal)
{
  float dist = 0.4;
  vec3 spos = hitp + normal * dist;
  float sdist = dF(spos).x;
  return clamp(sdist / dist, 0.4, 1.0);
}

vec3
getObjectColor (vec3 p, vec3 n, vec2 mat)
{
  vec3 col = vec3(1);
    
  if (mat.x == 2.0)
  {
    col = vec3(8., 3., 0.);
    col.r *= sin(t);
    col.g *= cos(t * .2);
  }
    
  return col;
}

vec3
doColor (vec3 sp, vec3 rd, vec3 sn, vec3 lp, vec2 mat)
{
  vec3 ld = lp - sp,
       objCol = getObjectColor(sp, sn, mat);

  float lDist = max(length(ld), 0.001);
  ld /= lDist;

  float atten = 1.0 / (1.0 + lDist * 0.25 + lDist * lDist * 0.05),
        diff = max(dot(sn, ld), 0.),
        spec = pow(max(dot(reflect(-ld, sn), -rd), 0.0), 7.0);

  vec3 H = normalize(normalize(ld) + rd);

  float F = 14.0,
        b = 1. - dot(rd, H),
        exponential = pow(b, 5.0),
        fresnel = exponential + F * (1. - exponential);

  return (objCol * (diff + 0.15) + vec3(1.) * spec * fresnel) * atten;
}

vec4
pixelColor (vec2 fragCoord)
{
  fragCoord *= tan(radians(FOV) / 2.0);
    
  vec3 light = vec3(0., 0., 5.),
       sceneColor = vec3(1.),
       vuv = vec3(0., 1., 0.), // up
       ro = vec3(0., 0., 8.2), // cam pos
       vrp = vec3(0., 0., 0.), // lAt
       vpn = normalize(vrp - ro),
       u = normalize(cross(vuv, vpn)),
       v = cross(vpn, u),
       vcv = (ro + vpn),
       scrCoord = (vcv + fragCoord.x * u * iResolution.x / iResolution.y + fragCoord.y * v),
       rd = normalize(scrCoord - ro),
       tr = trace(ro, rd),
       sn;

  ro += rd * tr.x;
  sn = getNormalHex(ro);

  float fog = smoothstep(FAR * FOG, 0., tr.x),
        sh = softShadow(ro, light, .9),
        ao = getAO(ro, sn);

  sceneColor += doColor(ro, rd, sn, light, tr.yz);

  if (tr.x < FAR)
  {
    sceneColor *= fog;
    sceneColor *= ao;
    sceneColor *= sh;
    sceneColor += pow(max(0., sn.y), 5.);
  }
  else
  {
    sceneColor *= 0.;
    sceneColor += pow(glow, 1.4) * .8;
  }

  return vec4(clamp(sceneColor, 0.0, 1.0), tr.x > 10. ? 0. : pow(abs(ro.z) * .5, 5.));
}

void
main ()
{
  gl_FragColor = pixelColor (gl_FragCoord.xy / iResolution.xy - .5);
}
