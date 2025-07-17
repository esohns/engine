#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

#define SPHERES

vec4 gObjD;
 
#define FAR  15.

vec3 GSCALE = vec3(1, 1, 1)/1.;

#define LIGHT_TYPE 0 

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

float
hash31 (vec3 f)
{
  uvec3 p = floatBitsToUint(f);
  p = 1664525U*((p >> 2U)^(p.yzx>>1U)^p.zxy);
  uint h32 = 1103515245U*(((p.x)^(p.y>>3U))^(p.z>>6U));

  uint n = h32^(h32 >> 16);
  return float(n & uint(0x7fffffffU))/float(0x7fffffff);
}

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max(n*n - .2, .001);
  n /= dot(n, vec3(1));

  vec3 tx = texture(tex, p.zy).xyz;
  vec3 ty = texture(tex, p.xz).xyz;
  vec3 tz = texture(tex, p.xy).xyz;

  return mat3(tx*tx, ty*ty, tz*tz)*n;
}

vec2
path (float z)
{
  float s = sin(z/9.);
  float c = cos(z/13.); 
  return vec2(s*c*8., (s + c)*4.); 
}

float
hm (vec3 p)
{
  vec2 pth = path(p.z);
  float h = (dot(sin(p - cos(p.yzx*2.)), vec3(1./6.)) + .5)*8. - 0.;

  h = max(h, -(length(p.xy - pth) - 6.));
  return h;
}

vec3
getCol (vec3 id)
{
  return .5 + .45*cos(6.2831*hash31(id + .113)/4. + vec3(0, 1, 2));
}

float
distLine (vec3 p, vec3 a, vec3 b)
{
  p -= a; b -= a;
  float h = clamp(dot(p, b)/dot(b, b), 0., 1.);
  return length(p - b*h);
}

float plane (vec3 p, vec3 n, float d) { return dot(p, n) + d; }

float
rayLine (vec3 ro, vec3 rd, vec3 p, vec3 n)
{
  float dn = dot(rd, n);
  return dn>0.? dot(p - ro, n)/dn : 1e8;   
} 

vec3 lp, ld;
vec3 gDir;
vec3 gRd;
float gCD;
vec3 gSc; 
vec3 gP;
vec4 gID;
vec3 gN;

vec3
getCell (inout vec3 p, inout vec3 sc)
{
  vec3 oP = p;

  vec3 ip = floor(p/sc) + .5;

  p -= ip*sc;

  vec3 n = abs(p); 
  n = step(n.yzx, n)*step(n.zxy, n)*sign(p);

  gN = n;

  ip += n*.5;
  p = oP - ip*sc;

  gP = p;

  return ip;
}

float
sdOctahedron (vec3 p, float h, float scl)
{
  scl /= 2.;
  float m2 = (h*h + scl*scl);

  p = abs(p);
  p.xz = (p.z>p.x) ? p.zx : p.xz;
  p.xz -= scl; 

  vec3 q = vec3( p.z, h*p.y - p.x*scl, h*p.x + p.y*scl);

  float s = max(-q.x, 0.);
  float t = clamp((q.y - q.x*scl)/(m2 + scl*scl), 0., 1.);

  float a = m2*(q.x + s)*(q.x + s) + q.y*q.y;
  float b = m2*(q.x + t*scl)*(q.x + t*scl) + (q.y - m2*t)*(q.y - m2*t);

  float d2 = max(-q.y, q.x*m2 + q.y*scl) < 0. ? 0. : min(a, b);

  return sqrt((d2 + q.z*q.z)/m2)*sign(max(q.z, -p.y));
}

int faceID;

