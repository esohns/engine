#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform samplerCube iChannel0;
uniform sampler2D iChannel1;

#define MORE_PRESICE_IMAGE

// Constants ----------
#define PI 3.14159265358979
#define P2 6.28318530717959

const int   MAX_TRACE_STEP = 120;
const float MAX_TRACE_DIST = 50.;
const float TRACE_PRECISION = .001;
const float FUDGE_FACTOR = .82;
const vec3  GAMMA = vec3(1./2.2);

const float GI_LENGTH = 1.6;
const float GI_STRENGTH = 1.;
const float AO_STRENGTH = .4;
const int   SHADOW_MAX_TRACE_STEP = 4;
const float SHADOW_MAX_TRACE_DIST = 10.;
const float SHADOW_MIN_MARCHING = .4;
const float SHADOW_SHARPNESS = 1.;
const float CAUSTIC_STRENGTH = 1.6;
const float CAUSTIC_SHARPNESS = 4.;

struct Surface
{
  float d;
  vec3  kd, tc, rl, rr;
};
Surface near (Surface s, Surface t) { if(s.d<t.d)return s; return t; }

struct Ray
{
  vec3  org, dir, col;
  float len, stp, sgn;
};
Ray ray (vec3 o, vec3 d) { return Ray (o,d,vec3(1),0.,0.,1.); }
Ray ray (vec3 o, vec3 d, vec3 c, float s) { return Ray (o,d,c,0.,0.,s); }
vec3 _pos (Ray r) { return r.org + r.dir * r.len; }

struct Hit
{
  vec3 pos, nml;
  Ray ray;
  Surface srf;
};

struct Camera
{
  vec3 pos, tgt;
  float rol, fcs;
};
mat3
_mat3 (Camera c)
{
  vec3 w = normalize(c.pos-c.tgt);
  vec3 u = normalize(cross(w,vec3(sin(c.rol),cos(c.rol),0)));
  return mat3(u,normalize(cross(u,w)),w);
}

struct AABB { vec3 bmin, bmax; };
struct DLight { vec3 dir, col; };
vec3 _lit (vec3 n, DLight l) { return clamp((dot(n, l.dir)+1.)*.5,0.,1.)*l.col; }

const float bpm = 144.;
const DLight amb = DLight(vec3(0,1,0), vec3(.7));
const DLight dif = DLight(normalize(vec3(1,1,0)), vec3(1.));
float phase;

vec3  _rgb(vec3 hsv) { return ((clamp(abs(fract(hsv.x+vec3(0,2,1)/3.)*2.-1.)*3.-1.,0.,1.)-1.)*hsv.y+1.)*hsv.z; }
mat3  _smat(vec2 a) { float x=cos(a.y),y=cos(a.x),z=sin(a.y),w=sin(a.x); return mat3(y,w*z,-w*x,0,x,z,w,-y*z,y*x); }
float _checker(vec2 uv, vec2 csize) { return mod(floor(uv.x/csize.x)+floor(uv.y/csize.y),2.); }
float len2(vec3 v) { return dot(v,v); }
float smin(float a, float b, float k) { return -log(exp(-k*a)+exp(-k*b))/k; }
float smax(float a, float b, float k) { return log(exp(k*a)+exp(k*b))/k; }
float vmin(vec3 v) { return min(v.x, min(v.y, v.z)); }
float vmax(vec3 v) { return max(v.x, max(v.y, v.z)); }
vec2  cycl(float t, vec2 f, vec2 r) { return vec2(cos(t*f.x)*r.x+cos(t*f.y)*r.y,sin(t*f.x)*r.x+sin(t*f.y)*r.y); }
vec3  fresnel(vec3 f0, float dp) { return f0+(1.-f0)*pow(1.-abs(dp),5.); }
float rr2rl(float rr) { float v=(rr-1.)/(rr+1.); return v*v; }

float dfPlane(vec3 p, vec3 n, float d) { return dot(p,n) + d; }
float dfBox(vec3 p, vec3 b, float r) { return length(max(abs(p)-b,0.))-r;}
float dfHex(vec3 p, vec2 h, float r) { vec3 q=abs(p); q.x=max(q.x*0.866025+q.z*0.5,q.z); return length(max(q.xy-h.xy,0.))-r; }
vec3 repXZ(vec3 p, vec2 r){ vec2 hr=r*.5; return vec3(mod(p.x+hr.x, r.x)-hr.x, p.y, mod(p.z+hr.y, r.y)-hr.y); }

