#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 80.

// 0, 1, 2, or 3: Round, square, rounded-square or octagonal
#define TUBE_SHAPE 3

#define CAMERA_WEAVE

vec3 vObjID;

float glow;

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
smax (float a, float b, float s)
{
  float h = clamp(.5 + .5*(a - b)/s, 0., 1.);
  return mix(b, a, h) + h*(1. - h)*s;
}

vec3
tex3D (sampler2D t, vec3 p, vec3 n )
{
  n = max(abs(n), 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;

  return (tx*tx*n.x + ty*ty*n.y + tz*tz*n.z);
}

float
sdBox (vec2 p, float b)
{
  vec2 d = p - b;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.));
}

float
tube (vec2 p, float sc, float rad)
{
#if TUBE_SHAPE == 0
  return length(p) - rad;
#elif TUBE_SHAPE == 1
  return sdBox(p, rad);
#elif TUBE_SHAPE == 2
  return smax(p.x, p.y, .015) - rad;
  //return pow(dot(pow(p, vec2(8)), vec2(1)), 1./8.) - rad;
#else
  return max(max(p.x, p.y), (p.x + p.y)*sc) - rad;
#endif
}

vec4
torTube (vec3 p)
{
  const float rad2 = .07;

  float tb = tube(abs(vec2(length(p.xy) - .5, p.z)), .75, rad2);

  float innerTb = 1e5;

  const float aNum = 12.; 

  p.xy = rot2(3.14159/4.)*p.xy;

  float a = atan(p.y, p.x);    
  float ia = floor(a/6.283*aNum) + .5;

  p.xy = rot2(ia*6.283/aNum)*p.xy;

  p.x -= .5;

  p = abs(p);

  float band = max(tube(p.xz, .75,  rad2 + .0075), p.y - .06);
  vec2 peg = vec2(tube(p.xy, .64, .0425), tube(p.yz, .64, .0425)); 

  if(mod(ia + 1., 3.)>2.)
  {
    band = min(band, max(tube(p.xz, .6, rad2 + .015), p.y - .04));
    band = min(band, max(tube(p.xz, .6, rad2 + .025), p.y - .04/3.));
  }
  else
  {
    float hole = min(peg.x, peg.y);

#if TUBE_SHAPE == 3
    band = min(band, min(max(peg.x, p.z - rad2 - .0075), max(peg.y, p.x - rad2 - .0075)));
#else 
    band = min(band, min(max(peg.x, p.z - rad2 - .02), max(peg.y, p.x - rad2 - .02)));
#endif
    band = max(band, -(hole + .015));

    tb = max(tb, -(hole + .015));

    innerTb = length(p) - rad2 + .01;
  }

  return vec4(tb, band, innerTb, ia);
}

vec4
straightTube (vec3 p)
{
  const float rad2 = .07;

  float tb = tube(abs(p.xy), .75, rad2);

  float innerTb = 1e5; //tb + .0075; 

  float band = 1e5;
  const float aNum = 1.;

  float ia = floor(p.z*3.*aNum);

  float opz = mod(p.z + 1./aNum/3., 1./aNum);

  p.z = mod(p.z, 1./aNum/3.) - .5/aNum/3.;
  p = abs(p);

  band = max(tb - .0075, p.z - .06);
  vec2 peg = vec2(tube(p.xz, .64, .0425), tube(p.yz, .64, .0425)); 

  if(opz>2./aNum/3.)
  {
    band = min(band, max(tube(p.xy, .6, rad2 + .015), p.z - .04));
    band = min(band, max(tube(p.xy, .6, rad2 + .025), p.z - .04/3.));
  }
  else
  {
    float hole = min(peg.x, peg.y);

#if TUBE_SHAPE == 3
    band = min(band, min(max(peg.x, p.y - rad2 - .0075), max(peg.y, p.x - rad2 - .0075)));
#else
    band = min(band, min(max(peg.x, p.y - rad2 - .02), max(peg.y, p.x - rad2 - .02)));
#endif
    band = max(band, -(hole + .015));

    tb = max(tb, -(hole + .015));

    innerTb = length(p) - rad2 + .01;
  }

  return vec4(tb, band, innerTb, ia);
}

vec4
torTubeTest (vec3 p)
{
  vec2 v = vec2(length(p.xy) - .5, p.z);

  return vec4(p, dot(v, v));
}

vec4
straightTubeTest (vec3 p)
{
  vec2 v = p.xy;

  return vec4(p, dot(v, v));
}

float
map (vec3 p)
{
  float rnd = fract(sin(dot(floor(p + vec3(111, 73, 27)), vec3(7.63, 157.31, 113.97)))*43758.5453);
  float rnd2 = fract(rnd*41739.7613 + .131);

  p = fract(p) - .5;

  if(rnd>.833) p = p.xzy;
  else if(rnd>.666) p = p.yxz;
  else if(rnd>.5) p = p.yzx;
  else if(rnd>.333) p = p.zxy;
  else if(rnd>.166) p = p.zyx;

  vec4 tb1, tb2, tb3;
  tb1 = torTubeTest(vec3(p.xy + .5, p.z));
  if(rnd2>.66)
  {
    tb2 = torTubeTest(vec3(p.yz - .5, p.x));
    tb3 = torTubeTest(vec3(p.xz - vec2(.5, -.5), p.y));
  }
  else
  {
    tb2 = torTubeTest(vec3(p.xy - .5, p.z));
    tb3 = straightTubeTest(p);  
  }

  p = tb1.w<tb2.w && tb1.w<tb3.w ? tb1.xyz : tb2.w<tb3.w ? tb2.xyz : tb3.xyz;

  vec4 tb;

  if(rnd2<=.66 && tb3.w<tb1.w && tb3.w<tb2.w) tb = straightTube(p);
  else tb = torTube(p);

  vObjID = tb.xyz;

  return min(min(vObjID.x, vObjID.y), vObjID.z);
}

