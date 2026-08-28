#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

const float tau  = atan(1.)*8.;
const float pi   = tau/2.;
const float tauq = tau/4.;

#define rot(a) mat2(cos((a)*tau + vec4(0.0, tauq, 3.0*tauq, 0.0)))

#define PALETTE_CLICK 1
vec3
paletteSelect (float t, int pal)
{
  if(pal == PALETTE_CLICK)
  {
    vec3 a = vec3(0.55, 0.55, 0.60);
    vec3 b = vec3(0.35, 0.30, 0.25);
    vec3 c = vec3(1.0);
    vec3 d = vec3(0.00, 0.15, 0.25);
    return ( a + 1.3*b*cos(tau*(c*t + d)) );
  }

  vec3 a = vec3(.248, 0.645, 0.135);
  vec3 b = vec3(0., 0.40, 0.135);
  vec3 c = vec3(.5);
  vec3 d = vec3(0.00, 0.10, 0.20)+.2;
  return a + b*cos(tau*(c*t + d));
}

vec3
pickTetraColor (float idx, vec3 c0, vec3 c1, vec3 c2, vec3 c3, vec3 c4)
{
  return (idx < 0.5) ? c0 :
         (idx < 1.5) ? c1 :
         (idx < 2.5) ? c2 :
         (idx < 3.5) ? c3 : c4;
}

float
hash13 (vec3 p)
{
  p = fract(p*0.1031);
  p += dot(p, p.yzx + 33.33);
  return fract((p.x+p.y)*p.z);
}

float
noise3d (vec3 p)
{
  vec3 i = floor(p);
  vec3 f = fract(p);
  f = f*f*(3.0-2.0*f);

  float n000 = hash13(i+vec3(0,0,0));
  float n100 = hash13(i+vec3(1,0,0));
  float n010 = hash13(i+vec3(0,1,0));
  float n110 = hash13(i+vec3(1,1,0));
  float n001 = hash13(i+vec3(0,0,1));
  float n101 = hash13(i+vec3(1,0,1));
  float n011 = hash13(i+vec3(0,1,1));
  float n111 = hash13(i+vec3(1,1,1));

  float nx00 = mix(n000,n100,f.x);
  float nx10 = mix(n010,n110,f.x);
  float nx01 = mix(n001,n101,f.x);
  float nx11 = mix(n011,n111,f.x);

  float nxy0 = mix(nx00,nx10,f.y);
  float nxy1 = mix(nx01,nx11,f.y);

  return mix(nxy0,nxy1,f.z);
}

float
hash12 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * 0.1031);
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.x + p3.y) * p3.z);
}

#define tonemap(x) ((x*1.05)/((x)+vec3(1.)))
vec3
tonemap2 (vec3 o)
{
	float w = 1.;
	float l = dot(o.rgb, vec3(0.2126, 0.7152, 0.0722));
	float L = l * (1. + l / (w*w)) / (1. + l);
	o.rgb *= L / l;
	return tanh( o*.75 )*.9;
}

//////////////////////////////////////////

#define MERKABA_WIRES                    0       // you should reduce TETRA_WIRE_RADIUS if you put this to 1 
#define zoOM                           ( 5./8. )
#define Speed                          ( 4./8. )
const float TETRA_WIRE_RADIUS =          0.03;
const bool  RAYS =                       true;   // fake "rays"
const bool  RAYSsmoootha =               true;
const float refractionIndex =            1.036;
const float IOR_AIR =                    1.00033;

#define PALETTE_IDLE     6
const float GLOW_BASE   = 0.00013;
const float GLOW_COLORK = 2.80;
const float SURF_TINT   = .75;
const float TETRA_CLIP_RADIUS = 1.06;

#define ENABLE_GLITTER 1
const float INC_THICK    = 0.015;
const float GLITTER_FREQ  = 180.;
const float GLITTER_RARE  = 0.67;
const float GLITTER_INT   = 0.75;

#define T (iTime*Speed/6.)
#define col C.rgb
#define res iResolution.xy
#define N(p) ((1./zoOM)*((p)-res*0.5)/min(res.y,res.x))

#define RayMarchSteps 112
const float HIT_EPS  = 1e-5;
const float HIT_STEP = 0.25;
const float HIT_PUSH = 0.09;
const float MARCH_K  = 0.7;

float
maxv2 (vec2 p)
{
  return max(p.x, p.y);
}

float
sphere (vec3 p, float r)
{
  return length(p) - r;
}

