#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform float iTimeDelta;
uniform int iFrame;
uniform vec4 iMouse;
uniform bvec4 iKeyboard;
uniform sampler2D iChannel0;

//#define doThuchetOctree

#define PI acos(-1.)

#define eps .002
#define iterRm 180
#define zFar 1500.
#define iterAO 2.
#define doShade
#define doAO
#define doSpec
#if 1
#define doNormals
#define doThuchetCoaster
#else
#define iterDfFractal 4.
#define cutY
#endif

#define THRUST 0.04*iTimeDelta
#define ROLL 1.0*iTimeDelta
#define ROTATE 4.0*iTimeDelta

#define TOO_CLOSE 0.01

#define limit 3.0

#define HASHSCALE3 vec3(.6531, .5563, .7498)

#define dot2(p) dot(p,p)

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * HASHSCALE3);
  p3 += dot(p3, p3.yzx + 19.19);

  return fract((p3.x + p3.y) * p3.z*15.3023+0.434);
}

#define checktree(k)                    \
for (j = 0.0; j < k; j++) {             \
    vec3 h = floor(r*exp2(j))*exp2(-j); \
    float rand = hash13(h+exp2(-j-1.0));\
    if (rand >= 0.5) {                  \
        break;                          \
    }                                   \
}

float
squarering (vec3 p)
{
  vec2 q = vec2(abs(length(p.xy)-0.5),abs(p.z));
  float len = length(max(q-0.1667,0.0))+min(max(q.x,q.y)-0.1667,0.0);

  return len;
}

float
truchet (vec3 p, int type)
{
  vec3 q = abs(p-0.5);

  vec3 q2 = vec3(min(min(q.x,q.y),q.z),0,max(max(q.x,q.y),q.z));
  q = min(q,q.yzx);
  q2.y = max(max(q.x,q.y),q.z);

  float inside = length(abs(q2.yz-vec2(0.0,0.5)))-0.1667;
  float inside2 = length(max(abs(q2.yz-vec2(0.0,0.5)),0.0))-0.1667;
  float outside = length(abs(q2.yz-vec2(0.5,0.5)))-0.3333;

  float len;
  if (type == 0)
    len = inside;
  else if (type == 1)
    len = -outside;
  else if (type == 2)
    len = inside2;
  else if (type == 3)
  {
    len = squarering(p.xyz-vec3(0,0,0.5));
    len = min(len, squarering(p.yzx-vec3(1,0,0.5)));
    len = min(len, squarering(p.zxy-vec3(1,1,0.5)));
  }

  return len;
}

float
mapOcreeeTruchet (vec3 p)
{
  vec3 fp;
  vec3 lp;
  float len;
  float i;
  float size = 1.0;

  vec3 r = p;
  float j;
  checktree(limit);
  i = j;

  fp = floor(p*exp2(i))*exp2(-i);

  lp = fract(p*exp2(i));

  int type = int(hash13((fp+exp2(-i-1.0))*vec3(0.93,0.89,1.23))*4.0);
  len = truchet(lp,type)*exp2(-i);
  while (i <= limit)
  {
    fp = floor(p*exp2(i))*exp2(-i);

    lp = fract(p*exp2(i));

    vec3 p2 = p*exp2(i);
    vec3 fp2 = floor(p2-0.5);
    for(int x = -0; x <= 1; x++)
      for(int y = -0; y <= 1; y++)
        for(int z = -0; z <= 1; z++)
        {
          r = (fp2+vec3(x,y,z))*exp2(-i);
          if (r != fp)
          {
            checktree(i);

            if (i==j)
            {
              vec3 q = abs(p2-fp2-vec3(x,y,z)-0.5);

              vec3 q2 = vec3(min(min(q.x,q.y),q.z),0,max(max(q.x,q.y),q.z));
              q = min(q,q.yzx);
              q2.y = max(max(q.x,q.y),q.z);

              float outside = length(abs(q2.yz-vec2(0.5,0.5)))-0.3333;

              len = max(-outside*exp2(-i),len);
            }
          }
        }
    size *= 0.5;
    len *= -1.0;
    i++;
  }

  return len;
}

#define sat(a) clamp(a,.0,1.)

float isInside (vec2 p, vec2 c) { vec2 d = abs(p-.5-c)-.5; return -max(d.x,d.y); }

vec4 load(int re) { return texture(iChannel0, (.5 + vec2(re, .0)) / vec2(textureSize(iChannel0, 0)), -100.); }
void store (int re, vec4 va, inout vec4 o, vec2 u) { if(isInside(u,vec2(re,0))>.0) o=va; }

