#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

// Cellular: 0, Gyroid: 1, Gyroid with 2 levels: 2, Gyroid (reverse space): 3
#define SURFACE 1
#define TAU 6.2831853
#define FAR 50.
#define ZERO min(iFrame, 0)

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

#define STATIC
float
hash21 (vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx)*.1031);
  p3 += dot(p3, p3.yzx + 42.123);

#ifdef STATIC
  return fract((p3.x + p3.y) * p3.z);
#else
  p3.x = fract((p3.x + p3.y) * p3.z);
  return sin(p3.x*TAU + iTime); // Animation, if desired.
#endif
}

float hash (float n) { return fract(cos(n)*45758.5453); }

float
hash31 (vec3 f)
{
  uvec3 p = floatBitsToUint(f);
  p = 1103515245U*((p >> 2U)^(p.yzx>>1U)^p.zxy);
  uint h32 = 1103515245U*(((p.x)^(p.y>>3U))^(p.z>>6U));

  uint n = h32^(h32 >> 16);
  return float(n & uint(0x7fffffffU))/float(0x7fffffff);
}

float
smax (float a, float b, float k)
{
   float f = max(0., 1. - abs(b - a)/k);
   return max(a, b) + k*.25*f*f;
}

float
smin (float a, float b, float k)
{
   float f = max(0., 1. - abs(b - a)/k);
   return min(a, b) - k*.25*f*f;
}

vec2
smin (vec2 a, vec2 b, float k)
{
   vec2 f = max(vec2(0), 1. - abs(b - a)/k);
   return min(a, b) - k*.25*f*f;
}

float
noise3D (vec3 p)
{
  const vec3 s = vec3(113, 157, 1);

  vec3 ip = floor(p);

  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);

  p -= ip;

  p = p*p*(3. - 2.*p);

  h = mix(fract(sin(mod(h, TAU))*43758.5453), 
          fract(sin(mod(h + s.x, TAU))*43758.5453), p.x);

  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

float
drawObject (vec3 p)
{
  p = fract(p) - .5;    
  return dot(p, p);
}

float
cellTile (vec3 p)
{
  vec4 v, d; 
  d.x = drawObject(p - vec3(.81, .62, .53));
  p.xy = vec2(p.y - p.x, p.y + p.x)*.7071;
  d.y = drawObject(p - vec3(.39, .2, .11));
  p.yz = vec2(p.z - p.y, p.z + p.y)*.7071;
  d.z = drawObject(p - vec3(.62, .24, .06));
  p.xz = vec2(p.z - p.x, p.z + p.x)*.7071;
  d.w = drawObject(p - vec3(.2, .82, .64));

  v.xy = smin(d.xz, d.yw, .05);
  d.x =  smin(v.x, v.y, .05);

  return d.x;
}

vec2
path (float z)
{
  float a = sin(z*.11);
  float b = cos(z*.14);
  return vec2(a*4. - b*1.5, b*1.7 + a*1.5); 
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
}

float
dist2D (vec2 p)
{
  return length(p); // Round cylinder.
}

int gID;
vec3 glow;