mat3
axisAngleMat (vec3 v, float a)
{
  mat3 m;
  vec3 w;
  vec3 b1;
  vec3 b2;
  vec3 bp;
  vec3 bm;
  vec2 cs = sin(a + vec2(0.5 * pi, 0.0));

  w = (1.0 - cs.x) * v * v + cs.x;
  b1 = (1.0 - cs.x) * v.xzy * v.yxz;
  b2 = -cs.y * v.zyx;
  bp = b1 + b2;
  bm = b1 - b2;

  m[0][0] = w.x;
  m[1][1] = w.y;
  m[2][2] = w.z;
  m[0][1] = bp.x;
  m[1][0] = bm.x;
  m[2][0] = bp.y;
  m[0][2] = bm.y;
  m[1][2] = bp.z;
  m[2][1] = bm.z;
  return m;
}

float
tetFrame (vec3 p, float w)
{
  vec3 q = p;
  p = abs(p);
  q = mix(q, q.yzx, step(maxv2(p.yz), p.x));
  q = mix(q, q.zxy, step(maxv2(p.zx), p.y));
  q = mix(q, vec3(q.x, -q.yz).yxz, step(q.z, 0.0)) - vec3(-1.0, 1.0, 1.0) / sqrt(3.0);
  return length(vec2(0.5 * (q.x + q.y), q.z)) - w;
}

float
glitterField (vec3 pb, vec3 rd)
{
  float n = noise3d(pb * ( GLITTER_FREQ*iResolution.y/550. ) + 2.3);
  float b = smoothstep( GLITTER_RARE, 1.0, n);
  float rim = pow(clamp(1.0 - abs(dot(normalize(pb), normalize(rd))), 0.0, 1.0), 3.0);
  return b * (0.25 + 0.75*rim);
}

vec3
toBase (vec3 p, mat2 R)
{
  vec3 b = p;
  b.xy = R*b.xy;
  b.yz = R*b.yz;
  return b;
}

vec2
mapBase (vec3 p, mat2 tetTilt, mat3 tetSpin)
{
  vec3 q = p;
  float dMin = 1e6;
  float iMin = 0.0;

  q.yz = tetTilt * q.yz;
  for (int i = 0; i < 5; ++i)
  {
    float d = tetFrame(q, TETRA_WIRE_RADIUS);
#if ( MERKABA_WIRES == 1 )
          d = min( tetFrame( -q, TETRA_WIRE_RADIUS ), d );
#endif        
    if (d < dMin)
    {
      dMin = d;
      iMin = float(i);
    }
    q = tetSpin * q;
  }

  dMin = max(dMin, sphere(p, TETRA_CLIP_RADIUS));
  return vec2(dMin, iMin);
}

vec3
normal (vec3 p, mat2 R, mat2 tetTilt, mat3 tetSpin)
{
  const float e = 0.00085;
  const vec3 k1 = vec3( 1.0, -1.0, -1.0);
  const vec3 k2 = vec3(-1.0, -1.0,  1.0);
  const vec3 k3 = vec3(-1.0,  1.0, -1.0);
  const vec3 k4 = vec3( 1.0,  1.0,  1.0);

  float d1 = mapBase(toBase(p + e*k1, R), tetTilt, tetSpin).x;
  float d2 = mapBase(toBase(p + e*k2, R), tetTilt, tetSpin).x;
  float d3 = mapBase(toBase(p + e*k3, R), tetTilt, tetSpin).x;
  float d4 = mapBase(toBase(p + e*k4, R), tetTilt, tetSpin).x;

  vec3 n = k1*d1 + k2*d2 + k3*d3 + k4*d4;
  return normalize(n);
}

vec3
getLightDir ()
{
  return normalize(vec3(0.45, 0.85, 0.30));
}

float
fresnelSchlick (float cosTheta, float F0)
{
  float m = clamp(1.0 - cosTheta, 0.0, 1.0);
  float m2 = m*m;
  return 3.*F0 + (1.0 - F0) * (m2*m2*m);
}

vec3
shadeFast (vec3 surfCol, vec3 Nn, vec3 rd, float insideMask)
{
  vec3 L = getLightDir();
  vec3 V = normalize(-rd);
  vec3 Hh = normalize(L + V);

  float NoL = max(dot(Nn, L), 0.0);
  float NoH = max(dot(Nn, Hh), 0.0);
  float NoV = max(dot(Nn, V), 0.0);

  float diff = mix(NoL*6.65 + 0.35, NoL, 1.0 - insideMask);

  float rough = .16;
  float shin  = mix(260.0, 55.0, rough)/1.25;
  float spec  = pow(NoH, shin) * (0.25 + 0.75*NoL);

  float F0 = 100.;
  float F  = fresnelSchlick(NoV, F0);

  float specGain = mix(.55, 1.0, 1.0 - insideMask);

  vec3 specCol = vec3(1.0);

  vec3 colL = surfCol * (0.12 + 1.10*diff) + specCol * (spec * (0.35 + 1.65*F) * specGain);
  return colL*1.5;
}