vec4 qid () {return vec4(0.0,0.0,0.0,1.0); }
vec4 qmulq (vec4 q1, vec4 q2) { return vec4(q1.xyz*q2.w+q2.xyz*q1.w+cross(q1.xyz,q2.xyz),(q1.w*q2.w)-dot(q1.xyz,q2.xyz)); }
vec4 aa2q (vec3 axis, float angle) { return vec4(normalize(axis)*sin(angle*0.5),cos(angle*0.5)); }
vec4 qinv (vec4 q) { return vec4(-q.xyz,q.w)/dot(q,q); }
vec3 qmulv (vec4 q, vec3 p) { return qmulq(q,qmulq(vec4(p,.0),qinv(q))).xyz; }
vec4 qpyr (vec3 o) { o*=0.5; vec3 s=sin(o), c=cos(o); return vec4(s.x*c.y*c.z+s.y*c.x*s.z,s.y*c.x*c.z-s.x*c.y*s.z,s.x*s.y*c.z+s.z*c.x*c.y, c.x*c.y*c.z-s.x*s.y*s.z); }

vec3 Tile (vec3 p) { vec3 a=vec3(8.0); return abs(mod(p,a)-a*0.5)-a*0.25; }
float DERect (vec4 z, vec3 r) { return length(max(abs(z.xyz)-r,0.0))/z.w; }

const float mr=0.5, mxr=0.975, scale = 2.52;
const vec3 rc=vec3(3.31,2.79,4.11),rcL=vec3(2.24,1.88,2.84);
const vec4 p0=vec4(4.0,0.0,-4.0,1.0);

float
torus (vec3 p,vec2 r)
{
  return length(vec2(abs(abs(length(p.xy)-r.x)-.1),abs(p.z)-.1))-r.y;
}

float
square (vec3 p,vec2 r)
{
  return length(vec2(abs(max(abs(p.x),abs(p.y))-r.x),p.z))-r.y;
}

const vec3 sunDir=normalize(vec3(0.7,1.0,-0.7))
          ,sunColor=vec3(1.0,0.99,.9)
          ,skyColor=vec3(.25,.26,.27);

vec3
Backdrop (vec3 rd)
{
  vec3 a = skyColor+.05*(rd+sin(rd.yzx*5.+2.4*sin(rd.zxy*3.0)))+sunColor*(max(0.0,dot(rd,sunDir))*0.2+pow(max(0.0,dot(rd,sunDir)),256.));
  return .7*a;
}

#define HASHSCALE1 .1031
#define HASHSCALE4 vec4(1031, .1030, .0973, .1099)
vec3 hash33 (vec3 p) { p=fract(p*HASHSCALE3); p+=dot(p,p.yxz+19.19); return fract((p.xxy+p.yxx)*p.zyx); }
float h12 (vec2 p)
{
  return fract(float(iFrame)+sin(dot(p,vec2(13.3145,17.7391)))*317.7654321);
}

#define u2(a) ((a)*2.-1.)
float
thuchetCoaster (vec3 p)
{
  vec3 p2 = mod(p,2.)-1.;
  vec3 floorpos=floor(p*.5);
  float len=1e10;
  vec3 orientation = u2(floor(hash33(floorpos)+0.5));
  vec3 p3 = p2*orientation;
  mat3 truchet=mat3(vec3(p3.yz*vec2(1, 1)+vec2(-1,-1),p3.x),vec3(p3.zx*vec2(1, 1)+vec2( 1, 1),p3.y),vec3(p3.yx*vec2(1,-1)+vec2( 1, 1),p3.z));
  vec3 lens=vec3(torus(truchet[0],vec2(1.0,0.02)),torus(truchet[1],vec2(1.0,0.02)),torus(truchet[2],vec2(1.0,0.02)));
  vec3 mask = vec3(lessThan(lens,min(lens.yzx,lens.zxy)));
  vec3 p4 = truchet[int(dot(mask,vec3(0,1,2)))];
  float dir = u2(mod(dot(floorpos,vec3(1.0)),2.0));
  p4=vec3(fract(dir*(atan(p4.x,p4.y)/6.28*4.0)+iTime*0.5)-0.5,length(p4.xy)-1.0,p4.z);
  
  return min(dot(lens,mask),length(p4)-0.1);
}

