uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define MDIST 350.0
#define STEPS 200.0
#define pi 3.1415926535
#define rot(a) mat2(cos(a),sin(a),-sin(a),cos(a))
#define pmod(p,x) (mod(p,x)-0.5*(x))

vec3
pal (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b*cos(2.*pi*(c*t+d));
}

float
h21 (vec2 a)
{
  return fract(sin(dot(a.xy,vec2(12.9898,78.233)))*43758.5453123);
}

float
h11 (float a)
{
  return fract(sin((a)*12.9898)*43758.5453123);
}

float
box (vec3 p, vec3 b)
{
  vec3 d = abs(p)-b;
  return max(d.x,max(d.y,d.z));
}

float
ebox (vec3 p, vec3 b)
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float
swave (float x, float a)
{
  return (sin(x*pi/3.-pi/2.)/sqrt(a*a+sin(x*pi/3.-pi/2.)*sin(x*pi/3.-pi/2.))+1./sqrt(a*a+1.))*0.5;
}

vec3 rdg = vec3(0);
float nsdf = 0.;
bool rnsdf = false;

vec2
blocks (vec3 p, vec3 scl, vec3 rd)
{
  float t = iTime;

  bvec3 isEdge = bvec3(true);
  vec3 dMin = vec3(-0.5) * scl;
  vec3 dMax = vec3(0.5) * scl;
  vec3 dMini = dMin;
  vec3 dMaxi = dMax;

  float id = 0.;
  float seed = floor(t/8.)+0.2;

  float ITERS = 5.;

  vec3 dim = dMax - dMin;
  for (float i = 0.; i < ITERS; i++)
  {
    vec3 divHash = vec3(0.49, 0.501, 0.511);
    vec3 divide = divHash * dim + dMin;
    dMax = mix( dMax, divide, step( p, divide ));
    dMin = mix( divide, dMin, step( p, divide ));

    float pad = 0.01;
    if(dMaxi.x>dMax.x+pad&&dMini.x<dMin.x-pad)isEdge.x=false;
    if(dMaxi.y>dMax.y+pad&&dMini.y<dMin.y-pad)isEdge.y=false;
    if(dMaxi.z>dMax.z+pad&&dMini.z<dMin.z-pad)isEdge.z=false;

    vec3 diff = mix( -divide, divide, step( p, divide));
    id = length(diff + 10.0);

    dim = dMax - dMin;
  }
  float volume = dim.x*dim.y*dim.z;
  vec3 center = (dMin + dMax)/2.0;
  float b = 0.;
  if(any(isEdge))
  {
    float expand = 1.05+0.45*(sin(length(center+100.0)*0.35+t*3.0*pi/3.)*0.5+0.5);
    if(isEdge.x)
    {
      dim.x+=abs(center.x*expand-center.x)*2.0;
      center.x*=expand;
    }
    if(isEdge.y)
    {
      dim.y+=abs(center.y*expand-center.y)*2.0;
      center.y*=expand;
    }
    if(isEdge.z)
    {
      dim.z+=abs(center.z*expand-center.z)*2.0;
      center.z*=expand;
    }
  }

  vec3 edgeAxis = mix(dMin, dMax, step(0.0, rd));
  vec3 dAxis = abs(p - edgeAxis) / (abs(rd) + 1E-4);
  float dEdge = min(dAxis.x,min(dAxis.y,dAxis.z));
  b= dEdge;
  vec3 d = abs(center);
  dim-=0.4;
  float a = ebox(p-center,dim*0.5)-0.2;

  nsdf = a;
  a = min(a, b);

  id = h11(id)*1000.0;

  return vec2(a,id);
}

vec3
map (vec3 p)
{
  float t = iTime;

  vec3 po = p;
  vec2 a = vec2(1);

  vec3 scl = vec3(15,15,15);
  vec3 rd2 = rdg;
  p.yz*=rot(t*0.5*pi/3.);
  rd2.yz*=rot(t*0.5*pi/3.);
  p.xy*=rot(t*0.5*pi/3.);
  rd2.xy*=rot(t*0.5*pi/3.);
  a = blocks(p,scl,rd2)+0.01;
  a.x = max(box(p,vec3(scl*1.3)),a.x);

  return vec3(a,nsdf);
}

vec3
norm (vec3 p)
{
  vec2 e = vec2(0.01,0.);
  return normalize(map(p).x-vec3(
    map(p-e.xyy).x,
    map(p-e.yxy).x,
    map(p-e.yyx).x));
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy-0.5*iResolution.xy)/iResolution.y;
  float t = iTime;
  vec3 col = vec3(0);
  vec3 ro = vec3(0,11.5,-30)*1.8;
  if(iMouse.z>0.)
  {
    ro.yz*=rot(2.0*(iMouse.y/iResolution.y-0.5));
    ro.zx*=rot(-7.0*(iMouse.x/iResolution.x-0.5));
  }
  // else ro.xz*=rot(t*0.3);
  ro.xz*=rot(-pi/4.);
  vec3 lk = vec3(0,0.,0);
  vec3 f = normalize(lk-ro);
  vec3 r = normalize(cross(vec3(0,1,0),f));
  vec3 rd = normalize(f*(0.99)+uv.x*r+uv.y*cross(f,r));    
  rdg = rd;
  vec3 p = ro;
  float dO = 0.;
  vec3 d = vec3(0);
  bool hit = false;
  for(float i = 0.; i<STEPS; i++)
  {
    p = ro+rd*dO;
    d = map(p);
    dO+=d.x*0.99;
    if(abs(d.x)<0.0001||i==STEPS-1.)
    {
      hit = true;
      break;
    }
    if(d.x>MDIST)
    {
      dO=MDIST;
      break;
    }
  }
  if(hit)
  {
    vec3 ld = normalize(vec3(0.5,0.9,-0.9));
    vec3 n = norm(p);
    vec3 r = reflect(rd,n);
    vec3 e = vec3(0.5);
    vec3 al = pal(fract(d.y+t*10.0/12.0)*0.6-0.2,e*1.5,e,e*2.0,vec3(0,0.33,0.66));
    col = al;

    float spec = length(sin(r*5.)*.5+.5)/sqrt(3.);
    float fres = 1.-abs(dot(rd,n))*0.9;

    float diff = length(sin(n*2.)*.5+.7)/sqrt(3.);

#define AO(a,n,p) smoothstep(-a,a,map(p+n*a).z)
    float ao = AO(0.3,n,p)*AO(.5,n,p)*AO(.9,n,p);
    col = al*diff+pow(spec,5.0)*fres*diff*vec3(1.000,0.878,0.792);
    col*=pow(ao,0.4);
  }
  vec3 bg = mix(vec3(0.345,0.780,0.988),vec3(0.424,0.059,0.925),length(uv));
  col = mix(col,bg,dO/MDIST);
  gl_FragColor = vec4(col,1.0);
}
