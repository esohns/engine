#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

#define PI 3.14159265358979
#define TAU 6.283185307179586
#define FAR 15.
#define GSCALE vec3(1, 1./12., 1)
// Gradient noise: 0, Sine noise: 1
#define NOISE_FUNC 0
// Light type: Point: 0, Direct: 1
#define LIGHT_TYPE 0
#define BEVEL
#define CAM_Z .25
//#define HOLES

float tm;

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max(n*n - .2, .001);
  n /= dot(n, vec3(1));

  vec3 tx = texture(tex, p.yz).xyz;
  vec3 ty = texture(tex, p.zx).xyz;
  vec3 tz = texture(tex, p.xy).xyz;

  return mat3(tx*tx, ty*ty, tz*tz)*n;
}

float
hash31 (vec3 f)
{
  uvec3 p = floatBitsToUint(f);
  p = 1664525U*((p >> 2U)^(p.yzx>>1U)^p.zxy);
  uint h32 = 1103515245U*(((p.x)^(p.y>>3U))^(p.z>>6U));

  uint n = h32^(h32 >> 16);
  return float(n & uint(0x7fffffffU))/float(0x7fffffff);
}

//#define STATIC
vec2
hash22G (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx)*vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx + 35.3873);
#ifdef STATIC
  return fract((p3.xx + p3.yz)*p3.zy)*2. - 1.;
#else
  p = fract((p3.xx + p3.yz)*p3.zy);
  return sin(p*TAU + tm);
#endif
}

#if NOISE_FUNC == 1
float
sinNoise (vec2 f)
{
  float d = dot(sin(f*2. + tm*.5 - cos(f.yx*2.8 - tm*.5)), vec2(.25)) + .5;
  f *= 2.;
  return mix(d, dot(sin(f*2. + tm*1. - cos(f.yx*2.8 - tm*1.)), vec2(.25)) + .5, 1./3.);
}
#else
float
gradN2D (vec2 f)
{
  const vec2 e = vec2(0, 1);

  vec2 p = floor(f);
  f -= p;

  vec2 w = f*f*f*(f*(f*6.-15.)+10.); 
  //vec2 w = f*f*(3. - 2.*f); // Cubic smoothing. 
  //vec2 w = f*f*f; w = ( 7. + (w - 7. ) * f ) * w; // Super smooth, but less practical.
  //vec2 w = .5 - .5*cos(f*PI); // Cosinusoidal smoothing.
  //vec2 w = f; // No smoothing. Gives a blocky appearance.

  float c = mix(mix(dot(hash22G(p + e.xx), f - e.xx), 
                    dot(hash22G(p + e.yx), f - e.yx), w.x),
                mix(dot(hash22G(p + e.xy), f - e.xy), 
                    dot(hash22G(p + e.yy), f - e.yy), w.x), w.y);
  return c*.5 + .5;
}
#endif

float
opExtrusion (float sdf, float pz, float h, float sf)
{
  vec2 w = vec2( sdf, abs(pz) - h) + sf;
  return min(max(w.x, w.y), 0.) + length(max(w, 0.)) - sf;   
}

float
dist2D (vec2 p, float iDepth)
{
#if NOISE_FUNC == 1
  tm = -iDepth*5.;

  float d = sinNoise(p*2.);
#else
  tm = -iDepth*3.;
    
  float d = gradN2D(p*2.);
#endif

#if NOISE_FUNC == 1
  d += iDepth*.85 - .36;
#else
  d += iDepth*.48 - .42;
#endif

  return d/2.;
}

float
rayLine (vec3 ro, vec3 rd, vec3 p, vec3 n)
{
  float dn = dot(rd, n);
  return dn>0.? dot(p - ro, n)/dn : 1e8;   
} 
 
vec4 vObjID;

vec3 gDir;
vec3 gRd;
float gCD;

vec3 gSc; 
vec3 gP;
vec4 gID;

vec3 glow;

