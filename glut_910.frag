#version 330

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 f)
{
  f = mod(f + 16384., 16384.); 
  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

float
hash31 (vec3 f)
{
  uvec3 p = floatBitsToUint(f);
  p = 1103515245U*((p >> 2U)^(p.yzx>>1U)^p.zxy);
  uint h32 = 1103515245U*(((p.x)^(p.y>>3U))^(p.z>>6U));

  uint n = h32^(h32 >> 16);
  return float(n & uint(0x7fffffffU))/float(0x7fffffff);
}

const float PI = 3.14159265;

float
D_GGX (float NoH, float roughness)
{
  float alpha = pow(roughness, 4.);
  float b = (NoH*NoH*(alpha - 1.) + 1.);
  return alpha/(PI*b*b);
}

float
G1_GGX_Schlick (float NoV, float roughness)
{
  float r = .5 + .5*roughness;
  float k = (r*r)/2.;
  float denom = NoV*(1. - k) + k;
  return max(NoV, .001)/denom;
}

float
G_Smith (float NoV, float NoL, float roughness)
{
  float g1_l = G1_GGX_Schlick(NoL, roughness);
  float g1_v = G1_GGX_Schlick(NoV, roughness);
  return g1_l*g1_v;
}

vec3
BRDF (vec3 col, vec3 n, vec3 l, vec3 v, float type, float rough, float fresRef, vec3 spCol)
{
  vec3 h = normalize(v + l);

  float nv = clamp(dot(n, v), 0., 1.);
  float nl = clamp(dot(n, l), 0., 1.);
  float nh = clamp(dot(n, h), 0., 1.);
  float vh = clamp(dot(v, h), 0., 1.);  

  vec3 f0 = vec3(.16*(fresRef*fresRef)); 
  f0 = mix(f0, col, type);

  vec3 F = f0 + (1. - f0)*pow(1. - vh, 5.);
  float D = D_GGX(nh, rough); 
  float G = G_Smith(nv, nl, rough); 
  vec3 spec = F*D*G/(4.*max(nv, .001));

  vec3 diff = vec3(nl);
  diff *= 1. - F;
  diff *= (1. - type);

  return (col*diff + spCol*spec*PI);
}

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel1;

#define FAR 15.
//#define SHOW_2D_PATTERN

float objID;

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

vec3
getTex (sampler2D iCh, vec2 p)
{
  vec3 tx = texture(iCh, p).xyz;
  return tx*tx;
}

float
hm (vec2 p)
{
  float h = dot(sin(p - cos(p.yx*2.5 + iTime*2.)), vec2(.25)) + .5; 
  return smoothstep(0.2, 1., h)*.7 + h*.3;
}

float
opExtrusion (float sdf, float pz, float h, float sf)
{
  vec2 w = vec2( sdf, abs(pz) - h) + sf;
  return min(max(w.x, w.y), 0.) + length(max(w, 0.)) - sf;
}

float
sBoxS (vec2 p, vec2 b, float rf)
{
  vec2 d = abs(p) - b + rf;
  return min(max(d.x, d.y), 0.) + length(max(d, 0.)) - rf;
}

vec3 gDir;
vec3 gRd;
float gCD;
vec2 gP;
vec3 gSc = vec3(1, 1, 1)/4.;
vec2 cntr = vec2(0);
vec2 dim = vec2(0);
float gD;

vec4
blocks (vec2 p)
{
  vec2 sc = gSc.xy;

  vec2 ip = floor(p/sc);
  p -= (ip + .5)*sc;

  mat2x2 eID = mat2x2(vec2(-.5, 0), vec2(0, .5));

  vec2 oFctr = .65*sc;

  vec2 rnd, rndN;
  vec2 dir;

  float check = mod(ip.x + ip.y, 2.)<.5? -1. : 1.;

  if(check<0.)
  {
    rnd.x = (hash21(ip) - .5)*oFctr.x;

    dir.x = p.x<rnd.x? -1. : 1.;

    rnd.y = (hash21(ip + dir*vec2(1, 0)) - .5)*oFctr.y;

    dir.y = p.y<rnd.y? -1. : 1.;

    rndN.x = hash21(ip + dir*vec2(1, 1));

    rndN.y  = hash21(ip + dir*vec2(0, 1));
  }

  if(check>0.)
  {
    rnd.y = (hash21(ip) - .5)*oFctr.y;
    dir.y = p.y<rnd.y? -1. : 1.;

    rnd.x = (hash21(ip + dir*vec2(0, 1)) - .5)*oFctr.x;
    dir.x = p.x<rnd.x? -1. : 1.;

    rndN.x = hash21(ip + dir*vec2(1, 0));

    rndN.y = hash21(ip + dir*vec2(1, 1));
  }

  rndN = (rndN - .5)*oFctr;
  rndN += dir*sc;

  cntr = mix(rnd, rndN, .5);
  dim = -dir*(rnd - rndN);

  ip = ip*sc + cntr; 

  return vec4(p - cntr, ip);
}

vec4 gVal;
vec3 glow;

float
map (vec3 p)
{
  float fl = -p.z;

  vec4 d4 = blocks(p.xy);

  vec2 p2 = d4.xy;
  vec2 id = d4.zw;

  float d2 = sBoxS(p2, dim/2. - .0025, .025);

  float h = hm(id);
  h = h*1.5 + .05;
  if(hash21(id + .43)<.5) h = h*.25;

  float minSc = min(gSc.x, gSc.y);
  if(dim.x>gSc.x && dim.y>gSc.y)
    d2 = max(d2, -(d2 + minSc/2.));

  float d = opExtrusion(d2, p.z + h/2., h/2., .01);

  d -= smoothstep(0., .007, abs(d2 + .035))*.005;

  d += (d2 + .035)*.15;

  const float lNum = 8.;
  float z = p.z + h;
  float ln = (abs(fract(z*lNum + .5) - .5) -.25)/lNum;
  d += smoothstep(0., 1./64., ln)*.01;

  vec2 q = p2;
  if(dim.x>gSc.x/1.2) q.x = abs(q.x + sign(p2.x)*.07) - dim.x/2.;
  if(dim.y>gSc.y/1.2) q.y = abs(q.y + sign(p2.y)*.07) - dim.y/2.;
  float dR = length(vec3(q, p.z + h + .015)) - .02;

  glow += max(p.z + h, 0.);

  vec2 rC = (gDir.xy*dim.xy - p2)/gRd.xy;

  gCD = min(rC.x, rC.y) + .0001;

  gVal = vec4(d2, d, id);
  gSc = vec3(gSc.xy, h);
 
  objID = fl<d && fl<dR? 0. : d<dR? 1. : 2.;

  return  min(fl, min(d, dR));
}
 
float
trace (vec3 ro, vec3 rd)
{
  float d, t = hash31(ro + rd)*.1;

  gDir = step(0., rd) - .5;
  gRd = rd; 

  glow = vec3(0);

  int i;
  const int iMax = 128;
  for (i = min(iFrame, 0); i<iMax; i++)
  {
    d = map(ro + rd*t);       

    if(abs(d)<.001 || t>FAR) break; 

    t += min(d, gCD); 
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p, float t)
{
  float sgn = 1.;
  vec3 e = vec3(.001, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += map(p + sgn*e)*sgn;
    sgn = -sgn;
    if((i&1)==1){ mp = mp.yzx; e = e.zxy; }
  }

  return normalize(mp);
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  ro += n*.0015;
  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.; 
  float end = max(length(rd), .0001);
  rd /= end;

  gDir = sign(rd)*.5;
  gRd = rd;

  for (int i = min(iFrame, 0); i<48; i++)
  {
    float d = map(ro + rd*t);

    shade = min(shade, k*d/t);

    t += clamp(min(d, gCD), .01, .25); 

    if (d<0. || t>end) break; 
  }

  return max(shade, 0.); 
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;
  for(int i = 0; i<5; i++)
  {
    float hr = float(i + 1)*.2/5.;        
    float d = map(p + n*hr);
    occ += (hr - d)*sca;
    sca *= .7;
  }

  return clamp(1. - occ, 0., 1.);    
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  vec3 ro = vec3(iTime/2., 0, -3);
  vec3 lk = ro + vec3(.0, .1, .25);

  vec3 lp = ro + vec3(1, 1, 0);

  float FOV = 1.25;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x ));
  vec3 up = cross(fwd, rgt); 

  vec3 rd = normalize(uv.x*rgt + uv.y*up + fwd/FOV);

  rd.xy *= rot2(-.15);    

  float t = trace(ro, rd);

  vec4 svVal = gVal;

  float svObjID = objID;

  vec3 svSc = gSc;
  vec2 svP = gP;

  vec3 svGlow = glow;

  vec3 col = vec3(0);

  if(t < FAR)
  {
    vec3 sp = ro + rd*t;
    vec3 sn = getNormal(sp, t);
    vec3 ld = lp - sp;
    float lDist = max(length(ld), .001);
    ld /= lDist;

    vec3 oCol; 

    float roughness = 1.;
    float matType = 1.;
    float reflectance = .25;

    if(svObjID>.5)
    {
      float rnd = hash21(svVal.zw + .34);
      vec3 sCol = .5 + .45*cos(6.2831853*rnd/7. + vec3(0, 1, 2)*1.5 - .5);
      sCol = mix(sCol, sCol*vec3(4, 1, .5), smoothstep(0., 1., sp.z + svSc.z));
      sCol += sCol*svGlow/4.;

      if(svObjID==2.) sCol = sCol/2.; 

      vec3 tx = getTex(iChannel1, svVal.zw);
      vec3 tx2 = tex3D(iChannel1, sp - vec3(0, 0, -svSc.z), sn);
      oCol = sCol*(tx2*2.);

      float grT = dot(tx2, vec3(.299, .587, .114)); 
      roughness *= grT*grT*4. + .2;

      if(svObjID==2.) roughness *= 1.5;
    }
    else
      oCol = vec3(4, 1, .5)*svGlow/4.; 

    float sh = softShadow(sp, lp, sn, 16.);
    float ao = calcAO(sp, sn);

    float atten = 1./(1. + lDist*.05);
    float am = length(sin(sn*2.)*.5 + .5)/sqrt(3.)*smoothstep(-1., 1., -sn.z); 

    vec3 lCol = mix(vec3(4, 1, .5), vec3(1, 2, 8), smoothstep(0., 1., -sp.z + svSc.z)*.5 + .5);

    vec3 ct = BRDF(oCol, sn, ld, -rd, matType, roughness, reflectance, lCol);

    col = (oCol*am*(.75 + sh*.25) + ct*(sh));        

    col *= ao*atten;
  }

#if 1 
  col = mix(col, (col + .5)*vec3(4, 2, 1), smoothstep(.15, .99, t/FAR));
  float ux = gl_FragCoord.x/iResolution.x;
  col = mix(col, mix(col.zyx, col.xzy, ux), smoothstep(0., 1., uv.y + .25)); 
#else
  col = mix(col, (col + .5)*vec3(1, 2, 4), smoothstep(.15, .99, t/FAR));
#endif

#ifdef SHOW_2D_PATTERN
  vec4 b = blocks(uv*4. + vec2(0, iTime/4.));
  float rnd = hash21(b.zw + .34);
  vec3 sCol = .5 + .45*cos(6.2831853*rnd/1. + vec3(0, 1, 2)*1.5);
  float bx = sBoxS(b.xy, dim/2., .02);
  col = mix(vec3(0), sCol, 1. - smoothstep(0., 1./iResolution.y, bx/4. + .003));
  t = 3.;
#endif

  gl_FragColor = vec4(max(col, 0.), t);
}
