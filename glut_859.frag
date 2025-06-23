#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

#define TAU 6.2831853 
//#define STATIC
#define NET_OVERLAY
// Yellow: 0, Metal: 1
#define WEBTYPE 0

float tm = 0.;

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
smin (float a, float b, float k)
{
  float f = max(0., 1. - abs(b - a)/k);
  return min(a, b) - k*.25*f*f;
}

vec4
hash4 (vec4 p)
{
  return fract(sin(mod(p, TAU))*43758.5453);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(113, 157, 1);

  vec3 ip = floor(p);

  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);

  p -= ip;

  p = p*p*(3. - 2.*p);

  h = mix(hash4(h), hash4(h + s.x), p.x);

  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f + 16384.);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

vec2
hash22 (vec2 f)
{
  uvec2 p = floatBitsToUint(f + 16384.);

  p = 1103515245U*((p>>1U)^(p.yx));
  uint h32 = 1103515245U*((p.x)^(p.y>>3U));
  uint n = h32^(h32>>16);

  uvec2 rz = uvec2(n, n*48271U);
#ifdef STATIC
  return (vec2((rz>>1)&uvec2(0x7fffffffU))/float(0x7fffffff) - .5)*.64;
#else
  f = vec2((rz>>1)&uvec2(0x7fffffffU))/float(0x7fffffff);
  return sin(f*TAU + tm)*.32;
#endif
}

vec4
Voronoi (vec2 p, float smF)
{
  float sc = 4.;
  p *= sc;

  vec2 ip = floor(p) + .5;
  p -= ip;

  float vor = 1e5, vor2 = 1e5;

  vec2 offsID = vec2(0);

  for(int i = min(0, iFrame); i<9; i++)
  {
    vec2 offs = vec2(i%3, i/3) - float(3/2);
    vec2 offsP = offs + hash22(ip + offs);

    float d = length(p - offsP);

    if(d<vor)
    {
      vor2 = vor;
      vor = d;
      offsID = offs;
    }
    else if (d<vor2)
      vor2 = d;
  }

  const int N = 3;

  float minDist = 1e8;

  vec2 pointCenter = (offsID + hash22(ip + offsID));

  for(int i = min(0, iFrame); i<N*N; i++)
  {
    vec2 offs = vec2(i%N, i/N) - float(N/2);
    if(offs == offsID) continue;

    vec2 idI = offsID + offs;
    vec2 pI = (idI + hash22(ip + idI));

    vec2 p2p = normalize(pI - pointCenter);
    vec2 edgeMid = mix(pointCenter, pI, .5);
    float d = -dot(p - edgeMid, p2p);

    minDist = smin(minDist, d, smF);
  }

  vec2 id = ip + offsID;

  return vec4(minDist/sc, (vor2 - vor)/sc, id);
}

vec3
pattern (vec2 p, vec2 ip)
{
#if 1
  float smF = .1;

#ifdef NET_OVERLAY
  p *= rot2(hash21(ip)*TAU);
#endif

  p.y += cos(p.x*TAU*2. + ip.y*.5 + ip.x*.5)/32.;

  vec4 v4 = Voronoi(p*1.5 + 71., smF);

  float v = -v4.x/1.5;

  p.y += cos(p.x*TAU*2. + ip.y)/64.;

  /*
  vec4 v4B = Voronoi(p*3.4 + 11., smF);
  v4B.x = min(-v4B.x, ((1. - abs(v))*n3D(vec3(p*24., 8)) - .4));//v*12.
  v = max(v, v4B.x*8.);
  */

  v *= smoothstep(0., .03, n3D(vec3(p*24., v)) - .5)*.5 + .75;

  return vec3(v, v4.zw);
#else
  float sc = 1./6.;

  float iy = floor(p.y/sc);

  p *= rot2(hash21(ip)*TAU);
  p.y += cos(p.x*TAU*2. + ip.y)*sc/4.;

  iy = floor(p.y/sc);
  p.y -= (iy + .5)*sc;

  float c = abs(p.y) - sc*.5;

  c *= smoothstep(0., .03, n3D(vec3(p*24., c)) - .5)*.5 + .75;

  return vec3(c, vec2(0, iy));
#endif
}

vec3 gID = vec3(0);
vec2 gIP;

float
bumpFunc (vec2 p)
{
  float smF = .2;
#ifdef NET_OVERLAY
  tm = iTime;
#else
  tm = 0.;
#endif

#ifdef NET_OVERLAY
  vec4 v4 = Voronoi(p, smF);
#else
  vec4 v4 = vec4(1e5, 0, 0, 0);
#endif
  float v = v4.x;

  gIP = v4.zw;

#ifdef NET_OVERLAY
  float ew = .015;
#else
  float ew = -1e5;
#endif
  
  if(v<ew)
  {
    v = abs(v - ew)/(ew)*.1;
    v = min(v, .15);

    p.y += cos(p.x*TAU*2.)/64.;
    v *= smoothstep(-.02, .02, n3D(vec3(p*24., v)) - .5)*.25 + .875;

    gID.x = 0.;
    gID.y = 0.;
    gID.z = v;
  }
  else
  {
#ifdef NET_OVERLAY
    v = (v - ew)/(1. - ew);
#endif
    gID.z = v;

    tm = 0.;

    vec3 p4 = pattern(p, gIP);
    v = .5 + min(-p4.x*6., .05) + min(-p4.x*6., .2);
    gID.x = -v;
    gID.y = 1.;
  } 

  return max(v, 0.);
}

