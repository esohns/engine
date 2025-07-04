#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

#define EDGES

#define HOLES

#define METAL_AMOUNT .35

#define COLOR_VARIETY 2

//#define RANDOM_MATERIALS

//#define SWIZZLE_PALETTE

//#define GOLD_AMOUNT .35

//#define LINES

int objID;

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

vec3
hash23 (vec2 f)
{
  uvec2 p = floatBitsToUint(f);
  uint  n = 1103515245U*((p.x)^(p.y>>3U));
  uvec3 u3 = uvec3(1, 16807U, 48271U);
  return vec3(((u3*n) >> 1) & uvec3(0x7fffffffU))/float(0x7fffffff);
}

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

vec3 hm3 (vec2 p) { return hash23(p); }

float
opExtrusion (float sdf, float pz, float h, float sf)
{
  vec2 w = vec2( sdf, abs(pz) - h) + sf;
  return min(max(w.x, w.y), 0.) + length(max(w, 0.)) - sf;
}

float scale = 1./1.5;
vec3 gDir;
vec3 gRd;
float gCD;
vec2 gIP;

void
tr (inout vec2 oP, inout vec3 d, inout vec2[3] id, int oID)
{
  vec2 p = oP;

  vec2 offs = vec2(0);

  vec2 ii = floor(p/scale);

#ifndef RANDOM_MATERIALS
  float t = iTime/8. + hash21(vec2(2, ii.x));
  float iT = floor(t);
  float fT = t - iT;

  float dir = mod(ii.x, 2.)<.5? -1. : 1.;
  p.y += dir*(smoothstep(0., .0625, fT) + .25)*scale;
  offs.y = dir*(iT + .25); 
#endif

  vec2 ip = floor(p/scale);
  p -= (ip + .5)*scale;

  ip += offs;
  gIP = ip;
  oP = p;

  float rnd = hash21(ip + .05);
  p *= rot2(floor(rnd*64.)*3.14159/2.);

  float rnd2 = hash21(ip + .1);

  d = vec3(1e5);
  id = vec2[3](vec2(0), vec2(0), vec2(0));

  if(rnd2<.3)
  {
    vec2 q = abs(p);
    float sq = max(q.x, q.y) - .5*scale;

    q = p;
    d.x = (p.x + p.y)*.7071;
    id[0] = ip - .25;

    d.y = max(sq, -d.x);
    id[1] = ip + .25;

    d = max(d, sq);

    oID = 0;
  }
  else if (rnd2<.5)
  {
    vec2 q = p;
    d.x = length(q) - 1./2.*.7071*scale + .05;
    id[0] = ip;

    q = abs(p);
    d.y = max(q.x - .5*scale, q.y - .5*scale);
    d.y = max(d.y, -d.x);
    id[1] = ip + vec2(-.25, 0);

    oID = 1;
  }
  else
  {
    vec2 q = abs(p);
    float sq = max(q.x, q.y) - .5*scale;

    float qrtr = length(p - .5*scale) - .5*scale;
    float semi = length(p - .5*scale) - 1.*scale;

    d.x = qrtr;
    id[0] = ip - .25;

    d.y = max(semi, -qrtr);
    id[1] = ip + vec2(0);

    d.z = max(sq, -semi);
    id[2] = ip + .25;

    d = max(d, sq);

    oID = 2;
  }

  float lw = .015*scale;
  d += lw;

  float rvO = (1./2. - 1./4.*.7071)*scale;

  float rv;

  if(oID == 1)
  {
    vec2 q = abs(p) - rvO;
    rv = length(q);
  }
  else
  {
    vec2 q = p;
    rv = length(q - rvO);
    rv = min(rv, length(q + rvO));

    if(oID == 2)
    {
      vec2 arcR = vec2(rvO, -(1.*scale - lw)/4.);
      rv = min(rv, min(length(p - arcR), length(p.yx - arcR)));
    }
  }

#ifdef EDGES
  float hw = .035;
#else
  float hw = .045;
#endif
 
#ifdef HOLES
  if(hash21(ip + .08)<.35)
    d = max(d, -(rv - hw*scale));
#endif
}

