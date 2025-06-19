#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

// glut_838_common.glsl
#define PI 3.14159265
#define TAU 6.2831853 

#define SUBDIV_PILLAR
#define SUBDIV_TRI
// Pink and green: 0, Blue and purple: 1
#define COLOR 1
//#define FLAT_TOP

//#define SHOW_GRID

float tm;

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash21 (vec2 f)
{
  uvec2 p = floatBitsToUint(f + 16384.);
  p = 1664525U*(p>>1U^p.yx);
  return float(1103515245U*(p.x^(p.y>>3U)))/float(0xffffffffU);
}

float
distLineS (vec2 p, vec2 a, vec2 b)
{
  p -= a, b -= a;
  float h = clamp(dot(p, b)/dot(b, b), 0., 1.);
  float s = b.x*p.y<b.y*p.x? -1. : 1.;
  return length(p - b*h)*s;
}

const float gSc = 1./2.;
#ifdef FLAT_TOP
const vec2 s = vec2(1.732, 1)*gSc;
#else
const vec2 s = vec2(1, 1.732)*gSc;
#endif
vec2[6] v, e;

#ifdef FLAT_TOP
const vec2[6] vID = vec2[6](vec2(-4, 0), vec2(-2, 6), vec2(2, 6), 
                            vec2(4, 0), vec2(2, -6), vec2(-2, -6)); 

const vec2[6] eID = vec2[6](vec2(-3, 3), vec2(0, 6), vec2(3), 
                            vec2(3, -3), vec2(0, -6), vec2(-3));
#else
const vec2[6] vID = vec2[6](vec2(-6, -2), vec2(-6, 2), vec2(0, 4), 
                            vec2(6, 2), vec2(6, -2), vec2(0, -4));

const vec2[6] eID = vec2[6](vec2(-6, 0), vec2(-3, 3), vec2(3, 3), vec2(6, 0), 
                            vec2(3, -3), vec2(-3, -3));
#endif

vec4
getGrid (vec2 p)
{
  vec4 ip = floor(vec4(p/s, p/s - .5));
  vec4 q = p.xyxy - vec4(ip.xy + .5, ip.zw + 1.)*s.xyxy;
  return dot(q.xy, q.xy)<dot(q.zw, q.zw)? vec4(q.xy, ip.xy*12.) : vec4(q.zw, ip.zw*12. + 6.);
}

vec2 gP;
float gPoly;
float gVert;

vec4
getPattern (vec2 p)
{
  vec4 p4 = getGrid(p);

  vec2 id0 = p4.zw;

  float ttm = tm/8. + .5;
  float a = (mod(floor(ttm), 2.) + smoothstep(0., .2, fract(ttm)))/2.;
  if(fract(ttm/2.)<.5) a = 1. - a;

  mat2 mR = rot2(-PI/7.*a)*1.8;

  float cirR = length(mR*e[0] - v[1]*2.);

  float[6] vArc;  
  for(int i = 0; i<6; i++)
  {
    vec2 rndOffsPnt = mR*e[i];
    vArc[i] = length(p4.xy - rndOffsPnt) - cirR;
  }

  float poly = -1e5;

  int pID = 0;

  int polygonID = 6;
 
  for(int i = 0; i<6; i++)
  {
    float arcI = max(vArc[i], -vArc[(i + 5)%6]);

    if(arcI<0.)
    {
      poly = arcI;
      pID = i;

      polygonID = i;

      pID = pID%12; 
      break;
    }

    poly = max(poly, -arcI);
  }

  if(polygonID<6)
  {
    int i = polygonID;

    vec2 cC = mR*e[(i + 2)%6];
    cC += e[i]*2.;
    float lnOuter = -(length(p4.xy - cC) - cirR);

    if(lnOuter<0.)
    {
      poly = max(poly, lnOuter);
      p4.zw = id0 + eID[(i + 0)%6];
      pID = 1;
    }
    else
    {
      poly = max(poly, -lnOuter);
  
      p4.zw = id0 + vID[(i + 1)%6];

      pID = 2;
    }

#ifdef SUBDIV_PILLAR
    if(pID==1)
    {
      vec2 cC = mR*e[(i + 1)%6]; 
      cC += e[(i + 5)%6]*2.;
      float lnn = length(p4.xy - cC) - cirR;
      float ln2 = -lnn; 

      if(ln2<0.)
      {
        poly = max(poly, ln2);
        p4.zw += eID[(i + 4)%6]/2. + vID[(i + 3)%6]/8.;
      }
      else
      {
        poly = max(poly, -ln2);

        vec2 cC = mR*e[(i + 4)%6];
        cC += e[(i + 1)%6]*2.;
        float lnn = length(p4.xy - cC) - cirR;
        poly = max(poly, (lnn));          

        pID = 3;
      }
    }
#endif

#ifdef SUBDIV_TRI
    if(pID==2)
    {
      vec3 lnV;
      vec2 cC;
      float lnn;

      cC = (mR)*e[(i + 3)%6]; 
      cC += e[i]*2. + e[(i + 1)%6]*2.; 
      lnn = length(p4.xy - cC) - cirR;
      lnV.x = lnn;

      cC = (mR)*e[(i + 1)%6];
      cC += e[(i + 5)%6]*2.;
      lnn = length(p4.xy - cC) - cirR;
      lnV.y = lnn;

      cC = (mR)*e[(i + 5)%6];
      cC += e[(i + 2)%6]*2.;
      lnn = length(p4.xy - cC) - cirR;
      lnV.z = lnn;

      lnV = max(lnV, -lnV.zxy);
      ivec3 ind = ivec3(3, 1, 5);

      for(int j = 0; j<3; j++)
      {
        float ln2 = lnV[j];
        if(ln2<0.)
        {
          poly = max(poly, ln2);
          p4.zw += eID[(i + ind[j])%6]/4.5;
          break;
        }
      } 
    }
#endif
  }

#ifdef SHOW_GRID
  float vert = 1e5;
  float hexagon = -1e5; 

  for(int i = 0; i<2; i++)
  {
    int ii = int(mod((atan(p4.x, p4.y)/TAU + 1./3.)*6. + float(i), 6.));

    hexagon = max(hexagon, distLineS(p4.xy, v[ii], v[(ii + 1)%6]));

    vert = min(vert, length(p4.xy - v[ii]) - .02); 
  }

  gPoly = min(abs(hexagon) - .005, vert);
  gPoly = max(gPoly, -vert - .01);
#else
  gPoly = 1e5;
#endif

  gP = p4.xy;

  return vec4(poly, p4.zw, pID);
}

