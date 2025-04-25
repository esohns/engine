#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define FAR 10.

//#define FLOOR_HOLES
//#define SIMPLEX_GRID
//#define FLOOR_PATTERN_ONLY

float svObjID = 0.;

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max(abs(n) - .1, .001); // n = max(n*n - .2, 0.001), etc.
  n /= length(n);

  vec3 tx1 = texture(tex, p.yz).xyz;
  vec3 tx2 = texture(tex, p.zx).xyz;
  vec3 tx3 = texture(tex, p.xy).xyz;

  return tx1*tx1*n.x + tx2*tx2*n.y + tx3*tx3*n.z;
    
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p);
  h = mix(fract(sin(mod(h, 6.2831))*43758.5453), fract(sin(mod(h + s.x, 6.2831))*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z);
}

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 p)
{
  float n = dot(p, vec2(127.183, 157.927));
  return fract(sin(n)*43758.5453);
}

#ifdef SIMPLEX_GRID
float
distLine (vec2 a, vec2 b)
{
  b = a - b;
  float h = clamp(dot(a, b) / dot(b, b), 0., 1.);
  return length(a - b*h);
}
#endif

// Floor curve shape - Circle: 0, Hexagon: 1, Dodecahedron: 2.
#define SHAPE 0

float
dist (vec2 p)
{
#if SHAPE == 0
  return length(p); // Circle.
#elif SHAPE == 1
  p *= rot2(3.14159/12.);
  p = abs(p);
  return max(p.y*.8660254 + p.x*.5, p.x); // Hexagon.
#else
  p *= rot2(3.14159/12.);
  p = abs(p);
  vec2 p2 = p*.8660254 + p.yx*.5;
  return max(max(p2.x, p2.y), max(p.x, p.y)); // Dodecahedron.
#endif
}

vec2
tube (vec2 p)
{
  p = abs(p);

  float mid = max(p.x - .05, p.y - .01);

  p.x = abs(p.x - .08);
  p.x -= .02;

  float rail = max(max(p.x, p.y), (p.x + p.y)*.7071) - .02;

  return vec2(rail, mid);
}

#define SHAPE3 0
float
tubeOuter (vec2 p)
{
#if SHAPE3 == 0
  return length(p); // Circle.
#elif SHAPE3 == 1
  p *= rot2(3.14159/12.);
  p = abs(p);
  return max(p.y*.8660254 + p.x*.5, p.x); // Hexagon.
#else
  p *= rot2(3.14159/12.);
  p = abs(p);
  vec2 p2 = p*.8660254 + p.yx*.5;
  return max(max(p2.x, p2.y), max(p.x, p.y)); // Dodecahedron.
#endif
}

float
pylon (vec2 p)
{
  p = abs(p);
  p.y -= .02;

  return max(max(p.x, p.y), (p.x + p.y)*.7071) - .025;
}

#define SHAPE2 1 
float
tube4 (vec2 p)
{
#if SHAPE2 == 0
  return length(p); // Circle.
#elif SHAPE2 == 1
  //p *= rot2(3.14159/12.);
  p = abs(p);
  return max(p.y*.8660254 + p.x*.5, p.x); // Hexagon.
#else
  //p *= rot2(3.14159/12.);
  p = abs(p);
  vec2 p2 = p*.8660254 + p.yx*.5;
  return max(max(p2.x, p2.y), max(p.x, p.y)); // Dodecahedron.
#endif
}