const vec4 ddp = vec4(-.8506508,.8506508,.5257311,0);
float
dfDdc (vec3 p, float r)
{
  float dp0=abs(dot(p,ddp.wyz)),dp1=abs(dot(p,ddp.wxz)),dp2=abs(dot(p,ddp.zwy)),
        dp3=abs(dot(p,ddp.zwx)),dp4=abs(dot(p,ddp.yzw)),dp5=abs(dot(p,ddp.xzw));
  return max(vmax(vec3(dp0,dp1,dp2)),vmax(vec3(dp3,dp4,dp5)))-r;
}

float
dfDdcr (vec3 p, float r, float e)
{
  float dp0=abs(dot(p,ddp.wyz)),dp1=abs(dot(p,ddp.wxz)),dp2=abs(dot(p,ddp.zwy)),
        dp3=abs(dot(p,ddp.zwx)),dp4=abs(dot(p,ddp.yzw)),dp5=abs(dot(p,ddp.xzw));
  return log(exp(dp0*e)+exp(dp1*e)+exp(dp2*e)+exp(dp3*e)+exp(dp4*e)+exp(dp5*e))/e-r;
}

Surface
map (vec3 p)
{
  vec2 ms = vec2(1.-exp(sin(phase/32.)*2.)/exp(2.), .7);
  if (iMouse.z > 0.) ms = iMouse.xy/iResolution.xy;
  mat3 m = _smat((ms*2.-1.)*PI);
  float ri = (ms.y<0.5)?1.6:(ms.y-.5)*3.+1.;
  float st = (ms.y>0.5)?.4:(.5-ms.y)*2.;
  vec3 col = _rgb(vec3(phase/256.,.4,_checker(p.xz,vec2(2.5))*.3+.4));
  vec3 dcl = _rgb(vec3(phase/256.+.33,.8,st));
  vec3 fcl = (textureLod(iChannel1,p.xz*.1,0.).rgb*.5+.3)*col;
  vec3 pos = vec3(2,2.4-max(0.,sin((phase+p.x)/64.*P2)),2);
  return near (
    Surface(dfPlane(p, vec3(0,1,0), 3.), fcl, vec3(0), vec3(.1), vec3(0)), 
    near (
      Surface(dfDdcr(repXZ(p,vec2(12))*m, 2.2, ms.x*10.+2.), dcl*.1, dcl*.9, vec3(rr2rl(ri)), vec3(ri)),
      Surface(dfDdc(repXZ(p+pos,vec2(4)), .5), vec3(0),dcl, vec3(rr2rl(ri)), vec3(ri))
    )
  );
}

vec3
calcNormal (vec3 p)
{
  vec3 v=vec3(.001,0,map(p).d);
  return normalize(vec3(map(p+v.xyy).d-v.z,map(p+v.yxy).d-v.z,map(p+v.yyx).d-v.z));
}

vec4
cs (vec3 pos, vec3 dir)
{
  vec4 col=vec4(0,0,0,1);
  float len=SHADOW_MIN_MARCHING;
  for( int i=0; i<SHADOW_MAX_TRACE_STEP; i++ )
  {
    Surface s = map(pos + dir*len);
    col.a   = min(col.a, SHADOW_SHARPNESS*s.d/len);
    col.rgb = s.tc;
    len += max(s.d, SHADOW_MIN_MARCHING);
    if (s.d<TRACE_PRECISION || len>SHADOW_MAX_TRACE_DIST) break;
  }
  col.a = clamp(col.a, 0., 1.);
  col.rgb = pow((1.-col.a), CAUSTIC_SHARPNESS) * col.rgb * CAUSTIC_STRENGTH;
  return col;
}

vec4
gi (vec3 p, vec3 n)
{
  vec4 col = vec4(0);
  for (int i=0; i<4; i++)
  {
    float hr = .01 + float(i) * GI_LENGTH / 4.;
    Surface s = map(n * hr + p);
    col += vec4(s.kd, 1.) * (hr - s.d);
  }
  col.rgb *= GI_STRENGTH / GI_LENGTH;
  col.w = clamp(1.-col.w * AO_STRENGTH / GI_LENGTH, 0., 1.);
  return col;
}

vec3
lighting (Hit h)
{
  if (h.ray.len > MAX_TRACE_DIST) return textureLod(iChannel0, h.ray.dir, 0.).rgb;
  vec4 fgi = gi(h.pos, h.nml);
  vec4 fcs = cs(h.pos, dif.dir);
  //   lin = ([Ambient]        + [Diffuse]        * [SS]  + [CAUSTICS])  * [AO]  + [GI]
  vec3 lin = (_lit(h.nml, amb) + _lit(h.nml, dif) * fcs.w + fcs.rgb) * fgi.w + fgi.rgb;
  return  h.srf.kd * lin;
}