float
D_GGX (float NoH, float roughness)
{
  float alpha = pow(roughness, 4.);
  float b = (NoH*NoH*(alpha - 1.) + 1.);
  return alpha/(3.14159265*b*b);
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

  return (col*diff + spec*3.14159265);
}
// glut_838_common.glsl

#define FAR 10.

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

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p);
  h = mix(fract(sin(mod(h, TAU))*43758.5453), 
          fract(sin(mod(h + s.x, TAU))*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z);
}

float
opExt (float sdf, float pz, float h, float sf)
{
  vec2 w = vec2(sdf, abs(pz) - h) + sf;
  return min(max(w.x, w.y), 0.) + length(max(w, 0.)) - sf;
}

vec4 gVal;
int gID;
 
float
heightMap (vec3 p)
{
  vec4 d4 = getPattern(p.xy);

  float poly = d4.x;
  int pID = int(d4.w);
  vec2 id = d4.yz;

  gVal = vec4(poly, id, pID);

  return poly;
}

int gHexBord;

float
m (vec3 p)
{
  float h = heightMap(p);

  float fl = -p.z;

  float edge2D = abs(h + .008) - .005;
  float edge = opExt(edge2D, p.z + .0525/2., .0525/2., .001);

  float poly2D = h + .005*2. + .008;
  float poly = opExt(poly2D, p.z + .025/2., .025/2., .001);

  poly += poly2D*.25;

#ifdef SHOW_GRID
  float edge2 = max(gPoly, abs(p.z + .035) - .035);
  gHexBord = edge2<poly? 1 : 0;
  poly = min(poly, edge2);
#endif

  gID = fl<edge && fl<poly? 0 : edge<poly? 1 : 2;

  return min(fl, min(edge, poly));
}

float
trace (vec3 ro, vec3 rd)
{
  float d, t = 0.;

  vec2 dt = vec2(1e5, 0);

  int i;
  const int iMax = 128;
  for (i = min(iFrame, 0); i<iMax; i++)
  {
    d = m(ro + rd*t);       
    dt = d<dt.x? vec2(d, dt.x) : dt;

    if(abs(d)<.001 || t>FAR)
      break;

    t += d*.9; 
  }

  if(i>=iMax - 1)
    t = dt.y;

  return min(t, FAR);
}