float
m (vec3 p)
{
  float fl = -p.z;

  vec3 d; vec2[3] id; int oID;
  vec2 gP = p.xy;
  tr(gP, d, id, oID);
 
  vec3 bev = min(-d*6., .2)*.03;
  vec3 h3 = hm3(gIP)*.25 + .05;
  float obj = opExtrusion(d.x, p.z, h3.x, .015) - bev.x; 
  obj = min(obj, opExtrusion(d.y, p.z, h3.y, .015) - bev.y); 
  if(oID==0) obj = min(obj, opExtrusion(d.z, p.z, h3.z, .015) - bev.z); 

  vec2 rC = (gDir.xy*scale - gP)/gRd.xy;

  gCD = max(min(rC.x, rC.y), 0.) + .0015;

  objID = fl<obj? 0 : 1;

  return min(fl, obj);
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int iter = 32; 

  ro += n*.0015;

  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.; 
  float end = max(length(rd), 0.0001);
  rd /= end;

  gDir = sign(rd)*.5;
  gRd = rd; 

  for (int i = min(iFrame, 0); i<iter; i++)
  {
    float d = m(ro + rd*t);
    shade = min(shade, k*d/t);
    t += clamp(min(d*.7, gCD), .01, .25); 

    if (d<0. || t>end)
      break;
  }

  return max(shade, 0.); 
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;
  for( int i = min(iFrame, 0); i<5; i++ )
  {
    float hr = float(i + 1)*.15/5.;        
    float d = m(p + n*hr);
    occ += (hr - d)*sca;
    sca *= .7;

    if(sca>1e5)
      break;
  }

  return clamp(1. - occ, 0., 1.);
}

vec3
nr (vec3 p)
{
  const vec2 e = vec2(.001, 0);

  float sgn = 1.;
  float mp[6];
  vec3[3] e6 = vec3[3](e.xyy, e.yxy, e.yyx);
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp[i] = m(p + sgn*e6[i/2]);
    sgn = -sgn;
    if(sgn>2.)
      break;
  }

  return normalize(vec3(mp[0] - mp[1], mp[2] - mp[3], mp[4] - mp[5]));
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
BRDF (vec3 col, vec3 n, vec3 l, vec3 v, float type, float rough, float fresRef)
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

  return (col*diff + spec*PI);
}