Ray
rayScreen (vec2 p, Camera c)
{
  return ray(c.pos, normalize(_mat3(c) * vec3(p.xy, -c.fcs)));
}

Ray
rayReflect (Hit h, vec3 rl)
{
  return ray(h.pos + h.nml*.01, reflect(h.ray.dir, h.nml), h.ray.col*rl, h.ray.sgn);
}

Ray
rayRefract (Hit h, float rr)
{
  vec3 r = refract(h.ray.dir, h.nml, (h.ray.sgn>0.)?(1./rr):rr);
  if (len2(r)<.001) return ray(h.pos+h.nml*.01, reflect(h.ray.dir, h.nml), h.ray.col, h.ray.sgn);
  return ray(h.pos - h.nml*.01, r, h.ray.col*((h.ray.sgn>0.)?h.srf.tc:vec3(1)), -h.ray.sgn);
}

Hit
sphereTrace (Ray r)
{
  Surface s;
  for(int i=0; i<MAX_TRACE_STEP; i++)
  {
    s = map(_pos(r));
    s.d *= r.sgn;
    r.len += s.d * FUDGE_FACTOR;
    r.stp = float(i);
    if (s.d < TRACE_PRECISION || r.len > MAX_TRACE_DIST)
      break;
  }
  vec3 p = _pos(r);
  if (r.len > MAX_TRACE_DIST)
  {
    s.tc = s.rl = vec3(0);
    s.kd = vec3(1);
  }

  return Hit(p, calcNormal(p)*r.sgn, r, s);
}

Hit
trace (Ray r)
{
  return sphereTrace(r);
}

// D=Diffuse/Draw, R=Reflect, T=Transmit, *_REP=Repeating, CT=cheap trick for black pixels
#define ATT(h,a)    h.ray.col*=(1.-(a))
#define FRL(h)      fresnel(h.srf.rl,dot(h.ray.dir,h.nml))
#define PATH(lmax)  vec3 col=vec3(0),rl,c;Hit h[lmax];h[0]=trace(r);float l0=h[0].ray.len;
#define D(lv)       if (len2(h[lv].srf.kd)>=.001) {col+=lighting(h[lv])*h[lv].ray.col;ATT(h[lv],h[lv].srf.kd);}
#define R(lv)       h[lv+1]=h[lv];ATT(h[lv],rl=FRL(h[lv]));if(len2(h[lv+1].srf.rl)>=.001){h[lv+1]=trace(rayReflect(h[lv+1],rl));
#define T(lv)       h[lv+1]=h[lv];ATT(h[lv],h[lv].srf.tc); if(len2(h[lv+1].srf.tc)>=.001){h[lv+1]=trace(rayRefract(h[lv+1],h[lv+1].srf.rr.x));
#define R_REP(lv,c) h[lv+1]=h[lv];ATT(h[lv],rl=FRL(h[lv]));for(int i=0;i<c;i++){if(len2(h[lv+1].srf.rl)<.001)break;h[lv+1]=trace(rayReflect(h[lv+1],rl));
#define T_REP(lv,c) h[lv+1]=h[lv];ATT(h[lv],h[lv].srf.tc); for(int i=0;i<c;i++){if(len2(h[lv+1].srf.tc)<.001)break;h[lv+1]=trace(rayRefract(h[lv+1],h[lv+1].srf.rr.x));
#define CT(lv)      c=h[lv].ray.col;if(len2(c)>=.25)col+=textureLod(iChannel0,-h[lv].ray.dir,0.).rgb*c*c;
#define LEND        }
#define PATHEND     return vec4(col, l0);

vec4
render (Ray r)
{
  PATH(3)
  D(0)
#ifdef MORE_PRESICE_IMAGE
  T_REP(0,8)
    D(1)
  LEND
  R(0)
    D(1)
    T_REP(1,4)
      D(2)
    LEND
  LEND
#else
  T(0)
    D(1)
    T_REP(1,2)
      D(2)
    LEND
    CT(2)
  LEND
  R(0)
    D(1)
  LEND
#endif
  PATHEND
}

vec4
gamma (vec4 i)
{
  return vec4(pow(i.xyz, GAMMA), i.w);
}

void
main ()
{
  phase = iTime * bpm / 60. * P2;

  vec2   m = cycl(phase/512.*P2, vec2(3,7), vec2(10,3));
  Camera c = Camera(vec3(m.x,cos(phase/192.*P2)+4.,m.y), vec3(0), 0., 1.73205081);
  Ray    r = rayScreen((gl_FragCoord.xy * 2. - iResolution.xy) / iResolution.x, c);

  vec4 res = render(r);
  res.w = min(abs(res.w - length(c.pos))/100., 1.);

  gl_FragColor = res;
}