float
curve (vec2 p, float spr, float amp, float offs)
{
  spr /= 450.;

  float d = bumpFunc(p);

  vec2 e = vec2(spr, 0);
  float d1 = bumpFunc(p + e.xy), d2 = bumpFunc(p - e.xy);
  float d3 = bumpFunc(p + e.yx), d4 = bumpFunc(p - e.yx);

  return 1. - clamp((d1 + d2 + d3 + d4 - d*4.)/e.x/2.*amp + offs + .5, 0., .5);
}

void
main ()
{
  vec2 iR = iResolution;
  vec2 uv = (gl_FragCoord.xy - iR*.5)/iR.y;

  vec3 sp = vec3(uv, 0);
  vec3 rd = normalize(vec3(uv, 1.));
  vec3 lp = vec3(cos(iTime/2.)*.5, sin(iTime/2.)*.2 + .5, -2.);
  vec3 sn = vec3(0., 0., -1);

  vec2 cam = vec2(2, 1)*iTime/32.;
  sp.xy += cam;
  lp.xy += cam;

  tm = iTime;
  vec4 v4Sh = Voronoi(sp.xy + normalize(lp - sp).xy*.1, .2);
  float vSh = v4Sh.x;

  vec2 eps = vec2(1.5/iResolution.y, 0.);

  float f = bumpFunc(sp.xy);
  vec3 svGID = gID;
  vec2 svGIP = gIP;
  float fx = bumpFunc(sp.xy + eps.xy);
  float fy = bumpFunc(sp.xy + eps.yx);

  float fx2 = bumpFunc(sp.xy - eps.xy);
  float fy2 = bumpFunc(sp.xy - eps.yx);

  float edge = abs(fx + fy+ fx2 + fy2 - 4.*f);
  edge = smoothstep(-.05, .05, -.5 + edge/eps.x/2.);

  const float bumpFactor = .2;

  fx = (fx2-fx)/eps.x/2.;
  fy = (fy2-fy)/eps.x/2.;

  sn = normalize(sn + vec3(fx, fy, 0)*bumpFactor);

  float spr = 3., amp = 1., offs = 0.;
  float crv = curve(sp.xy, spr, amp, offs);

  vec3 ld = lp - sp;
  float lDist = max(length(ld), 0.001);
  ld /= lDist;

  float atten = 1./(1. + lDist*lDist*.15);

  float diff = max(dot(sn, ld), 0.);  
  float spec = pow(max(dot( reflect(-ld, sn), -rd), 0.), 16.); 
  float fre = pow(max(1. - max(dot(-rd, sn), 0.), 0.), 5.);

  vec3 texCol;
    
  float sf = 1./iResolution.y;
    
  float ew = .004;

  if(svGID.y==0.)
  {
#if WEBTYPE == 0
    texCol = vec3(1, .8, .125)*1.25;
#else
    texCol = vec3(.4, 1, 1.5)*.25;
    diff *= diff*2.;
#endif
  }
  else
  {
    vec3 pCol = vec3(.15, .4, .7);

    tm = 0.;
    vec3 pat = pattern(sp.xy, svGIP);

    float rnd = hash21(svGIP + pat.yz)*.5 + .25;
    float rnd2 = hash21(svGIP + pat.yz + .1)*.5 + .25;
    float rnd3 = hash21(svGIP + pat.yz + .2)*.5 + .25;

    pCol = pCol*(rnd + .5) + vec3(0, rnd2*.3, rnd3*.1);

    texCol = mix(texCol, pCol*.5, 1. - smoothstep(0., sf, pat.x + ew));

    float ns = mix(n3D(sp*24.), n3D(sp*48.), 1./3.);
    ns = mix(ns, n3D(sp*96.), 1./3.);

    texCol *= ns*1.5;

#ifdef NET_OVERLAY
    texCol = mix(texCol, texCol*.35, 1. - smoothstep(0., .035, svGID.z));
    texCol = mix(texCol, texCol*.35, 1. - smoothstep(0., .035, vSh));
#endif
  }

  texCol = mix(texCol*vec3(1, .05, .3)*1.5, texCol*1.5, diff*diff);

  if(svGID.y==1.) diff *= diff; 

  float bl = max(dot(normalize(vec3(-ld.xy, 0)), sn), 0.);

  texCol = texCol*.9 + texCol*vec3(1, .1, .3)*bl*12.;

  texCol = mix(texCol, texCol*.25, 1. - smoothstep(0., sf, abs(svGID.z) - ew));

  texCol *= crv*.85 + .65;  

  vec3 col = texCol*(diff*1.5 + .5 + spec*4.)*atten;

  col *= 1. - edge*.7;

  uv = abs(uv) - vec2(iR.x/iR.y, 1)/2. + .15;
  float vig = min(max(uv.x, uv.y), 0.) + length(max(uv, 0.)) - .075;
  col = mix(col, col*.65,  smoothstep(0., .1, vig));

  gl_FragColor = vec4(pow(max(col, 0.), vec3(1)/2.2), 1.);
}