void
main ()
{
  vec4 c;
  vec2 u = gl_FragCoord.xy;
  u = (u - iResolution.xy*.5)/iResolution.y;    

  vec3 r = normalize(vec3(u, 1)), o = vec3(0, iTime/2., -3), l = o + vec3(.5, 1, 1.5);
  r.yz = rot2(.15)*r.yz;
  r.xz = rot2(-cos(iTime*3.14159/32.)/8.)*r.xz;
  r.xy = rot2(sin(iTime*3.14159/32.)/8.)*r.xy; 

  if(iMouse.z>0.)
  {
    r.yz *= rot2(-.5*(iMouse.y - iResolution.y*.5)/iResolution.y*3.1459);  
    r.xz *= rot2(-.25*(iMouse.x - iResolution.x*.5)/iResolution.x*3.1459);
  } 

  float d, t = hash21(r.xy*57. + fract(iTime))*.5;

  gDir = sign(r)*.5;
  gRd = r; 

  for(int i = min(iFrame, 0); i<96; i++)
  {
    vec3 p = o + r*t;
    d = m(p);
    if(abs(d)<.001)
      break;
    t += min(d*.9, gCD);
  }

  int gObjID = objID;
 
  vec3 p = o + r*t, n = nr(p); 

  vec3 ld = l - p;
  float lDist = length(ld);
  ld /= lDist;
  float at = 1./(1. + lDist*lDist*.05);

  float sh = softShadow(p, l, n, 8.);
  float ao = calcAO(p, n);

  vec3 d3; vec2[3] vID; int oID;
  vec2 p2 = p.xy;
  tr(p2, d3, vID, oID);

  int index = (d3.x<d3.y && d3.x<d3.z)? 0 : d3.y<d3.z? 1 : 2;

  float obj2D = d3[index];
  vec2 id = vID[index]*scale;

  vec3 h3 = hm3(gIP)*.25 + .05;
  float h = h3[index];
 
  vec3 txP = vec3(p2, p.z);
  vec3 txN = n;
  vec3 tx = tex3D(iChannel1, txP/2., txN);

#ifdef LINES
  float lSc = 24.;
  float pat = (abs(fract((p2.x - p2.y)*lSc - .5) - .5) - .125)/lSc;
#endif  

  float roughness = .2;
  float matType = 0.;
  float reflectance = .5;

  vec3 oCol = vec3(0);

  if(gObjID == 0)
  {
    oCol = vec3(.125);
    matType = 1.;
    roughness = .5;
  }
  else if (gObjID==1)
  {
    vec3 txR = txP;
    txR.xy *= rot2(-3.14159/6.);
    vec3 rTx = tex3D(iChannel2, txR*vec3(.5, 3, .5), txN);
    float rGr = dot(rTx, vec3(.299, .587, .114));

    float sRnd = hash21(id + .01);

    vec3 sCol = .5 + .45*cos(6.2831*sRnd/8. + (vec3(0, 1, 2) + .05));
    sCol = pow(sCol, vec3(1.5));

#if COLOR_VARIETY >= 2
    if(hash21(id + .27)<.35) sCol = sCol.zyx;
#endif

#if COLOR_VARIETY >= 3
#ifdef SWIZZLE_PALETTE
    if(hash21(id + .15)<.35) sCol = mix(sCol, sCol.yxz, .5);
#else
    if(hash21(id + .15)<.35) sCol = mix(sCol, sCol.xzy, .5);
#endif
#endif

    float b = abs(obj2D) - .01;
    float pH = p.z + h - .04;
    b = max(b, (p.z + h - .02));

    float sf = .01;
    float ew = .02;

#ifdef RANDOM_MATERIALS
    float rRnd = hash21(gIP + .11);
    sCol = .5 + .45*cos(6.2831*rRnd + vec3(0, 1, 2)*hash21(gIP + .4)*1.5);
#endif

#ifdef EDGES
    float rW = .035; // Rim width.
            
    oCol = sCol;
    oCol = mix(oCol*.8, oCol*.15, (1. - smoothstep(0., sf, pH)));
    oCol = mix(oCol, mix(min(sCol*1.4, 1.), vec3(1), .2), (1. - smoothstep(0., sf, pH + ew)));
    oCol = mix(oCol, oCol*.15, (1. - smoothstep(0., sf, obj2D + rW)));

#ifdef LINES
    sCol = mix(sCol*1.15, sCol*.7, (1. - smoothstep(0., sf, pat)));
#endif
    oCol = mix(oCol, sCol, (1. - smoothstep(0., sf, obj2D + rW + ew)));
#else
    oCol.xyz = sCol;
#ifdef LINES
    pat = max(pat, pH + .02);
    oCol = mix(oCol, oCol*.15, (1. - smoothstep(0., sf, abs(pH + .02) - ew/2.)));
    oCol = mix(oCol*1.1, oCol.xyz*.7, (1. - smoothstep(0., sf, pat)));
#endif
#endif

    float gr = dot(oCol.xyz, vec3(.299, .587, .114));

    oCol = mix(oCol, oCol.xzy, clamp(-p2.y - .125, 0., 1.));

#if COLOR_VARIETY == 0
    oCol.xyz = vec3(gr*sqrt(gr)*1.5);
#endif

#ifdef SWIZZLE_PALETTE
    oCol = oCol.yzx;
#endif

#ifndef RANDOM_MATERIALS   
    if(hash21(id + .2)<METAL_AMOUNT)
    {
      oCol = mix(oCol, vec3(gr), .9);

#ifdef GOLD_AMOUNT
      if(hash21(id + .43)<GOLD_AMOUNT) oCol.xyz *= vec3(1, .75, .4)*2.5;
#endif

      matType = 1.;
      oCol.xyz *= tx*1. + .5;
      roughness = .6;
      roughness *= rGr*.6 + .4;
    }
    else
    {
      roughness = .3;
      roughness *= (rGr*.4 + .6);
      oCol *= tx*.6 + .9;
    }
#endif

#ifdef RANDOM_MATERIALS
    roughness = hash21(gIP + .31)*(rGr*.5 + .5);
    matType = step(.5, hash21(gIP + .32));
    reflectance = hash21(gIP + .33);
    oCol *= tx*2.5 + .2;
#endif
  }

  float am = pow(length(sin(n*2.)*.5 + .5)/sqrt(3.), 2.)*1.5;

  vec3 ct = BRDF(oCol, n, ld, -r, matType, roughness, reflectance);

  c.xyz = (oCol*am*(sh*.5 + .5) + ct*(sh))*ao*at;
 
  c = vec4(max(c.xyz, 0.), t);  

  gl_FragColor = c;
}