float
map (vec3 q3)
{
  vec2 pth = path(q3.z);
  float fl = 1e5;
 
  vec3 sc = GSCALE;

  vec3 p = q3;

  vec3 p3 = getCell(p, sc); 

  vec3 id = p3;

  const vec3[4] nn = vec3[4](vec3(-1, 1, 0), vec3(1, 1, 0),
                             vec3(0, 1, -1), vec3(0, 1, 1));

  float t = 1e8;

  float obj = -1e5; 

  float smF = .0;//.08;

  vec3 svP = p;

  vec3 svRd = gRd;
  if(abs(gN.x)>.5){ p = p.yxz; svRd = svRd.yxz; }
  if(abs(gN.z)>.5){ p = p.xzy; svRd = svRd.xzy; }

  for(int i = 0; i<4; i++)
  {
    vec3 fR = nn[i];
    fR *= sc/4.;

    fR = dot(svRd, fR)<0.? -fR : fR;

    float ti = rayLine(p, svRd, fR, normalize(fR));

    t = min(ti, t);
  }

  faceID = 0;
  if(p.x + p.z>0.) faceID += 4;
  if(p.y>0.) faceID += 2;
  if(p.x - p.z>0.) faceID += 1;

  gCD = max(t, 0.) + .0001;

  float d = 1e5;

  float dFrame = 1e5;

  float h = hm(id*sc);

  if(h<4.)
  {
    obj = sdOctahedron(p, sc.x/2., sc.x);

    d = obj + .0001;

    float svD = d;

    vec3 ap = abs(p);
    float e1 = (dot(ap, vec3(0, 1, 0)));
    float e2 = (dot(ap, vec3(1, 0, -1)/sqrt(2.)));
    e2 = min(e2, (dot(ap, vec3(1, 0, 1)/sqrt(2.))));

    dFrame = min(abs(e1), abs(e2));

    float eW = .135*sc.x;

    float rnd = hash31(id + .01);

    float rnd2 = hash31(id + float(faceID)/8. + .03);

    if(rnd2<.35)
    {
      float hole = max(dFrame, -(dFrame - eW));
      d = max(d, (dFrame - eW));
      d = d + hole*.125;
    }
    else
      d = d + .01 - dFrame*.125;

    if(rnd2<.35) dFrame = min(dFrame, abs(dFrame - eW));
    dFrame = max(dFrame - .005, obj);
       
#ifdef SPHERES
    if(rnd<.15)
    {
      d = length(p) - sc.x/2./sqrt(2.) + .0015;
      dFrame = 1e5;
      faceID = 0;
    }
#endif
  }

  gSc = sc; 
  gP = p;
  gID = vec4(1e5, id);

  gObjD = vec4(fl, d, dFrame, 1e5);

  return min(fl, min(d, dFrame));
}

float
rayMarch (vec3 ro, vec3 rd)
{
  float d, t = 0.;
  vec2 dt = vec2(1e8, 0);

  gDir = step(0., rd) - .5;
  gRd = rd;

  const int iter = 160;
  int i = 0;

  for (i = 0; i<iter; i++)
  {
    d = map(ro + rd*t);

    if (d<dt.x) { dt = vec2(d, t); } 

    if (d<.001 || t>FAR)
      break;

    t += min(d*.9, gCD);
  }

  if (i == iter - 1)
    t = dt.y;

  return min(t, FAR);
}

float
softShadow (vec3 p, vec3 ld, float lDist, float k)
{
  float res = 1.;
  float t = 0.;

  gDir = step(0., ld) - .5;
  gRd = ld; 

  for (int i=0; i<64; i++)
  {
    float d = map(p + ld*t);
    res = min(res, k*d/t);
    if (d<0. || t>lDist) break;

    t += clamp(min(d*.9, gCD), .01, .25);
  }

  return clamp(res, 0., 1.);
}

vec3
normal (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.002, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += map(p + sgn*e)*sgn;
    sgn = -sgn;
    if((i&1)==1){ mp = mp.yzx; e = e.zxy; }
  }

  return normalize(mp);
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;
  for (int i = 0; i<5; i++)
  {
    float hr = float(i + 1)*.2/5.;        
    float d = map(p + n*hr);
    occ += (hr - d)*sca;
    sca *= .7;
  }

  return clamp(1. - occ, 0., 1.);
}

float lDist;

