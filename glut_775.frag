#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;
uniform sampler2D iChannel2;

#define FAR 15.

// values of about "1./2." to "1./6" work
#define GSCALE vec3(1./4.);

// Light type: Point: 0, Direct: 1
#define LIGHT_TYPE 0

// Set to zero for a static camera
#define CAM_Z 1./3.

#define OFFSET_ROWS
#define GREY_FACES
#define COLORED_SIDES
//#define SINGLE_FACE_COLOR
//#define HOLES

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max(n*n - .2, .001);
  n /= dot(n, vec3(1)); 

  vec3 tx = textureLod(tex, p.yz, 0.).xyz;
  vec3 ty = textureLod(tex, p.zx, 0.).xyz;
  vec3 tz = textureLod(tex, p.xy, 0.).xyz;

  return mat3(tx*tx, ty*ty, tz*tz)*n;
}

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
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

float
n2D (vec2 p)
{
  const vec2 s = vec2(1, 113);

  vec2 ip = floor(p); p -= ip;

  vec4 h = vec4(0., s.x, s.y, s.x + s.y) + dot(ip, s);

  p = p*p*(3. - 2.*p);

  h = fract(sin(mod(h, 6.2831589))*43758.5453);

  h.xy = mix(h.xy, h.zw, p.y);
  return mix(h.x, h.y, p.x);
}

float fbm (vec2 p) { return n2D(p)*.533 + n2D(p*2.)*.267 + n2D(p*4.)*.133 + n2D(p*8.)*.067; }

float
sBoxS (vec2 p, vec2 b, float sf)
{
  p = abs(p) - b + sf;
  return min(max(p.x, p.y), 0.) + length(max(p, 0.)) - sf;
}

float
opExtrusion (float sdf, float pz, float h, float sf)
{
  vec2 w = vec2( sdf, abs(pz) - h) + sf;
  return min(max(w.x, w.y), 0.) + length(max(w, 0.)) - sf;   
}

vec3
getTex (sampler2D iCh, vec2 p)
{
  vec3 tx = texture(iCh, p).xyz;
  return tx*tx; // Rough sRGB to linear conversion.
}

float gBx;

float
hm (vec2 p)
{
  p.y -= iTime*CAM_Z;
 
  p -= (vec2(cos(iTime), sin(iTime)))*vec2(.5, .25);

  p = rot2(cos(iTime/2.)*3.14159*.85)*p;

  float bx = sBoxS(p, vec2(1.25), .1);
  gBx = bx;

  return min(bx + .65, abs(bx));
}

vec4
getGrid (vec2 p, inout vec2 sc)
{
#ifdef OFFSET_ROWS
  if(mod(floor(p.y/sc.y), 2.)<.5) p.x += sc.x/2.;
#endif

  vec2 q = p;

  vec2 ip = (floor(p/sc) + .5)*sc;
  p -= ip;

  const int n = 2;
  for(int i = 0; i<n; i++)
  {
    if(hm(ip)>float(n - i)/float(n)/4.)
      break;

    p = q;
    sc /= 2.;

    ip = (floor(p/sc) + .5)*sc;
    p -= ip;
  }

  return vec4(p, ip);
}

vec3 gDir;
vec3 gRd;
float gCD;
vec3 gSc; 
vec2 gP;
vec4 gID;
vec3 glow;