float
map (vec3 q3)
{
  float fl = q3.y + 1.;

  vec3 sc = GSCALE;
  
  float ipy = (floor(q3.y/sc.y) + .5)*sc.y;
  float py = q3.y - ipy;

  float d2 = dist2D(q3.xz, ipy); 
  vec2 p = q3.xz;

  gCD = rayLine(vec3(q3.x, py, q3.y), gRd, gDir*vec3(0, sc.y, 0), gDir*vec3(0, 2, 0));
  gCD = max(gCD, 0.) + .0015;
 
#ifdef HOLES
  d2 = max(d2, -(d2 + .035 - ipy/16.));
#endif

#ifdef BEVEL
  float yH = py>0.? sc.y/2.*.66 : sc.y/2. - .0015;
#else
  float yH = sc.y/2. - .0015;
#endif

  float maxH = sc.y*2.;
  float d = ipy<maxH? opExtrusion(d2, py, yH, 0.) : 1e5;

#ifdef BEVEL
  if(py>0.)
  {
    d += d2*.25;
    d = max(d, abs(py) - sc.y/2. + .0015);
  }
#endif

  if(d<.25)
  {
    float dd = min(q3.y + sc.y*4., 0.);
    glow += max(-dd, 0.)*2.;    
  }

  gSc = sc;
  gP = vec3(p.x, py, p.y);
  gID = vec4(d2, vec3(0, ipy, 0));

  vObjID = vec4(fl, d, 1e5, 1e5);

  return min(fl, d);
}

float
rayMarch (vec3 ro, vec3 rd)
{
  float d, t = hash31(ro + rd)*.25;
  vec2 dt = vec2(1e8, 0);

  gDir = step(0., rd) - .5;
  gRd = rd;

  glow = vec3(0);

  const int iter = 160;
  int i = 0;
  for (i = 0; i<iter; i++)
  {
    d = map(ro + rd*t);

    if (abs(d)<.001 || t > FAR)
      break;

    t += min(d*.8, gCD);
  }

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

    t += clamp(min(d*.8, gCD), .01, .25);
  }

  return clamp(res, 0., 1.);
}

vec3
normal (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.0025, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += map(p + sgn*e)*sgn;
    sgn = -sgn;
    if((i&1)==1)
    {
      mp = mp.yzx;
      e = e.zxy;
    }
  }

  return normalize(mp);
}

vec2
hash23 (vec3 f)
{
  uvec3 p = floatBitsToUint(f);
  p = 1103515245U*((p >> 2U)^(p.yzx>>1U)^p.zxy);
  uint h32 = 1103515245U*(((p.x)^(p.y>>3U))^(p.z>>6U));

  uint n = h32^(h32>>16);

  uvec2 rz = uvec2(n, n*48271U);
  return vec2((rz>>1)&uvec2(0x7fffffffU))/float(0x7fffffff);
}

vec3
cosDir (vec3 p, vec3 n)
{
  vec2 rnd = hash23(p);
  float u = rnd.x;
  float v = rnd.y;

  float a = 6.2831853*v;
  u = 2.*u - 1.;
  return normalize(n + vec3(sqrt(1. - u*u)*vec2(cos(a), sin(a)), u));
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 3., occ = 0.;
  for(int i = 0; i<12; i++)
  {
    float hr = (float(i) + 1.)*.25/12.; 
    vec3 rn = cosDir(p + n*hr, n);
    float d = map(p + rn*hr);

    occ = occ + max(hr - d, 0.)*sca;
    sca *= .7;
  }

  return clamp(1. - occ, 0., 1.);    
}

vec2 path (float z) { float s = sin(z/4.)*cos(z/2.); return vec2(s*8., 0.); }