float
trace (vec3 o, vec3 r)
{
  glow = 0.;

  float t = 0., d, ad;

  for (int i = 0; i<128; i++)
  {
    d = map(o + r*t);
    ad = abs(d);

    glow += 1./(1. + ad*ad*8.);

    if(ad<.001*(t*.125 + 1.) || t>FAR) break;

    t += d; 
  }

  return min(t, FAR);
}

float
shadow (vec3 ro, vec3 lp, float k, float t)
{
  const int maxIterationsShad = 32; 

  vec3 rd = lp-ro;

  float shade = 1.;
  float dist = .001*(t*.125 + 1.);
  float end = max(length(rd), 0.0001);

  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);

    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist));

    dist += clamp(h, .01, .2); 

    if (h<0.0 || dist > end) break; 
  }

  return min(max(shade, 0.) + .0, 1.); 
}

float
cAO (vec3 p, vec3 n)
{
  float sca = 1.25, occ = 0.0;
  for( int i=0; i<5; i++ )
  {
    float hr = .01 + float(i)*.5/4.;        
    float dd = map(p + hr*n);
    occ += (hr - dd)*sca;
    sca *= .7;
  }

  return clamp(1. - occ, 0., 1.);   
}

vec3
nrm (vec3 p, inout float edge, inout float crv, float t)
{
  vec2 e = vec2(1./mix(400., iResolution.y, .5)*(1. + t*.5), 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.002, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}
 
void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  float tm = iTime;
#ifdef CAMERA_WEAVE
  tm *= .75;
#endif

  vec3 o = vec3(0, 0, tm); 
  vec3 lp = o + vec3(-1, 3, -1);

#ifdef CAMERA_WEAVE
  o.x += sin(tm * 3.14159265/6. + 1.5707963);
#endif

  vec3 r = normalize(vec3(uv, 1.15));
  r = normalize(vec3(r.xy, r.z - length(r.xy)*.15));

#ifdef CAMERA_WEAVE
  r.xz *= rot2(-sin(tm/2. - 1.5707963) * 0.6);
  r.xy *= rot2(-sin(tm/2. - 1.5707963) * 0.4);
#else
  r.xz *= rot2(sin(tm/2.) * 0.4);
  r.xy *= rot2(cos(tm/2.) * 0.2);
#endif

  float t = trace(o, r);

  float objID = (vObjID.x<vObjID.y && vObjID.x<vObjID.z) ? 0. : (vObjID.y<vObjID.z) ? 1. : 2.;

  vec3 sc = vec3(0);

  if(t<FAR)
  {
    vec3 sp = o + r*t;

    float edge = 0., crv = 1.;
    vec3 sn = nrm(sp, edge, crv, t);

    vec3 oCol = vec3(1);
    vec3 bCol = mix(vec3(1, .1, .3).zyx, vec3(1, .5, .1).zyx, dot(sin(sp*8. - cos(sp.yzx*4. + iTime*4.)), vec3(.166)) + .5);

    if(objID<.5)oCol = mix(bCol, vec3(1), .97);
    else if(objID>1.5) oCol = mix(bCol, vec3(1), .05) + bCol*2.;
    else oCol = oCol = mix(bCol, vec3(1.35), .97)*vec3(1.1, 1, .9);

    vec3 tx = tex3D(iChannel0, sp*2., sn);
    tx = smoothstep(.0, .5, tx)*2.;

    if(objID<1.5) oCol *= tx;
    else oCol *= mix(vec3(1), tx, .5);

    float ao = cAO(sp, sn);
    float sh = shadow(sp + sn*.002, lp, 16., t); 

    vec3 ld = lp - sp;
    float dist = max(length(ld), 0.001);
    ld /= dist;

    float atten = 3.5/(1. + dist*0.05 + dist*dist*0.05);

    float diff = max(dot(ld, sn), 0.);
    if(objID<1.5) diff = pow(diff, 4.)*2.;
    float spec = pow(max(dot( reflect(ld, sn), r), 0.0 ), 32.0);

    sc = oCol*(diff + ao*.2) + mix(bCol.zyx, vec3(1, .7, .3), .5)*spec*4.;

    sc += .015/max(abs(.05 - map(sp*1.5 + sin(iTime/6.))), .01)*oCol*mix(bCol, vec3(1, .8, .5), .35);

    if(objID<1.5) sc += bCol*glow*.025;
    else sc += bCol*glow*1.5;

    sc *= (1. - edge*.7);
    sc *= atten*(sh + ao*.25)*ao;
  }

  float fog = 1./(1. + t*.125 + t*t*.05);
  sc = mix(vec3(0), sc, fog);//

  uv = gl_FragCoord.xy/iResolution;
  sc *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .125);

  gl_FragColor = vec4(sqrt(max(sc, 0.)), 1.);
}
