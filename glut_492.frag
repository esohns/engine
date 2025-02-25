uniform float time;
uniform vec2 resolution;

#define FC gl_FragCoord.xy
#define R resolution
#define MN min(R.x,R.y)
#define T time
#define N normalize
#define S smoothstep
#define rot(a) mat2(cos((a)-vec4(0,11,33,0)))
#define hue(a) (vec3(.7,.5,.4)+vec3(.6,.7,.5)*cos(6.3*(a)+vec3(0,83,21)))
#define MAXD 48.

float
rnd (vec3 p)
{
  p=fract(p*vec3(12.9898,78.233,156.34));
  p+=dot(p,p+34.56);
  return fract(p.x*p.y*p.z);
}

float
box (vec3 p, vec3 s)
{
  p=abs(p)-s;
  return length(max(p,.0))+min(.0,max(max(p.x,p.y),p.z));
}

float
map (vec3 p)
{
  float k=rnd(floor((p+1.)*.2));
  p=mod(p+1.,5.)-2.5;
  p.xz*=rot(T*.2+k);
  float d=box(p,vec3(1));
  d*=.5;
  return d;
}

vec3
norm (vec3 p)
{
  float h=1e-3; vec2 k=vec2(-1,1);
  return N(
    k.xyy*map(p+k.xyy*h)+
    k.yxy*map(p+k.yxy*h)+
    k.yyx*map(p+k.yyx*h)+
    k.xxx*map(p+k.xxx*h)
  );
}

void
cam (inout vec3 p)
{
  p.yz*=rot(-.48);
  p.xz*=rot(T*.2+.42);
}

float
shadow (vec3 p, vec3 lp)
{
  float shd=1., maxd=length(lp-p);
  vec3 l=N(lp-p);
  for (float i=1e-3; i<maxd;) {
    float d=map(p+l*i);
    if (d<1e-3) {
      shd=.0;
      break;
    }
    shd=min(shd,128.*d/i);
    i+=d;
  }
  return shd;
}

vec2
raymarch (inout vec3 p, vec3 rd)
{
  float d=.0, dd=.0;
  for (float i=.0; i<400.; i++) {
    d=map(p);
    if (abs(d)<1e-3 || d>MAXD) break;
    p+=rd*d;
    dd+=d;
  }
  return vec2(d,dd);
}

vec3
render (inout vec3 p, vec3 rd, vec3 lp, out vec2 d, bool shd)
{
  vec3 col=vec3(0);
  d=raymarch(p,rd);
  if (d.x<MAXD) {
    vec3 n=norm(p), l=N(lp-p);
    float
    dif=clamp(dot(l,n),.0,1.);
    col+=.2+dif*(shd?shadow(p+n*5e-2,lp):1.);
    col*=hue(3.1415+rnd(floor((p+1.)*.2)));
  }
  return col;
}

void
main ()
{
  vec2 uv=(FC-.5*R)/MN, d, dz;
  vec3 col=vec3(0),
  p=vec3(0,0,-5),
  rd=N(vec3(uv,1));
  cam(p); cam(rd);
  p.z-=T;
  vec3 lp=p+vec3(-.3,.5,-3.1);
  col+=render(p,rd,lp,d,true);
  vec3 n=norm(p);
  p+=n*5e-2;
  float fres=pow(clamp(1.-dot(-rd,n),.0,1.),5.);
  col+=mix(.02,.28,fres)*render(p,reflect(rd,norm(p)),lp,dz,false);
  col=mix(vec3(0),col,exp(-2e-3*d.y*d.y));
  gl_FragColor=vec4(col,1);
}