float
map (vec3 p)
{
#if SURFACE > 1
  vec3 q = p*3.14159265;
  float cav = dot(cos(q/2.), sin(q.yzx/2.));
  float cav2 = dot(cos(q/4.), sin(q.yzx/4.));
  cav = smin(cav, cav2/2., 1.);
  q = p;
#else
  vec3 q = p;
#endif

  float cav3 = (cellTile(q/2.))*2. - .08; 
  float cav4 = (cellTile(q*1.5 + .5))/1.5 - .01;

#if SURFACE == 0
  float sD = (.05 - cellTile(p/6.))*6.;
#endif

  vec2 pth = path(p.z);
  p.xy -= pth;

#if SURFACE == 1
  float sD = .5 - abs(dot(cos((p)*TAU/8.), sin(p.yzx*TAU/8.)));
#elif SURFACE == 2
  float sD = cav + .2;
#elif SURFACE == 3
  float sD = -cav - .6;
#endif

  sD = smax((1. - dist2D(p.xy)), sD, 2.);

  float innerSD = sD + .85; 
  //innerSD = abs(innerSD + .05) - .05;
  sD = smax(sD, -(innerSD - .1), .15);

  sD = smax(sD, -cav3, .35);
  sD = smax(sD, -cav4, .15);

  vec3 q3 = p;
  float tunSc = .95; 

  q3.xy *= rot2(p.z/2.);
  q3.xy += (dot(cos(q3*TAU/8.), sin(q3.yzx*TAU/8.))*.125 + .125);

  q3.xy -= tunSc;

  float cable = dist2D(q3.xy + .025) - .025;
  sD = min(sD, cable);

  float zSc = 1./2.;
  q3.z = p.z - zSc/2.*0. - iTime*0.;
  float ipZ = floor(q3.z/zSc);
  q3.z -= (ipZ + .5)*zSc;

  float lgt2D = sBoxS(q3.xy + .025, vec2(.05), .02);
  float lgt = sBoxS(vec2(lgt2D, q3.z), vec2(.05,  zSc/4.), .02);

  float gA = 1./(.5 + lgt*lgt*32.);
  float rnd = hash(ipZ + .12);

  vec3 gCol = vec3(gA, gA*sqrt(gA)*.4, gA*gA*.2)*2.;
  if(lgt<innerSD)
    gCol = mix(gCol, gCol/5., 1. - smoothstep(.35, .45, sin(TAU*ipZ/16. - iTime*4.)*.5));
  if(lgt<sD && lgt<1.) glow += gCol;

  gA = 1./(.5 + innerSD*innerSD*128.);
  gCol = vec3(gA, gA*sqrt(gA)*.4, gA*gA*.2);
  if((innerSD*innerSD/2.<sD) && innerSD<1.)
    glow += gCol/2.;

  gID = sD<lgt && sD<innerSD? 0 : lgt<innerSD? 1 : 2;
  sD = min(sD, lgt);
  sD = min(sD, innerSD);

  return sD;
}

float
trace (vec3 ro, vec3 rd)
{
  glow = vec3(0);

  float d, t = hash31(fract(ro*88.567)*7. + rd)*.25;
  for(int i = ZERO; i<128; i++)
  {
    d = map(ro + rd*t);

    if(abs(d)<.001 || t>FAR) break;

    t += min(d*.9, .2);
  }

  return min(t, FAR);
}

vec3
normal (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.001, 0, 0), mp = e.zzz;
  for(int i = ZERO; i<6; i++)
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
softShadow (vec3 ro, vec3 lp, float k)
{
  const int iter = 32; 

  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.; 
  float end = max(length(rd), 1e-5);
  rd /= end;

  for (int i = ZERO; i<iter; i++)
  {
    float d = map(ro + rd*t);
    shade = min(shade, k*d/t);

    if (d<0. || t>end) break; 

    t += clamp(d, .01, .5); 
  }

  return max(shade, 0.); 
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;
  for( int i = 0; i<5; i++ )
  {
    float hr = float(i + 1)*.15/5.;        
    float d = map(p + n*hr);
    occ += (hr - d)*sca;
    sca *= .75;
  }

  return clamp(1. - occ, 0., 1.);  
}

float
curve (vec3 p, float spr, float amp, float offs)
{
  float d = map(p);

  spr /= 450.;

#if 0
  vec2 e = vec2(-spr, spr); // Example: ef = .25;
  float d1 = map(p + e.yxx), d2 = map(p + e.xxy);
  float d3 = map(p + e.xyx), d4 = map(p + e.yyy);
  return clamp((d1 + d2 + d3 + d4 - d*4.)/e.y/2.*amp + offs + .5, 0., 1.);
#else  
  vec2 e = vec2(spr, 0);
  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);