vec4
render (vec3 ro, vec3 rd, inout float svVal)
{
#if LIGHT_TYPE == 0
  vec3 lp = ro + vec3(1, 1.5, 3);
#else
  vec3 ld = normalize(vec3(1, 1.5, 3));
#endif

  float t = rayMarch(ro, rd);
  svVal = t;

  vec3 svSc = gSc;
  vec3 svP = gP;
  vec4 svGID = gID;

  vec3 svGlow = glow;

  float svTm = tm;

  int objID = vObjID.x<vObjID.y? 0 : 1;

  vec3 p = ro + rd*t;

  vec3 sky = mix(vec3(2, .8, .4), vec3(.4, .8, 2), smoothstep(0., 1., p.y + 1.));

  vec3 col = sky;
 
  if (t<FAR)
  {
    vec3 n = normal(p);

#if LIGHT_TYPE == 0
    vec3 ld = lp - p;
    float lDist = length(ld);
    ld /= lDist;
#else
    float lDist = FAR;//length(lp - p);
#endif

    float shd = softShadow(p + n*.0015, ld, lDist, 8.);
    float ao = calcAO(p, n);

    vec3 oCol = vec3(.1, .15, .3);

    vec3 sCol = mix(vec3(.4, .25, 1), vec3(1, .2, .4), clamp(1. - max(p.y + .25, 0.)*4., 0., 1.));

    oCol = mix(oCol, sCol*4., 1. - smoothstep(0., .003, svP.y - svSc.y/4.));

    tm = svTm;
    vec2 q = p.xz;
    float px = .001;
#if NOISE_FUNC == 1
    float d = sinNoise(q*2.);
    float d3X = sinNoise((q - vec2(px, 0))*2.); 
    float d3Y = sinNoise((q - vec2(0, px))*2.);        
#else
    float d = gradN2D(q*2.);
    float d3X = gradN2D((q - vec2(px, 0))*2.); 
    float d3Y = gradN2D((q - vec2(0, px))*2.);
#endif
    vec2 dX = (vec2(d3X, d3Y) - d)/px/2.;
    float dt = length(dX); 

    svGID.x /= dt;

    float ew = .004;
    float bord = abs(svGID.x + ew/2.) - ew;
    bord = max(bord, abs(abs(svP.y + ew) - svSc.y/2. + ew*2.) - ew);

    oCol = mix(oCol, (oCol*3. + .5), 1. - smoothstep(0., .003, bord - ew*2.));
    oCol = mix(oCol, oCol*.05, 1. - smoothstep(0., .003, bord));

    vec3 fillDir = vec3(-ld.xz, 0.);
    float bl = max(dot(fillDir, n), 0.);
    oCol = mix(oCol, oCol + sky*3., bl + .04);

    vec3 txP = vec3(p.x, p.y - svSc.y, p.z);
    vec3 tx = tex3D(iChannel0, txP*3. + svGID.z, n);
    txP.xz = rot2(PI/5.)*txP.xz;

    oCol *= tx*1.8 + .35;

#if LIGHT_TYPE == 0
    float atten = 1./(1. + lDist*.1);
#else
    float atten = 1./(1. + lDist*.03);
#endif

    float dif = max(dot(ld, n), 0.);
    dif = pow(dif, 4. + 4.*tx.x);

    float spe = pow(max(dot(reflect(ld, n), rd), 0.), 64.*tx.x + 8.);

    float speR = pow(max(dot(normalize(ld - rd), n), 0.), 8.);
    vec3 rf = reflect(rd, n);
    vec3 rTx = texture(iChannel1, rf).zxy; rTx *= rTx;
    oCol = oCol + oCol*speR*rTx*4.;

    float am = pow(length(sin(n)*.5 + .5)/sqrt(3.), 2.); 

    col = oCol*(am*(shd*.5 + .5) + dif*shd + vec3(1, .97, .92)*spe*0.*shd)*ao*atten;

    col += vec3(1.2, .5, .15)*col*svGlow*4.;
  }

  col = mix(col, sky.xyz*2., smoothstep(.15, .99, t/FAR));

  const float focD = 2., coc = .8;

  float l = abs(t - focD) - coc;
  l = clamp(l/coc, 0., 2.);
  svVal = mix(l, smoothstep(0., 1., -p.y + .15), .25);
  return vec4(col, 0.);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;
  u *= 1. + dot(u, u)*.125;

  tm = iTime;

  vec3 lk = vec3(0, 0, iTime*CAM_Z);
  vec3 ro = lk + vec3(0, 1., -1.4);

  vec3 ww = normalize(lk - ro);
  vec3 uu = normalize(cross(vec3(0, 1, 1), ww ));
  vec3 vv = cross(ww, uu);
  const float FOV = PI/4.;
  vec3 rd = normalize(u.x*uu + u.y*vv + ww/FOV);

  vec2 pDir = normalize(path(lk.z) - path(ro.z));
  float turn = path(lk.z).x;
  rd.xz = rot2(turn/32.)*rd.xz;
  rd.xy *= rot2(-turn/64.);

  if (iMouse.z > 0.)
  {
    rd.yz *= rot2((iMouse.y - iResolution.y*.5)/iResolution.y*3.1459);  
    rd.xz *= rot2((iMouse.x - iResolution.x*.5)/iResolution.x*3.1459);  
  } 

  float t = FAR;
  vec4 c4 = render(ro, rd, t);
  vec3 col = c4.xyz;

  gl_FragColor = vec4((max(col, 0.)), t);
}