float
gd (vec3 u)
{
#ifdef doThuchetOctree
  u-=1.;
  return -mapOcreeeTruchet(u*.1);
#endif
#ifdef doThuchetCoaster
  return thuchetCoaster(u);
#else
  vec3 p=Tile(u);
  vec4 z=vec4(p,1);
  float dG=1e3;
  for(float n=.0;n<iterDfFractal;n++)
  {
    z.xyz=clamp(z.xyz,-1.,1.)*2.-z.xyz;
    z*=scale/clamp(max(dot(z.xy,z.xy),max(dot(z.xz,z.xz),dot(z.yz,z.yz))),mr,mxr);
    z+=p0;
    if(n==2.)
      dG=DERect(z,rcL);
  }
  dG=min(dG,DERect(z,rc));
#ifdef cutY
  dG=max(dG,u.y);
#endif
  return dG;
#endif
}

vec3
normal (vec3 p, float l)
{
  vec2 e=vec2(.1,0);
  return normalize(vec3(l-gd(p-e.xyy),l-gd(p-e.yxy),l-gd(p-e.yyx)));
}

float
gd1 (vec3 p,inout vec4 mcol)
{
  p=Tile(p);
  vec4 z=vec4(p,1);
  float dG=1e3;
  vec4 mc=vec4(0);
  for (float n=.0;n<5.;n++)
  {
    z.xyz=clamp(z.xyz, -1.0, 1.0) *2.0-z.xyz;
    z*=scale/clamp(max(dot(z.xy,z.xy),max(dot(z.xz,z.xz),dot(z.yz,z.yz))),mr,mxr);
    z+=p0;
    if(n==3.)
      mc=vec4(vec3(.5,.3,.2)+z.xyz*.05,.25);
    else if(n==2.)
      dG=DERect(z,rcL);
  }
  float ds=DERect(z,rc);
  if(dG<ds)
    mcol+=vec4(0.5,0.6,0.9,0.9)+vec4(z.xyz*0.025,0.0);
  else
  mcol+=mc;
  
  return min(dG,ds);
}

vec3
fog (float ts, vec3 c, vec3 f)
{
  float fog=min(pow(ts*20./zFar*.2,1.33)*.54,1.);
  
  return mix(c,f,fog);
}

float
AO (vec3 p, vec3 d, float s)
{
  float t=.0,a=1.,f,n=.01+.04*s;
  for(float i=.0;i<iterAO;i++)
  {
    f=max(gd(p+d*t)*1.5,n);
    a=min(a,f/t+t*0.5);
    t+=f;
  }
  
  return a;
}

#ifdef doShade
vec3
SpecOcc (vec3 r, vec3 d, vec3 N, vec3 p, vec3 o, float s, vec4 mcol)
{
  float l = dot(N, sunDir)
#ifdef doAO
      , shad = AO(p + N * .001, sunDir, s)
#else
      , shad=.5
#endif
;
#ifdef doSpec
  float v = dot(-d, N);vec3 cc=vec3(.6,.8,1)
       ,lc = vec3(1,.8,.6);
  float cd=exp(-distance(o,p));
  vec3 R=reflect(d,N);
  float spcl=pow(sat(dot(R,sunDir)),10.)
       ,spcc=pow(max(.0,dot(R,-d)),1.+cd)*.25;
  r=r*(cd*v*cc+shad*l*lc)+(cd*spcc*cc+shad*spcl*lc)*mcol.a
#endif
 ;
 r=sat(r);

 return r;
}
#endif

vec3
scene (vec3 o, vec3 d, float s)
{
  float g=gd(o)*s*.5,
        t=g,ao=1.,
        f=2./min(iResolution.x,iResolution.y);
  vec4 gs=vec4(1e6),ts=vec4(-1);
  for(int i=0;i<iterRm;i++)
  {
    g=gd(o+d*t);
    bool ps=all(lessThan(vec2(g,ts.w),vec2(min(f*t,ao),0)));
    if (ps)
    {
      gs=vec4(abs(g),gs.xyz);
      ts=vec4(t,ts.xyz);
    }
    t+=g;
    ao=g;
    if (t>zFar||g<eps)
      break;
  }
  
  if(g<f*t && g<gs.x)
  {
    gs.x=g;
    ts.x=t;
  }
  
  vec3 c=Backdrop(d);
  vec3 fcol=c;
  for (int i=0;i<4;i++)
  {
    if(ts.x<.0)
      break;
    float px=f*ts.x;
    vec3 so=o+d*ts.x;
    vec4 mcol = vec4(.0);
#define g3(a) vec3(gd1(so+a.xyy,mcol),gd1(so+a.yxy,mcol),gd1(so+a.yyx,mcol))
    vec3 ve=vec3(px,0,0);
    vec3 dp = g3(ve), dn = g3(-ve);
#ifndef doNormals 
    float d1=gd1(so,mcol);
    vec3 N=(dp-dn)/(length(dp-vec3(d1))+length(vec3(d1)-dn));
#else
    vec3 N = normal(so, eps) * .5 + .5;
#endif
    vec3 spc = mcol.rgb * 0.14;
#ifdef doShade
    spc = SpecOcc(spc, d, N, so, o, s, mcol);
#endif

#ifdef doFog
    spc=fog(ts.x,spc,fcol);
#endif  
    c=mix(spc,c,sat(gs.x/px));
    gs=gs.yzwx;
    ts=ts.yzwx;
  }

  return sat(c*2.);
}