#if 1
  return smoothstep(-.05, .05, (d1 + d2 + d3 + d4 + d5 + d6 - d*6.)/e.x/2.*amp + offs);
#else
  d *= 2.;
  return 1. - smoothstep(-.05, .05, (abs(d1 + d2 - d) + abs(d3 + d4 - d) + 
                          abs(d5 + d6 - d))/e.x/2.*amp + offs + .0);
#endif
#endif
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  vec3 lookAt = vec3(0, 0, iTime*4.);
  vec3 camPos = lookAt + vec3(0, 0, -.2);

  vec3 lightPos = camPos + vec3(0, .25, 5);

  lookAt.xy += path(lookAt.z);
  camPos.xy += path(camPos.z);
  lightPos.xy += path(lightPos.z);

  float FOV = TAU/6.;
  vec3 forward = normalize(lookAt - camPos);
  vec3 right = normalize(vec3(forward.z, 0, -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(uv.x*right + uv.y*up + forward/FOV );
  rd.xy = rot2(-path(lookAt.z).x/16.)*rd.xy;

  float t = trace(camPos, rd);

  int svGID = gID;
  vec3 svGlow = glow;

  vec3 sky = vec3(1, .8, .55);

  vec3 gP = camPos + rd*t;
  vec2 pth = gP.xy - path(gP.z);
  svGlow = mix(svGlow.xzy, svGlow, .5 + clamp(pth.y/.2, 0., 1.)*.5);
  sky = mix(sky.xzy, sky, .5 + clamp(pth.y/2., 0., 1.)*.5);

  vec3 sceneCol = sky;

  if(t<FAR)
  {
    vec3 sp = camPos + t*rd;
    vec3 sn = normal(sp);

    float ao = calcAO(sp, sn);
    float sh = softShadow(sp + sn*.002, lightPos, 4.);

    float spr = 4., ampC = 1., offs = .0;
    float crv = curve(sp, spr, ampC, offs);

    vec3 ld = lightPos - sp;

    float distlpsp = max(length(ld), .001);

    ld /= distlpsp;

    float atten = 1./(1. + distlpsp*.25);

    float ambience = .5;

    float diff = max( dot(sn, ld), 0.);

    vec3 hf = (ld - rd)/2.;
    float spec = pow(max(dot(hf, sn), 0.), 8.);

    vec3 texCol = mix(vec3(.5, .4, .3), vec3(0, .1, .2), 
                      (noise3D(sp*32.)*.66 + noise3D(sp*64.)*.34));
    texCol *= mix(vec3(.16, .1, 0), vec3(.8, .9, .96), (1. - cellTile(sp*4.5)*.75));
    texCol *= mix(vec3(.9, .95, 1), vec3(.1, 0, 0), .75 - smoothstep(-.1, .5, cellTile(sp*.5))*.75);

    if(svGID==2)
    {
      texCol = mix(texCol, vec3(.03), .5);
      texCol += texCol*svGlow*.1;
    }
    else if(svGID==1)
    {
      texCol += texCol*svGlow*.1;
    }

    float bl = max(dot(-ld, sn), 0.);
    texCol = texCol + texCol*sky*bl*8.;

    texCol += texCol*sky*(sn.y*.5 + .5);

    sceneCol = texCol*(diff*sh + ambience + vec3(.2)*spec*sh);
    sceneCol *= crv + .5;
    sceneCol *= atten*ao;
  }

  sceneCol += sceneCol*svGlow;

  sceneCol = mix(sky, sceneCol, 1./(t*t/FAR/FAR*5. + 1.));

  uv = gl_FragCoord.xy/iResolution;
  sceneCol = mix(sceneCol, vec3(0), (1. - pow(16.*uv.x*uv.y*(1.-uv.x)*(1. - uv.y), .25))*.5);

  gl_FragColor = vec4(sqrt(max(sceneCol, 0.)), 1);
}
