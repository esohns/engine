uniform vec2 iResolution;
uniform float iTime;

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);

  return fract((p3.xx+p3.yz)*p3.zy);
}

vec3
hash32 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yxz+33.33);

  return fract((p3.xxy+p3.yzz)*p3.zyx);
}

#define inf 99999.9
#define pi  3.14159265

vec2
fc (vec2 x)
{
  return hash22(x);
}

float
vrn (vec2 uv, inout vec3 cc)
{
  vec2 f  = floor(uv);
  float d  = inf;
  float d2 = inf;
  float td = 0.0;
  vec2 tc;
  vec3 col;
  vec2 nc;
  for (int i = -1; i < 2; ++i)
    for (int j = -1; j < 2; ++j)
    {
      vec2 c = (f+vec2(i,j));
      c = c + fc(c);
      td = distance(c,uv);
      if (td < d)
      {
        d2 = d;
        d = td;
        tc  = c;
        continue;
      }
      if (td < d2)
        d2 = td;
    }
  cc = hash32(vec2(tc))*0.4 + vec3(0.6);
  cc *=  1.0 - smoothstep(abs(d2 - d),0.00, 0.07 ); 

  return  1.0 - smoothstep(0.0,0.2, abs(d2 - d)); 
}

struct Ray
{
  vec3 origin;
  vec3 dir;
  float t;
};

vec4 sph = vec4(0.0,0.0,-4.0,1.0);
int
solveQ (inout Ray r, inout float mi, inout float ma)
{
  vec4 ce = sph;
  vec3 o = r.origin - ce.xyz;
  vec3 d = r.dir;
  float a = 1.0; float b = 2.0*dot(o,d); float c = dot(o,o) - ce.w * ce.w;
  float disc = b*b - 4.0*a*c;
  if (disc < 0.0)
    return 0;
  disc = sqrt(disc);
  mi = (-b - disc)*0.5/a;
  ma = (-b + disc)*0.5/a;

  return 1;
}

vec2
getuv (vec3 p, vec3 n)
{
  vec2 suv;
  suv.x =  atan(n.x, n.z) / (pi*2.0) + 0.5;
  suv.y =  0.5 + asin(n.y)/pi;

  return suv;
}

int
intsph (Ray ray, inout float intr, inout vec3 n,inout vec3 cc)
{
  float mi, ma;
  int a = solveQ(ray, mi,ma);
  if (a == 0)
    return 0;
  vec3 p;
  vec3 pmi = ray.origin + mi * ray.dir;
  vec3 pma = ray.origin + ma * ray.dir;
  p = pmi;
  n = normalize(p - sph.xyz);
  vec2 suv  = getuv(pmi,n) + vec2(iTime*0.1,0.0);
  float tmp = vrn(suv*20.0,cc);
  intr = mi;
  if (tmp <= 0.0)
  {
    p = pma;
    n   = normalize(-p + sph.xyz);
    suv = getuv(pma,n) + vec2(iTime*0.1,0.0);
    tmp = vrn(suv*20.0,cc);
    if (tmp <= 0.0)
    {
      intr = inf;
      return 0;
    }
    intr = ma;
  }

  return 1;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xx;
  uv      -= vec2(0.5,0.5*iResolution.y/iResolution.x);
  vec3 col = vec3(0.1,0.0,uv.y);  
  vec3 lp    = vec3(sph.xyz + vec3(3.,6.5,7.3));

  Ray ray;
  ray.origin = vec3(0.0,0.0,2.0);
  ray.dir    = normalize(vec3(uv.x,uv.y,-1.0));
  vec3 cc;
  vec3  n;
  vec3 p;
  float intr = inf;
  int a = intsph(ray,intr, n, cc);
  if (a != 0)
  {
    p     = ray.origin + intr*ray.dir;
    vec3  dir  = normalize(-p + lp);
    vec3  vdir = normalize(ray.origin - p);
    vec3  rdir = reflect(-dir, n);
    float spec = pow(max(dot(vdir, rdir), 0.0), 16.0);
    float diff = dot(n,dir); diff = max(diff,0.0) + 0.1;
    vec3 tmp =  cc*(spec+diff);
    col = vec3(tmp);
  }

  float intr1 = (-ray.origin.y - 1.2) / ray.dir.y;
  if (intr1 >= 0.0)
  {
    vec3 p2   = ray.origin + ray.dir * intr1;
    if (intr1 <= intr) 
    {
      Ray r; r.origin = lp; r.dir = normalize(p2-lp);
      float ii; vec3 nn; vec3 ccc;
      a = intsph(r,ii, nn, ccc);
      int pt = (int(floor(p2.x / 2.0)) + int(floor(p2.z / 2.0))) & 1;
      vec3 co = pt == 0 ? vec3(0.3) : vec3(0.5,0.3,0.2);
      if (a == 0)
        col = (exp(p2.z*0.06))*co * vec3(max(0.0,dot(normalize(lp-p) ,vec3(0.0,1.0,0.0))));
      else
        col = vec3(0.0);
    }
  }
  
  gl_FragColor = vec4(col,1.0);
}
