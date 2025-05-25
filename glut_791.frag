#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define SHOWGRID false
#define BEZIER_STEPS 8
#define GRID_SIZE 5.
#define SPLINE_WIDTH .1
#define SPLINE_COLOR vec3(1)
#define SPLINE_SOFTNESS .5
#define SHADOW_WIDTH 3.
#define CYCLE_VARIATIONS 0.

float g[64] = float[64] (
  0., 3., 1., 5., 2., 7., 4., 6.,
  0., 1., 2., 3., 4., 7., 5., 6.,
  0., 7., 1., 5., 2., 6., 3., 4.,
  0., 4., 1., 3., 2., 5., 6., 7.,
  0., 5., 1., 4., 2., 7., 3., 6.,
  0., 6., 1., 5., 2., 4., 3., 7.,
  0., 2., 1., 5., 3., 4., 6., 7.,
  0., 7., 2., 6., 3., 4., 5., 1.
);

float
N21 (vec2 p)
{
  p = fract(p*vec2(123.34,345.56));
  p += dot(p, p+34.23);
  return fract(p.x*p.y);
}

mat2
Rot (float a)
{
  float s = sin(a), c = cos(a);
  return mat2(c,-s,s,c);
}

float
dLineSeg (vec2 p, vec2 a, vec2 b)
{
  vec2 pa = p - a, ba = b - a;
  float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1.);	
  return length(pa - ba * h);
}

float
dBezier (vec2 p, vec4 a, vec4 b)
{
  vec2 p1=a.xy, p2=p1+a.zw, p3=b.xy+b.zw, p4=b.xy;

  float d = 100.;

  vec2 pb = p1;
  vec2 ppb;
    
  for(int i=0;i<BEZIER_STEPS;i++)
  {
    float t = float(i+1)/float(BEZIER_STEPS);

    ppb = pb;

    vec2 p12 = mix(p1, p2, t);
    vec2 p23 = mix(p2, p3, t);
    vec2 p34 = mix(p3, p4, t);

    vec2 p1223 = mix(p12, p23, t);
    vec2 p2334 = mix(p23, p34, t);

    pb = mix(p1223, p2334, t);

    d = min(d, dLineSeg(p, ppb, pb));
  }

  return d;
}

vec4
GetVec (float i, float s)
{
  i = mod(i, 8.);
  float a = 6.2832*i/(8./9.);
  vec2 pos = vec2(
    round(cos(a-.5*3.1415-.3)*2.),
    round(cos(a-.3)*2.)
  );

  i = floor(i/2.);

  float m2 = mod(i, 2.);
  vec2 tangent = vec2(
    i==1. ? -1. : m2,
    i==0. ? -1. : 1.-m2
  );
  //tangent *= 3.;//mix(.25, 2.5, fract(sin(s*345.23)*5434.));

  return vec4(pos.x, pos.y, tangent.x, tangent.y)*.25;
}

vec4
Spline (vec2 uv, float w, int k, float n, float s)
{
  float d = dBezier(uv, GetVec(g[k+0]+n, s+1.), GetVec(g[k+1]+n, s+2.));
  float m = smoothstep(w, w*SPLINE_SOFTNESS, d);

  vec3 col = m*SPLINE_COLOR;
  float shadow = max(m, smoothstep(w*SHADOW_WIDTH, 0., d));

  return vec4(col, shadow);
}

vec4
QuadTruchetTex (vec2 uv)
{
  vec2 gv = fract(uv)-.5;
  vec2 id = floor(uv);
  float n = N21(id);  

  float t = iTime*CYCLE_VARIATIONS;
  int k = int(floor(fract(n*3425.12+floor(t*3.))*8.))*8;
  float s = n*10.;
  n = floor(n*8.)+floor(t*2.);
  float w = SPLINE_WIDTH;

  vec4 s1 = Spline(gv, w, k, n, s);
  vec4 s2 = Spline(gv, w, k+2, n, s+2.);
  vec4 s3 = Spline(gv, w, k+4, n, s+4.);
  vec4 s4 = Spline(gv, w, k+6, n, s+6.);

  vec4 col = mix(s1, s2, s2.a);
  col = mix(col, s3, s3.a);
  col = mix(col, s4, s4.a);

  if(SHOWGRID && (gv.x>.48||gv.y>.48)) col.r = 1.;

  return col;
}

vec3
GetRayDir (vec2 uv, vec3 ro)
{
  vec3 f = normalize(vec3(0)-ro),
       r = normalize(cross(vec3(0,1,0), f)),
       u = cross(f, r),
       c = ro + f,
       i = c + uv.x*r + uv.y*u,
       rd = normalize(i-ro);
  return rd;
}

vec3
WorldToCube (vec3 p)
{
  vec3 ap = abs(p);
  vec3 sp = sign(p);
  float m = max(ap.x, max(ap.y, ap.z));
  vec3 st;
  if(m==ap.x)
    st = vec3(p.zy, 1.*sp.x);
  else if(m==ap.y)
    st = vec3(p.zx, 2.*sp.y);
  else
    st = vec3(p.xy, 3.*sp.z);
  st.xy /= m;
  // st.xy = atan(st.xy*tan(0.868734829276))/0.868734829276;

  //st.xy *= 3.0/(2.0+abs(st.xy));
  //st.xy *= 1.25 - 0.25*st.xy*st.xy;
  st.xy *= (1.45109572583 - 0.451095725826*abs(st.xy));

  return st;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy-.5*iResolution.xy)/iResolution.y;
  vec2 M = (iMouse.xy-.5*iResolution.xy)/iResolution.y;

  vec3 ro = vec3(0.,0.,-3.);
  ro.yz *= Rot(-M.y*3.+sin(iTime*.1));
  vec3 rd = GetRayDir(uv, ro);

  float dO=0., dS;
  vec3 p;
  for(int i=0; i<100; i++)
  {
    p = ro + rd * dO;
    dS = length(p)-1.;
    dS = min(dS, -(length(p)-4.));

    if(dS<.001)
      break;
    dO += dS;
  }

  vec4 col = vec4(0);
  if(dS<.001)
  {
    float d = length(p-ro);
    vec3 n = normalize(p);
    float fresnel = 1.+dot(rd, n);
    fresnel *= fresnel;
    fresnel *= fresnel;

    p.xz *= Rot(iTime*.1);
    vec3 cc = WorldToCube(p);
    col = QuadTruchetTex(cc.xy*GRID_SIZE+cc.z*10.);

    if(length(p)<1.2)
      col = mix(col, vec4(.5), fresnel);

    col /= max(1., d*.3);
  }

  col *= vec4(1.-dot(uv,uv));

  gl_FragColor = vec4(col);
}