vec4
mainImage2 (out vec4 fragColor, vec2 fragCoord, vec3 A, vec3 D)
{
  vec3 t=normalize(vec3((2.0*fragCoord-iResolution)/iResolution.y,1.0));
  vec3 o=load(0).xyz;
  vec4 fw=(load(1));
  t=qmulv(fw,t);
  float s=h12(fragCoord);
  return vec4(scene(o,t,s),1.0);
}

void
steer (out vec4 o, vec2 u)
{
  vec4 p,qrot;
  if(iFrame<2)
  {
    p=vec4(.1,0,0,.01);
    qrot=normalize(vec4(.1,.2,.3,.8));
  }
  else
  {
    p=load(0);
    qrot=load(1);
    vec3 fw=vec3(0,0,1);
    fw=qmulv(qrot,fw);
    vec3 newp=p.xyz+fw*p.w*.2;
    if(gd(newp)>TOO_CLOSE)
      p.xyz=newp;
    else
    {
      if (gd(vec3(p.xy,newp.z))>TOO_CLOSE)
        p.z=newp.z;
      if(gd(vec3(p.x,newp.y,p.z))>TOO_CLOSE)
        p.y=newp.y;
      if(gd(vec3(newp.x,p.yz))>TOO_CLOSE)
        p.x=newp.x;
    }
    p.w += THRUST * ((iKeyboard.x ? 1 : 0) - (iKeyboard.z ? 1 : 0));
    float roll = ROLL * ((iKeyboard.w ? 1 : 0) - (iKeyboard.y ? 1 : 0));
    vec2 m=vec2(0);
    if(iMouse.z>.0)
    {
      vec2 n=iMouse.xy-abs(iMouse.zw);
      n.x=-n.x;
      m.xy-=(n)/iResolution;
    }
#ifdef USE_AUTO_PILOT
    else
    {
      float d=gd(p.xyz);
      newp=qmulv(qrot,vec3(d,0,0));
      float d2=gd(p.xyz+newp);
      m.x=sign(d2-d)*ROTATE/(.1+2.*d*d);
    }
#endif
    vec4 qp=qpyr(vec3(m.yx*ROTATE,roll));
    qrot=normalize(qmulq(qrot,qp));
  }
  store(0, p, o, u);
  store(1, qrot, o, u);
}

void
mainImage3 (out vec4 o, vec2 u, vec3 A, vec3 D)
{
  if(all(lessThan(u,vec2(2.5,1.5))))
    steer(o,u);
  else
   o=mainImage2(o,u,A,D);
}

void
mainVR (out vec4 o, vec2 u, vec3 a, vec3 d)
{
  if (all(lessThan(u, vec2(2.5, 1.5))))
    steer(o, u);
  else
    mainImage3(o, u, a, d);
}

void
main ()
{
  vec4 O;
  vec2 U = gl_FragCoord.xy;

  const vec3 cameraPos=vec3(0);
  const float rad=360./acos(-1.), dar=acos(-1.)/180.;
  vec2 m=u2(iMouse.xy/iResolution);
  m=PI*mix(vec2(1.-m.x,m.y*.35),vec2(1,0),step(iMouse.z,.0));
  vec2 fov=vec2(25.);fov.x=atan(tan(fov.y/rad)*iResolution.x/iResolution.y)*rad;
  mat3 cam;
  cam[0]=vec3(cos(m.x),0,sin(m.x));
  m.x=m.x+PI*.5;
  cam[1]=vec3(cos(m.x)*cos(m.y),sin(m.y),sin(m.x)*cos(m.y));
  fov=(2.*U/iResolution-1.)*sin(fov*dar)/sin(PI*.5-fov*dar);
  cam[0]=cameraPos-cam[1]+fov.x*cam[0]-fov.y*cross(cam[0],cam[1]);
  cam[1]=normalize(cam[0]-cameraPos);
  cam[1]*=vec3(-1,1,-1);
  cam[0]*=vec3(-1,1,-1);
  mainVR (O,U,cam[0],cam[1]);

  gl_FragColor = O;
}