void
main ()
{
  vec4 C = vec4 (0.0, 0.0, 0.0, 1.0);
  vec2 POS = gl_FragCoord.xy;

  vec2 uv = N(POS);
  vec3 ro = vec3(0.0, 0.0, -2.0);
  vec3 rd = vec3(uv, 1.2);

  // PRECOMPUTE base rotation once
  mat2 R = rot(T);
  float tetraTiltA = atan(2.0 / (sqrt(5.0) + 1.0));
  vec2 tetraAxisYZ = sin(tetraTiltA + vec2(0.5 * pi, 0.0));
  mat2 tetTilt = mat2(tetraAxisYZ.x, tetraAxisYZ.y, -tetraAxisYZ.y, tetraAxisYZ.x);
  mat3 tetSpin = axisAngleMat(vec3(0.0, tetraAxisYZ.x, tetraAxisYZ.y), 2.0 * pi / 5.0);
  int paletteIdx = (iMouse.z > 0.0) ? PALETTE_CLICK : PALETTE_IDLE;
  float dither = (hash12(POS.xy) - 0.5) / 128.0;
  vec3 blueGlow = vec3(0.0135, 0.388, 0.79)/2.;
  vec3 pal0 = paletteSelect(0.5/6.0, paletteIdx);
  vec3 pal1 = paletteSelect(1.5/6.0, paletteIdx);
  vec3 pal2 = paletteSelect(2.5/6.0, paletteIdx);
  vec3 pal3 = paletteSelect(3.5/6.0, paletteIdx);
  vec3 pal4 = paletteSelect(5.0/6.0, paletteIdx);
  vec3 glow0 = mix(blueGlow, blueGlow * (0.35 + 0.85*pal0), GLOW_COLORK);
  vec3 glow1 = mix(blueGlow, blueGlow * (0.35 + 0.85*pal1), GLOW_COLORK);
  vec3 glow2 = mix(blueGlow, blueGlow * (0.35 + 0.85*pal2), GLOW_COLORK);
  vec3 glow3 = mix(blueGlow, blueGlow * (0.35 + 0.85*pal3), GLOW_COLORK);
  vec3 glow4 = mix(blueGlow, blueGlow * (0.35 + 0.85*pal4), GLOW_COLORK);

  float t = 0.0;
  bool inside = false;
  float curIOR = IOR_AIR;

  for (int i=0;i<RayMarchSteps;i++)
  {
    if(t > 2.67)
      break;

    vec3 p = ro + rd*t;

    vec3 baseP = toBase(p, R);
    vec2 dm = mapBase(baseP, tetTilt, tetSpin);

    float d   = dm.x;
    float idx = dm.y;

    vec3 surfCol = pickTetraColor(idx, pal0, pal1, pal2, pal3, pal4);

    float ad = abs(d);
    float glow = smoothstep(0.00015, 0.05, ad);
    float dd = max(ad, 0.005);

    vec3 glowCol  = pickTetraColor(idx, glow0, glow1, glow2, glow3, glow4);
    col += GLOW_BASE/(dd*dd) * surfCol*glowCol * glow;

    if ( inside )
    {
      col += surfCol/50.0;
    }

    if (!inside && RAYS )
    {
      col += ( surfCol / 42.0 * pow(length(p), .4) + dither )/1.67;

      if ( RAYSsmoootha == true )
      {
        if( (i & 3) == 0 )
        {
          col = ( col + tonemap2( col ) )/1.75;
        }
      }
    }

#if ENABLE_GLITTER
    float matMask = smoothstep(INC_THICK, 0.0, ad);
    if(matMask > 0.001)
    {
      float bub = glitterField(baseP, rd);
      col += GLITTER_INT * bub * matMask * (0.35 + 0.85*surfCol);
    }
#endif

    if(ad < HIT_EPS)
    {
      vec3 Nn = normal(p, R, tetTilt, tetSpin);
      if(dot(rd,Nn) > 0.0) Nn = -Nn;

      float insideMask = inside ? 1.0 : 0.0;
      vec3 lit = shadeFast(surfCol, Nn, rd, insideMask);

      col += lit * SURF_TINT;

      bool entering = !inside;
      float etaI = curIOR;
      float etaT = entering ? refractionIndex : IOR_AIR;

      vec3 refr = refract(rd, Nn, etaI/etaT);

      if(length(refr) < 1e-6)
      {
        rd = reflect(rd, Nn);
      }
      else
      {
        rd = refr;
        inside = entering;
        curIOR = inside ? refractionIndex : IOR_AIR;
      }

      t += HIT_STEP + HIT_PUSH;
      continue;
    }

    t += ad * MARCH_K;
  }

  col = tonemap(col);
  C.a = 1.0;

  gl_FragColor = C;
}