vec4
simplexWeaveBump (vec3 q)
{
  vec2 p = q.xy;

  vec2 s = floor(p + (p.x + p.y)*.36602540378);

  p -= s - (s.x + s.y)*.211324865;

  float i = p.x < p.y? 1. : 0.;
  vec2 ioffs = vec2(1. - i, i);

  vec2 p1 = p - ioffs + .2113248654, p2 = p - .577350269; 

  float dh = hash21((s*3. + ioffs + 1.));
  if(dh<1./3.)
  {
    vec2 ang = p;
    p = p1, p1 = p2, p2 = ang;
  }
  else if(dh<2./3.)
  {
    vec2 ang = p;
    p = p2, p2 = p1, p1 = ang;
  }

  vec3 a = vec3(atan(p.y, p.x), atan(p1.y, p1.x), atan(p2.y, p2.x));

  float tw = .2;
  float mid = dist((p2 - p))*.5;
  vec3 cir = vec3(dist(p), dist(p1), dist(p2));
  vec3 tor =  abs(cir - mid) - tw;

  vec3 bg = vec3(.75);
  vec3 col = bg;
  vec3 rimCol = vec3(.5);

  vec3 ridges = smoothstep(.05, -.05, -cos(a*12.) + .93)*.5;

  vec3 ridges2 = smoothstep(.05, -.05, sin(a*18.) + .93);

  a = sin(a*3. - 6.283/9.)*.5 + .5; 

  const float sf = .015, lw = .02;

  col = mix(col, vec3(.1), 1. - smoothstep(0., sf, tor.y));
  col = mix(col, rimCol*(1. - ridges.y), 1. - smoothstep(0., sf, tor.y + lw));

  col = mix(col, vec3(.1), (1. - smoothstep(0., sf, tor.x))*1.);
  col = mix(col, rimCol*(1. - ridges.x), (1. - smoothstep(0., sf, tor.x + lw))*1.);

#ifdef SIMPLEX_GRID
  vec3 c = vec3(distLine(p, p1), distLine(p1, p2), distLine(p2, p));
  c.x = min(min(c.x, c.y), c.z);
  col = mix(col, vec3(.1), (1. - smoothstep(0., sf*2., c.x - .02)));
  col = mix(col, vec3(1, .8, .45)*1.35, (1. - smoothstep(0., sf*1.25, c.x - .005)));
#endif

  vec2 d1 = vec2(1e5), d2 = vec2(1e5), d3 = vec2(1e5);

  q.z -= .5;

  float depth = .125;
  vec2 v1 = vec2(tubeOuter(p.xy) - mid, q.z - depth); // Bottom level arc.
  vec2 v2 = vec2(tubeOuter(p1.xy) - mid, q.z + depth*a.y - depth); 
  vec2 v3 = vec2(tubeOuter(p2.xy) - mid, q.z); // Top level arc.

  d1 = tube(v1);
  d2 = tube(v2);
  d3 = tube(v3);

  float bump = d1.x<d2.x && d1.x<d3.x? ridges2.x : d2.x<d3.x? ridges2.y : ridges2.z;

  return vec4(col, 1. - bump);
}