float
map (vec3 q3)
{
  float fl = q3.y + .5;
 
  vec3 sc = GSCALE;

  vec4 p4 = getGrid(q3.xz, sc.xz); 
  vec2 p = p4.xy;
  vec2 id = p4.zw;

  vec2 rC = (gDir.xz*sc.xz - p)/gRd.xz;

  gCD = max(min(rC.x, rC.y), 0.) + .001;

  float h = hm(id);

  h = clamp(h*2., 0., 1.)*.95 + (sin(hash21(id + .05)*6.28315289 + iTime)*.5 + .5)*.05;

  float d2 = sBoxS(p, sc.xz/2., 0.);

#ifdef HOLES
  if(sc.x>1./16.) d2 = max(d2, -(d2 + sc.x/2.5));
#endif

  float d = opExtrusion(d2, q3.y - h/2., h/2., 0.);

  vec3 p3 = vec3(p.x, q3.y - (h - 3.)  - .0045, p.y);
  d = min(d, max(length(p3) - 3., d2));

  d -= min(-d2*2., .06)*.15;

  if(q3.y<h)
  {
    float dd = max(h - q3.y, 0.)/h;
    glow += max(1. - dd, 0.);    
  }

  gSc = vec3(sc.x, h, sc.z);
  gP = p;
  gID = vec4(d, d2, id);
 
  return min(fl, d);
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
    if((i&1)==1)
    {
      mp = mp.yzx;
      e = e.zxy;
    }
  }

  return normalize(mp);
}

float
rayMarch (vec3 ro, vec3 rd)
{
  float d, t = hash31(ro + rd)*.25;

  vec2 dt = vec2(1e8, 0);

  gDir = step(0., rd) - .5;
  gRd = rd;

  glow = vec3(0);

  const int iter = 128;
  int i = 0;
     
  for (i = 0; i < 128; i++)
  {
    d = map(ro + rd*t);

    if (d<dt.x)
      dt = vec2(d, t);

    if (d<.001 || t > FAR)
      break;

    t += min(d*.9, gCD);
  }

  if(i == iter - 1)
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
  float sca = 2., occ = 0.;
  for(int i = 0; i<12; i++)
  {
    float hr = (float(i) + 1.)*.35/12.; 
    //float fi = float(i + 1);
    //vec3 rnd = vec3(hash31(p + fi), hash31(p + fi + .1), hash31(p + fi + .3)) - .5;
    //vec3 rn = normalize(n + rnd*.15);
    vec3 rn = cosDir(p + n*hr, n); // Random half hemisphere vector.
    float d = map(p + rn*hr);
        
    occ = occ + max(hr - d, 0.)*sca;
    sca *= .7;
  }

  return clamp(1. - occ, 0., 1.);    
}