vec4
render (inout vec3 ro, inout vec3 rd, vec2 u)
{
  float t = rayMarch(ro, rd);

  vec3 svSc = gSc;
  vec3 svP = gP;
  vec4 svGID = gID;

  int objID = gObjD.x<gObjD.y && gObjD.x<gObjD.z? 0 : gObjD.y<gObjD.z? 1 : 2;

  float svFID = float(faceID);

  vec3 fog = vec3(0);
  vec3 col = fog;

  vec3 p = ro + rd*t;

  if (t < FAR)
  {
    vec3 n = normal(p);

#if LIGHT_TYPE == 0
    ld = lp - p;
    lDist = length(ld);
    ld /= lDist;
#endif

    float shd = softShadow(p + n*.0015, ld, lDist, 8.);
    float ao = calcAO(p, n);

    vec3 id = svGID.yzw;

    if(svP.x + svP.z>0.) svFID += 4.;
    if(svP.y>0.) svFID += 2.;
    if(svP.x - svP.z>0.) svFID += 1.;

    if(hash31(id + .01)<.15) svFID = 0.;

    vec3 oCol = getCol(id + svFID/8.);

    vec3 blCol = oCol;
    blCol = mix(blCol*sqrt(blCol)*12., blCol.yzx*10., smoothstep(.4, .6, u.y*.5 + .5));

    vec3 fCol = mix(oCol*sqrt(oCol)*1.2, oCol.yzx, smoothstep(.4, .6, u.y*.5 + .5));
    oCol = mix(fCol*.5, vec3(.5)*dot(oCol, vec3(.299, .587, .114)), .7);

    float blR = hash31(id + svFID/8. + .2);
    if(objID!=0)
      oCol = mix(oCol, blCol, smoothstep(.485, .497, sin(blR*6.2831853 + iTime/2.)*.5));

    if(objID==2) oCol *= .05;

    vec3 tx = tex3D(iChannel0, p/2., n);
    tx = smoothstep(0., .5, tx);
    oCol *= tx;

    float dif = max(dot(ld, n), 0.);
    dif = pow(dif, 1.8 + tx.x);

    float spe = pow(max(dot(reflect(ld, n), rd), 0.), 32.);

    float spR = pow(max(dot(normalize(ld - rd), n), 0.), 8.);

    vec3 rf = reflect(rd, n);
    vec3 rTx = texture(iChannel1, rf.xzy*vec3(1, 1, 1)).xyz; rTx *= rTx;
    oCol = oCol + oCol*spR*rTx.yzx*8.;

    float am = pow(length(sin((n)*3.14159/4.)*.5 + .5)/sqrt(3.), 2.)*1.5; 

    col = oCol*(am + vec3(1, 1, .5)*dif*shd + vec3(1, .6, .4)*spe*shd*4.)*ao;

    float rt = t/FAR;
    col *= 1./(1. + rt*.2); 
  }

  col = mix(col, fog, smoothstep(.3, .99, t/FAR));

  return vec4(col, t);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  vec2 svU = u;

  u *= 1. + dot(u, u);

  vec3 lk = vec3(0, -.6, iTime*2.);
  vec3 ro = lk + vec3(0, 0, -2.5);

#if LIGHT_TYPE == 0
  lp = ro + vec3(0, 0, 4);
  lp.xy += path(lp.z);
#else
  ld = normalize(vec3(0, 4, 4));
  lDist = FAR;
#endif

  lk.xy += path(lk.z);
  ro.xy += path(ro.z);

  vec3 ww = normalize(lk - ro);
  vec3 uu = normalize(cross(vec3(0, 1, 0), ww ));
  vec3 vv = cross(ww, uu);
  const float FOV = 3.14159/3.;
  vec3 rd = normalize(u.x*uu + u.y*vv + ww/FOV);

  rd.xy = rot2(path(lk.z).x/24.)*rd.xy; 

  if(iMouse.z>0.)
  {
    vec2 ms = -(iMouse.xy - iResolution.xy*.5)/iResolution.y;
    rd.yz *= rot2(ms.y*3.1459);  
    rd.xz *= rot2(ms.x*3.1459);  
  } 

  rd.xz *= rot2(-3.1459265/3.);
  rd.xy *= rot2(3.14159/4.);

  vec4 c4 = render(ro, rd, svU);

  gl_FragColor = vec4(max(c4.xyz, 0.), 1.);
}
