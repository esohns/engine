uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 50.

#define SHOW_DETAILS
//#define DARK

float svObjID;
float objID;

#define TUN 0. // Tunnel: Basically, the metal plates.
#define FLR 1. // Floor:  Concrete curbs and underflooring.
#define MTL 2. // Metal:  The metallic mesh and the light casings.
#define LGT 3. // Lights: The lights attached to the top mesh.
#define BLT 4. // Bolts:  The hexagonal bolts.
#define PIP 5. // Pipes:  The cyclinders beside the lights.

mat2 rot (float th) { vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

float
fBm (vec3 p)
{
  return n3D(p)*.57 + n3D(p*2.)*.28 + n3D(p*4.)*.15; 
}

vec3
camPath (float t)
{
  float a = sin(t*3.14159265/16. + 1.5707963*1.);
  float b = cos(t*3.14159265/16.);

  return vec3(a*2., b*a, t);    
}

vec2
objMin (vec2 a, vec2 b)
{
  float s = step(a.x, b.x);
  return s*a + (1. - s)*b;
}

vec3 glow;

float
map (vec3 p)
{
  const float depth = .1;

  p.xy -= camPath(p.z).xy;

  float tun = (1. + depth) - length(p.xy); 

  float flr = p.y + .695;
  flr = max(flr, tun - depth - .1);

  vec3 q = p;
  vec3 q2 = p;

  float a = atan(q.y, q.x)/6.2831853;
  float ia = (floor(a*6.) + .5)/6.*6.2831853;
  float ia2 = (floor(a*18.) + .5)/18.*6.2831853;

  q.xy *= rot(ia + sign(mod(q.z + .25, 1.) - .5)*3.14159/18.);
  q2.xy *= rot(ia2); // Polar conversion for 18 segments (for the bolts).
   
  q.x = mod(q.x, 2.) - 1.;
  q.z = mod(q.z, .5) - .25; 

  q2.x = mod(q2.x, (2. + depth)) - (2. + depth)/2.;

  q = abs(q);
  q2 = abs(q2);

  float blt = max(max(q2.x*.866025 + q2.y*.5, q2.y) - .02, q.z - .08);

  q2 = p;
  q2.xy *= rot(ia - sign(p.x)*3.14159/18.);
  q2 = abs(q2);

  float tunDetail = max(min(min(q.y - .06, q.z - .06), max(q2.y - .06, p.y)), 
                        -min(min(q.y - .01, q.z - .01), max(q2.y - .01, p.y))); 

  tun = min(tun, max(tunDetail, tun-depth));  

  q = abs(p);
  float mtl = max(q.x - .14, abs(p.y - .88) - .02);  // Top mesh.
  mtl = min(mtl, max(q.x - .396, abs(p.y + .82) - .02)); // Bottom mesh.//.81

  q.z = abs(mod(p.z, 2.) - 1.);

  float lgt = max(max(q.x - .07, q.z - .07), abs(p.y - 1.) - .255);

  float lgt2D = length(vec3(q.x, p.y - 1. + .255, q.z));

  float casings = max(max(q.x - .1, q.z - .1), abs(p.y - 1.) - .23);

  q.xz = abs(mod(q.xz, 1./8.) - .5/8.);

  mtl = max(mtl, -max(max(q.x - .045, q.z - .045), abs(abs(p.x) - .19) - .14)); // Holes in the mesh.
  mtl = min(mtl, casings ); // Add the light casings to the top mesh.

  q = p;
  const float th = 6.283/18.;
  float sx = sign(p.x);
  float pip = length(q.xy - vec2(sx*sin(th*1.4), cos(th*1.4))*1.05) - .015;
  pip = min(pip, length(q.xy - vec2(sx*sin(th*1.6), cos(th*1.6))*1.05) - .015);

  vec2 d = objMin(vec2(tun, TUN), vec2(blt, BLT));
  d = objMin(d, vec2(mtl, MTL));
  d = objMin(d, vec2(lgt, LGT));
  d = objMin(d, vec2(flr, FLR));
  d = objMin(d, vec2(pip, PIP));

  if(lgt2D<.35 && lgt2D - .35<d.x)
  {
    glow += vec3(2, 1, .5)*.003/(.001 + lgt2D);
  }

  objID = d.y;

  return d.x;
}

float
trace (vec3 ro, vec3 rd)
{
  float rnd = fract(sin(dot(ro + rd, vec3(57, 41, 113)))*43758.5453);
  float d, t = rnd*.25;

  glow = vec3(0);

  for (int i=0; i<96; i++)
  {
    d = map(ro + rd*t);
    if(abs(d)<.001*(t*.125 + 1.) || t>FAR)
      break;
    t += min(d, .5);
  }

  return min(t, FAR);
}

vec3
tex3D (sampler2D channel, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(channel, p.yz).xyz;
  vec3 ty = texture(channel, p.xz).xyz;
  vec3 tz = texture(channel, p.xy).xyz;

  return tx*tx*n.x + ty*ty*n.y + tz*tz*n.z;
}

float
bumpFunction (vec3 p)
{
  p.xy -= camPath(p.z).xy;

  float res = 0.;
  if(svObjID==FLR)
  {
    p.xy *= 16.;
    res = n3D(p*4.)*.66 + n3D(p*8.)*.34;
    res = 1.-abs(res - .75)/.75;
  }
  else if(svObjID==TUN)
  {
    res = n3D(p*16.)*.66 + n3D(p*32.)*.34;
  }

  if(svObjID==MTL)
  {
    p.xz = abs(mod(p.xz + 1./8.,  1./8.) - .5/8.);
    res = max(p.x, p.z) - .25/8.;

    res = max(res*8., 0.);
  }

  return res;
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  const vec2 e = vec2(0.001, 0);
  float ref = bumpFunction(p);
  vec3 grad = (vec3(bumpFunction(p - e.xyy),
                    bumpFunction(p - e.yxy),
                    bumpFunction(p - e.yyx) )-ref)/e.x;

  grad -= nor*dot(nor, grad);

  return normalize( nor + grad*bumpfactor );
}

vec3
texBump (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.001, 0);

  mat3 m = mat3( tex3D(tx, p - e.xyy, n), tex3D(tx, p - e.yxy, n), tex3D(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114)*m;
  g = (g - dot(tex3D(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

  return normalize( n + g*bf );
}

vec3
nr (vec3 p, inout float crv, float ef)
{
  vec2 e = vec2(ef/450., 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p);

  crv = clamp((d1 + d2 + d3 + d4 + d5 + d6 - d*6.)*32. + .5, 0., 1.);

  e = vec2(.002, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx); 

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
cao (vec3 p, vec3 n)
{
  float sca = 1., occ = 0.;
  for(float i=0.; i<5.; i++)
  {
    float hr = .01 + i*.5/4.;
    float dd = map(n * hr + p);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp(1.0 - occ, 0., 1.);
}

float
softShadow (vec3 ro, vec3 lp, float k)
{
  const int maxIterationsShad = 20; 

  vec3 rd = (lp-ro);

  float shade = 1.0;
  float dist = 0.05;
  float end = max(length(rd), 0.001);

  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, k*h/dist);
    dist += clamp(h, 0.01, 0.25);

    if (h<0.001 || dist > end)
      break;
  }

  return min(max(shade, 0.) + 0.2, 1.0); 
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  float speed = 2.;
  vec3 ro = camPath(iTime*speed);

  vec3 lk = camPath(iTime*speed + .5);

  vec3 lp = camPath(iTime*speed + 2.25);
  lp.y += .6;

  float FOV = .75;
  vec3 fwd = normalize(lk-ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt);

  vec3 rd = fwd + FOV*(u.x*rgt + u.y*up);
  rd = normalize(vec3(rd.xy, (rd.z - length(rd.xy)*.3)*.7));

  float swivel = camPath(lk.z).x;
  rd.xy = rot(swivel/48. )*rd.xy;
  rd.xz = rot(swivel/16. )*rd.xz;

  float t = trace(ro, rd);
  svObjID = objID;

  vec3 sp = ro + rd*t;

  vec3 pathSp = sp - camPath(sp.z);

  float crv = 1., ef = 12.;
  vec3 sn = nr(sp, crv, ef);

  float sh = softShadow(sp, lp, 16.);
  float ao = cao(sp, sn);

  lp -= sp;
  float lDist = max(length(lp), .0001);
  lp /= lDist;

  float atten = 1./(1.0 + lDist*.25 + lDist*lDist*.025);

  const float tSize0 = 1./2.;
  vec3 tx = tex3D(iChannel0, sp*tSize0, sn);
  tx = smoothstep(0., .5, tx);

  if(svObjID==BLT || svObjID==PIP)
    tx *= vec3(1.25, 1, .75);
  else if(svObjID==TUN)
    tx *= vec3(1., .4, .2);
  else if(svObjID==MTL)
    tx *= pathSp.y<0.? vec3(1.1, .8, .7)/2. : vec3(1.1, .8, .7)/4.;
  else if(svObjID==FLR)
    tx *= vec3(1.5, .78, .62);
  else if(svObjID==LGT)
    tx *= vec3(5, 4, 3);

  float spot = max(2. - length(pathSp.xy - vec2(0, 1.)), 0.)*(cos((sp.z + 1.)*3.14159)*.5+.5);
  spot = smoothstep(.5, 1., spot); 
  tx += (tx)*spot;

#ifdef SHOW_DETAILS
  float bf =.005;
  if(svObjID==FLR || svObjID==MTL)
    bf = .02;
  sn = doBumpMap(sp, sn, bf/(1. + t/FAR));
#endif

  float tbf = .03;
  if(svObjID==LGT)
    tbf = .0;
  sn = texBump(iChannel0, sp*tSize0*2., sn, tbf);

  float slm = fBm(sp*4.);
  float slf = 1.;
  if(svObjID!=TUN && svObjID!=FLR)
    slf = .75;
  tx = mix(vec3(1), vec3(.25, .6, .2), (slm*.75 + .25)*slf)*tx;
  tx = mix(vec3(1), vec3(.25, .4, .15)*.1, (1.-abs(slm - .5)*2.)*.75*slf)*tx;

  float dif = max(dot(lp, sn), 0.);
  float spe = pow(max(dot(reflect(-lp, sn), -rd), 0.), 32.);
  float fre = pow(clamp(dot(rd, sn) + 1., 0., 1.), 2.);

#ifdef DARK
  tx /= 4.;
#endif

  vec3 fc = tx*(dif + .125 + vec3(1, .8, .5)*fre*8. + vec3(1, .9, .7)*spot*4.) + vec3(1, .7, .5)*spe*1.;
  fc *= atten*sh*ao*clamp(crv*1.5, 0., 1.);

  fc += (fc + .35)*glow;

  vec3 bg = vec3(.4, .35, .3);
  fc = mix(fc, bg, smoothstep(0., .95, t/FAR));

  gl_FragColor = vec4(sqrt(clamp(fc, 0., 1.)), 1.0);
}