vec4
simplexWeave (vec3 q)
{
  vec2 p = q.xy;

  vec2 s = floor(p + (p.x + p.y)*.36602540378);

  p -= s - (s.x + s.y)*.211324865;

  float i = p.x < p.y? 1. : 0.;
  vec2 ioffs = vec2(1. - i, i);

  vec2 p1 = p - ioffs + .2113248654, p2 = p - .577350269; 

  float dh = hash21((s*3. + ioffs + 1.));
  if(dh<1./3.)
  {
    vec2 ang = p;
    p = p1, p1 = p2, p2 = ang;
  }
  else if(dh<2./3.)
  {
    vec2 ang = p;
    p = p2, p2 = p1, p1 = ang;
  }

  vec3 a = vec3(atan(p.y, p.x), atan(p1.y, p1.x), atan(p2.y, p2.x));

  float mid = dist((p2 - p))*.5;

  const float aNum = 6.;
  vec3 ia = (floor(a/6.283*aNum) + .5)/aNum;
  ia += .25/aNum;

  vec2 px = rot2(ia.x*6.283)*p;
  px.x -= mid;

  vec2 py = rot2(ia.y*6.283)*p1;
  py.x -= mid;

  vec2 pz = rot2(ia.z*6.283)*p2;
  pz.x -= mid;

  px = abs(px);
  py = abs(py);
  pz = abs(pz);
   
  px.x = abs(px.x - .08);
  py.x = abs(py.x - .08);
  pz.x = abs(pz.x - .08);

  float cdx = tube4(px) - .02;
  float cdy = tube4(py) - .02;
  float cdz = tube4(pz) - .02;
 
  float bdx = pylon(px);
  float bdy = pylon(py);
  float bdz = pylon(pz);


  a = sin(a*3. - 6.283/9.)*.5 + .5;

  q.z -= .5;

  vec2 d1 = vec2(1e5), d2 = vec2(1e5), d3 = vec2(1e5);

  float depth = .125;
  vec2 v1 = vec2(tubeOuter(p.xy) - mid, q.z - depth); // Bottom level arc.
  vec2 v2 = vec2(tubeOuter(p1.xy) - mid, q.z + depth*a.y - depth); 
  vec2 v3 = vec2(tubeOuter(p2.xy) - mid, q.z); // Top level arc.

  d1 = tube(v1);
  d2 = tube(v2);
  d3 = tube(v3);

  cdx = max(cdx, abs(q.z) - .03);
  cdy = max(cdy, abs(q.z) - .03);
  cdz = max(cdz, abs(q.z) - .03);

  q.z -= .475;//.465;

  bdx = max(bdx, abs(q.z) - .45);
  bdy = max(bdy, abs(q.z) - .45);
  bdz = max(bdz, abs(q.z) - .45);
  bdx = min(min(bdx, bdy), bdz);

  vec3 cir = vec3(dist(p), dist(p1), dist(p2));
  vec3 tor =  abs(cir - mid) - .21;

  float hole0 = 0.;
#ifdef FLOOR_HOLES
  hole0 = -min(tor.x, tor.y);
#endif

  d1.x = min(d1.x, cdx);
  d2.x = min(d2.x, cdy);
  d3.x = min(d3.x, cdz);

  d1.xy = min(min(d1.xy, d2.xy), d3.xy);

  return vec4(hole0, bdx, d1.xy);
}

vec4 objID;

float
map (vec3 p)
{
  float terr = .75 - p.z;

  vec4 hm = simplexWeave(p);

#ifdef FLOOR_HOLES
  terr = min(terr + .05, max(terr, -hm.x));
#endif

#ifdef FLOOR_PATTERN_ONLY
  hm += 1e5;
#endif

  objID.xyzw = vec4(terr, hm.y, hm.z, hm.a);

  return min(terr, min(min(hm.y, hm.z), hm.a));
}

float
bumpFunction (vec3 p)
{
  vec4 sw = simplexWeaveBump(p);

  float bump;
  if(svObjID == 0.)
    bump = sw.x;
  else
    bump = sw.w/4.;

  return bump;
}

vec3
doBumpMap (vec3 p, vec3 n, float bumpfactor)
{
  vec2 e = vec2(2./iResolution.y, 0); 

  float fx = bumpFunction(p - e.xyy);
  float fy = bumpFunction(p - e.yxy);
  float fz = bumpFunction(p - e.yyx);

  float fx2 = bumpFunction(p + e.xyy);
  float fy2 = bumpFunction(p + e.yxy);
  float fz2 = bumpFunction(p + e.yyx);

  vec3 grad = vec3(fx - fx2, fy - fy2, fz - fz2)/(e.x*2.);  
  grad -= n*dot(n, grad);          

  return normalize(n + grad*bumpfactor);
}

vec3
getNormal (vec3 p, inout float edge, inout float crv, float ef)
{
  vec2 e = vec2(ef/mix(450., iResolution.y, .5), 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.0025, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 3., occ = 0.0;
  for( int i=1; i<6; i++ )
  {
    float hr = float(i)*.125/5.;        
    float dd = map(p + hr*n);
    occ += (hr - dd)*sca;
    sca *= .75;
  }

  return clamp(1. - occ, 0., 1.);   
}

float
getShad (vec3 ro, vec3 n, vec3 lp)
{
  const float eps = .001;

  float t = 0., sh = 1., dt;

  ro += n*eps*1.1;

  vec3 ld = (lp - ro);
  float lDist = length(ld);
  ld /= lDist;

  for(int i = min(0, iFrame); i<24; i++)
  {
    dt = map(ro + ld*t);

    sh = min(sh, 8.*dt/t);

    t += clamp(dt, .01, .25);
    if(dt<0. || t>lDist)
      break;
  }

  return max(sh, 0.);
}

float
trace (vec3 ro, vec3 rd)
{
  float d, t = 0.; 
  for(int j = min(0, iFrame); j<64; j++)
  {
    d = map(ro + rd*t);
    if(abs(d)<.001*(1. + t*.125) || t>FAR)
      break;

    t += d;
  }

  return min(t, FAR);
}

vec3
envMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd;

  rd *= 2.5;

  float c = n3D(rd)*.57 + n3D(rd*2.)*.28 + n3D(rd*4.)*.15;
  c = smoothstep(.4, 1., c);

  vec3 col = vec3(c, c*c, c*c*c*c);

  return mix(col, col.zyx, sRd*.25 + .5); 
}