vec4
render (vec3 ro, vec3 rd)
{
#if LIGHT_TYPE == 0
  vec3 lp = ro + vec3(2, 1, 6);
#else
  vec3 ld = normalize(vec3(2, 1. + 4., 6));//-vec3(-1.5, -3, -3)
  float lDist = FAR;
#endif

  float t = rayMarch(ro, rd);

  vec3 svSc = gSc;
  vec2 svP = gP;
  vec4 svGID = gID;

  float svBX = gBx;

  vec3 col = vec3(0);
  if (t < FAR)
  {
    vec3 p = ro + rd*t;
    vec3 n = normal(p);

#if LIGHT_TYPE == 0
    vec3 ld = lp - p;
    float lDist = length(ld);
    ld /= lDist;
#endif

    float shd = softShadow(p + n*.0015, ld, lDist, 8.);
    float ao = calcAO(p, n);

    vec2 id = svGID.zw;

    float bx = abs(svBX + .35) - .25;

    vec3 bgCol = mix(vec3(.8, .8, 1)*1.1, vec3(.65, .6, .9)/1.25, hash21(id + .13));

    vec3 shCol = mix(vec3(1, .05, .04)*1.5, vec3(1, .1, .1)/2., hash21(id + .14));
    shCol = mix(shCol, .5 + .45*cos(6.2831*hash21(id + .15)/6. + vec3(0, 1.3, 2)*1.5 + .5), .25);
    shCol = shCol*1.3 + .05;

    vec3 an = abs(n);
    int face = max(an.x, an.z)<an.y? 1 : 0;
#ifdef COLORED_SIDES
    if(face == 0)
    {
      bgCol = mix(bgCol, bgCol*shCol*2., .95);
      shCol = mix(bgCol/2., bgCol*shCol, .5);//vec3(.7, .9, 1.2)/1.2;
      bgCol = mix(bgCol, shCol, .5);
      shCol = bgCol;
    }
#else
    if(face == 0)
      shCol = bgCol;
#endif

    bgCol *= vec3(.7, .9, 1.2)*.85;

#ifdef GREY_FACES
    if(face==1)
    {
      shCol = vec3(1./3.)*dot(shCol, vec3(.299, .587, .114)); 
      bgCol = shCol;              
    }
#endif

#ifdef SINGLE_FACE_COLOR
#ifdef COLORED_SIDES
    if(face == 1)
#endif
    { shCol = bgCol; }
#endif

    vec3 oCol = mix(bgCol, shCol, (1. - smoothstep(0., .25, bx))*min(svSc.x*10., 1.));

    vec3 txP = vec3(p.x, p.y - svSc.y, p.z);
    vec3 tx = tex3D(iChannel0, txP/2. + .5, n);

    oCol *= tx*2. + .45;

    vec3 rTxP = txP;
    rTxP.xz *= rot2(3.14159/9.);
    vec3 dTx = tex3D(iChannel2, rTxP/vec3(4, 1, 1.), n);
    float difSt = dot(dTx, vec3(.299, .587, .114));

    float dif = max(dot(ld, n), 0.);
    dif = pow(dif, 2. + difSt*16.);

    float spe = pow(max(dot(reflect(ld, n), rd), 0.), 8. + 8.*difSt);

    float ew = .0075*(1. + t*.1);
    float h = svSc.y;
    vec2 sc = svSc.xz;
    float rct = svGID.y;
#ifdef HOLES
    if(sc.x>1./16.) rct = max(rct, -(rct + sc.x/2.5));
#endif
    float top = max(abs(p.y - h), abs(rct));
    float side = abs(abs(svP.x) - sc.x/2.);
    side = max(side, abs(abs(svP.y) - sc.y/2.));
    float objEdge = min(top, side - ew*.4) - ew;

    oCol = mix(oCol, oCol.yxz, dif*dif/5.);

    float speR = pow(max(dot(normalize(ld - rd), n), 0.), 5.);
    vec3 rf = reflect(rd, n);
    vec3 rTx = textureLod(iChannel1, -rf, 0.).xyz;
    rTx *= rTx;
    oCol += oCol*speR*rTx*4.;

    float am = pow(length(sin(n*2.)*.5 + .5)/sqrt(3.), 2.)*1.5; 

    vec3 speCol = vec3(1, .7, .4);

    col = oCol*(am + dif*shd + speCol*spe*shd*4.)*ao;

    col += col*col*glow/4.;

    col = mix(col, col*.1, 1. - smoothstep(0., .005, objEdge));

    float rt = t/FAR;
    col *= 1.5/(1. + rt*.2); 
  }

  col = mix(col, vec3(0), smoothstep(.3, .99, t/FAR));

  return vec4(col, t);
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  vec3 lk = vec3(0, 0, iTime*CAM_Z);
  vec3 ro = lk + vec3(cos(iTime/4.)*.02, 4, -1. + sin(iTime/2.)*.05);

  vec3 ww = normalize(lk - ro);
  vec3 uu = normalize(cross(vec3(0, 1, 0), ww ));
  vec3 vv = cross(ww, uu);
  const float FOV = 3.14159/3.;
  vec3 rd = normalize(u.x*uu + u.y*vv + ww/FOV);

  vec2 offs = vec2(fbm(rd.xz*12.), fbm(rd.xz*12. + .35));
  const float oFct = .01;
  rd.xz -= (offs - .5)*oFct; 
  rd = normalize(rd);

  if(iMouse.z>0.)
  {
    rd.yz *= rot2((iMouse.y - iResolution.y*.5)/iResolution.y*3.1459);  
    rd.xz *= rot2((iMouse.x - iResolution.x*.5)/iResolution.x*3.1459);  
  }

  vec4 c4 = render(ro, rd);
  vec3 col = c4.xyz;

  col *= smoothstep(1.5, .5, length(2.*gl_FragCoord.xy/iResolution.xy - 1.)*.7);
  col /= 1. + col/2.5;

  gl_FragColor = vec4(pow(max(col, 0.), vec3(.4545)), 1);
}