vec3
nr (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.0025, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += m(p + sgn*e)*sgn;
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
softShadow (vec3 ro, vec3 rd, float lDist, float k)
{
  const int iters = 32; 

  float shade = 1.;
  float t = 0.; 

  for (int i = min(iFrame, 0); i<iters; i++)
  {
    float d = m(ro + rd*t);

    shade = min(shade, k*d/t);
    t += clamp(d*.8, .01, .25); 

    if (d<0. || t>lDist)
      break;
  }

  return max(shade, 0.); 
}
 
float
cAO (vec3 p, vec3 n)
{
  float sca = 3., occ = 0.;
  for(int i = 0; i<5; i++)
  {
    float hr = .01 + float(i)*.25/4.;        
    float dd = m(n * hr + p);
    occ += (hr - dd)*sca;
    sca *= .75;
  }

  return clamp(1. - occ, 0., 1.);    
}

void
main ()
{
  vec4 c;
  vec2 u = gl_FragCoord.xy;

  u = (u - iResolution.xy*.5)/iResolution.y;
    
  tm = iTime;

  u *= .94 + dot(u, u)*.08;

  vec3 r = normalize(vec3(u, 1)), 
       o = vec3(iTime/8., iTime/16., -1), l = vec3(.35, .5, 0);

  r.yz *= rot2(-.1);
  r.xz *= rot2(-.1);
  l.yz *= rot2(-.1);
  l.xz *= rot2(-.1);
  l += o;

  vec2 sDiv12 = s/12.;

  for(int i = 0; i<6; i++)
  {
    v[i] = vID[i]*sDiv12;
    e[i] = eID[i]*sDiv12;
  }

  float t = trace (o, r);
 
  int svID = gID;
  vec4 svVal = gVal;

  int svHexBord = gHexBord;

  vec3 col = vec3(0);

  if(t<FAR)
  {
    vec3 p = o + r*t, n = nr(p);

    l -= p;
    float lDist = max(length(l), .001);
    l /= lDist;

    float atten = 1./(.5 + lDist*lDist*.25);

    float sh = softShadow(p + n*.0015, l, lDist, 16.);
    float ao = cAO(p, n);

    vec3 tx = tex3D(iChannel0, (p), n);
    vec3 oCol = vec3(.05);
    vec3 inGCol = vec3(2.5, 2, 1.5);

    if(svID==2)
    {
      float colID = hash21(svVal.yz + .1);

#if COLOR == 1
      vec3 pCol = .5 + .45*cos(TAU*colID/8. + vec3(0, 1, 2)*1.5);
      if(svVal.w == 1.){ oCol = pCol.zyx; }
      if(svVal.w == 2.){ oCol = mix(pCol.yzx, pCol, .0); }
#else
      vec3 pCol = .5 + .45*cos(TAU*colID/6. + vec3(0, 1, 2) + .5);
      inGCol = inGCol.zyx;
      if(svVal.w == 1.){ oCol = mix(pCol.xzy, pCol, .2);  }
      if(svVal.w == 2.){ oCol = mix(pCol.yxz, pCol.zyx, .2);  }
#endif

      if(svVal.w == 0.){ oCol = vec3(.25)*dot(pCol, vec3(.299, .587, .114)); }
      if(svVal.w == 3.){ oCol = vec3(.25)*dot(pCol, vec3(.299, .587, .114)); }

      if(svHexBord==1) oCol = vec3(.8, .9, 1);
    }

    if(svID==1)
    {
#if COLOR == 1
      oCol = vec3(.7);
#else
      oCol = vec3(.75, .67, .65);
#endif
    }

    oCol = mix(oCol, oCol*(oCol)*inGCol*.5, 1. - smoothstep(0., .03, abs(svVal.x + .018) - .005));

    oCol *= max(-svVal.x/gSc, 0.)*3. + .25;

    oCol *= tx*2. + .5;

    float backFill = max(dot(vec3(-l.xy, 0.), n), 0.);
    float ns0 = n3D(p*3. + iTime/4.);
    ns0 = smoothstep(-.25, .25, ns0 - .5);
    oCol += oCol*mix(vec3(1, .05, .0), vec3(1, .1, .2), ns0*.5)*backFill*4.*sh;

    float ambience = pow(length(sin(n*2.)*.5 + .5), 2.);

#if 1
    float matType = svID==2? 0. : 1.;
    float roughness = dot(tx, vec3(.299, .587, .114));
    float reflectance = roughness*2.;

    roughness *= 1. + matType;

    if(matType==1.) roughness *= .7;

    vec3 ct = BRDF(oCol, n, l, -r, matType, roughness, reflectance);

    col = (oCol*ambience*(sh*.75 + .25) + ct*(sh));
#else
    float df = max(dot(l, n), 0.);
    df = pow(df, 2.) + pow(df, 4.)*.5;
        
    float sp = pow(max(dot(reflect(-l, n), -r), 0.), 32.);

    float gr = dot(tx, vec3(.299, .587, .114));
    if(svID==1) df = pow(df, 4. + 8.*gr)*.5;
    else df *= df + gr;
    col = oCol*(df*sh + sp*sh*2. + ambience*.5);
#endif 

    vec3 hv = normalize(-r + l);
    vec3 ref = reflect(r, n);
    vec3 tx2 = texture(iChannel1, reflect(r, n)).xyz; tx2 *= tx2;
    float specR = pow(max(dot(hv, n), 0.), 8.);
    float rF = svID==1? 2. : 3.;
    col += col*specR*tx2*rF;

    col *= ao*atten;
  }

  c = vec4(pow(max(col, 0.), vec3(1)/2.2), 1);

  gl_FragColor = c;
}