void
main ()
{
  vec3 rd = normalize(vec3(gl_FragCoord.xy - iResolution.xy*.5, iResolution.y*.525));
  rd = normalize(vec3(rd.xy, rd.z - length(rd.xy)*.05));

  float tm = iTime*.75;

  rd.xy *= rot2(sin(tm/4.)*.25);
  rd.yz *= rot2(-.35);

  vec3 ro = vec3(tm, cos(tm/4.), -.5);
  vec3 lp = ro + vec3(cos(tm/2.)*.5, sin(tm/2.)*.5 + .25, -.25);
  float t = trace(ro, rd);

  svObjID = objID.x<objID.y && objID.x<objID.z && objID.x<objID.w? 0. : 
  objID.y<objID.z && objID.y<objID.w? 1. : objID.z<objID.w? 2. : 3.;

  vec3 sp = ro + rd*t;
  float edge = 0., crv = 1., ef = 4.;
  vec3 sn = getNormal(sp, edge, crv, ef);

  if(svObjID==0. || svObjID==3.)
    sn = doBumpMap(sp, sn, .02/(1. + t*.5/FAR));

  vec3 ld = lp - sp;
  float lDist = max(length(ld), 0.001);
  float atten = 1./(1. + lDist*lDist*.1);
  ld /= lDist;

  vec3 oC0 = tex3D(iChannel0, sp, sn)*1.5;
  vec3 oC1 = tex3D(iChannel1, sp/2., sn)*1.5;

  if(svObjID != 3.) oC1 = smoothstep(0., .9, oC1);

  vec3 oC; 

  vec4 sw = simplexWeaveBump(sp);

  if(svObjID == 0.)
  {
    oC1 = mix(oC1, oC0, .25);
    oC1 *= (sw.xyz*.95 + .05);
    oC = oC1;
  }
  else if(svObjID == 1.)
    oC = vec3(.375)*oC1;
  else if(svObjID == 2.)
  {
    oC1 = mix(oC1, oC0, .15);

    if(sp.z<.47)
      oC = oC1;
    else
      oC = oC1/2.;
  }
  else
  {
    oC = vec3(1, .05, .1)*2.5;  
    oC = mix(oC, oC.yzx, dot(sin(sp*4. - cos(sp.yzx*4.))*.5 + .25, vec3(1./3.)*.375));
     
    oC0 *= (sw.a*.9 + .15);
    oC1 *= (sw.a*.9 + .15);
    oC *= oC1;
  }

  float diff = max(dot(ld, sn), 0.);
  float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 16.);
  float fre = clamp(dot(sn, rd) + 1., .0, 1.);
  if(svObjID == 2.) diff = pow(diff, 2.)*1.25;

  float sh = getShad(sp, sn, lp);
  float ao = calcAO(sp, sn);
  sh = min(sh + ao*.3, 1.);

  vec3 col = (oC*(diff + .3*ao) + vec3(.5, .7, 1)*spec*2.);// + vec3(.1, .3, 1)*pow(fre, 4.)*4.;

  vec3 eMap = envMap(reflect(rd, sn), sn);
  if(svObjID == 3.)
    col += oC0*eMap*5.;
  else if(svObjID == 0.)
    col += oC1*eMap*3.;
  else
    col += (oC1 + .5)*eMap*3.5;

  col *= 1. - edge*.7;

  col *= (atten*sh*ao);

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
