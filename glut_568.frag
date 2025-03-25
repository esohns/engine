uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define SEAMLESS_LOOP
//#define COLOUR_CYCLE

#define PI 3.14159265359
#define PHI (1.618033988749895)

float t;

#define saturate(x) clamp(x, 0., 1.)

mat3
rotationMatrix (vec3 axis, float angle)
{
  axis = normalize(axis);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;

  return mat3(
      oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
      oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
      oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c
  );
}

mat3
orientMatrix (vec3 A, vec3 B)
{
  mat3 Fi = mat3(
      A,
      (B - dot(A, B) * A) / length(B - dot(A, B) * A),
      cross(B, A)
  );
  mat3 G = mat3(
      dot(A, B),              -length(cross(A, B)),   0,
      length(cross(A, B)),    dot(A, B),              0,
      0,                      0,                      1
  );
  return Fi * G * inverse(Fi);
}

#define GDFVector3 normalize(vec3(1, 1, 1 ))
#define GDFVector3b normalize(vec3(-1, -1, -1 ))
#define GDFVector4 normalize(vec3(-1, 1, 1))
#define GDFVector4b normalize(vec3(-1, -1, 1))
#define GDFVector5 normalize(vec3(1, -1, 1))
#define GDFVector5b normalize(vec3(1, -1, -1))
#define GDFVector6 normalize(vec3(1, 1, -1))
#define GDFVector6b normalize(vec3(-1, 1, -1))

#define GDFVector7 normalize(vec3(0, 1, PHI+1.))
#define GDFVector7b normalize(vec3(0, 1, -PHI-1.))
#define GDFVector8 normalize(vec3(0, -1, PHI+1.))
#define GDFVector8b normalize(vec3(0, -1, -PHI-1.))
#define GDFVector9 normalize(vec3(PHI+1., 0, 1))
#define GDFVector9b normalize(vec3(PHI+1., 0, -1))
#define GDFVector10 normalize(vec3(-PHI-1., 0, 1))
#define GDFVector10b normalize(vec3(-PHI-1., 0, -1))
#define GDFVector11 normalize(vec3(1, PHI+1., 0))
#define GDFVector11b normalize(vec3(1, -PHI-1., 0))
#define GDFVector12 normalize(vec3(-1, PHI+1., 0))
#define GDFVector12b normalize(vec3(-1, -PHI-1., 0))

#define GDFVector13 normalize(vec3(0, PHI, 1))
#define GDFVector13b normalize(vec3(0, PHI, -1))
#define GDFVector14 normalize(vec3(0, -PHI, 1))
#define GDFVector14b normalize(vec3(0, -PHI, -1))
#define GDFVector15 normalize(vec3(1, 0, PHI))
#define GDFVector15b normalize(vec3(1, 0, -PHI))
#define GDFVector16 normalize(vec3(-1, 0, PHI))
#define GDFVector16b normalize(vec3(-1, 0, -PHI))
#define GDFVector17 normalize(vec3(PHI, 1, 0))
#define GDFVector17b normalize(vec3(PHI, -1, 0))
#define GDFVector18 normalize(vec3(-PHI, 1, 0))
#define GDFVector18b normalize(vec3(-PHI, -1, 0))

#define fGDFBegin float d = 0.;
#define fGDFExp(v) d += pow(abs(dot(p, v)), e);
#define fGDF(v) d = max(d, abs(dot(p, v)));
#define fGDFExpEnd return pow(d, 1./e) - r;
#define fGDFEnd return d - r;

float
fDodecahedron (vec3 p, float r)
{
  fGDFBegin
  fGDF(GDFVector13) fGDF(GDFVector14) fGDF(GDFVector15) fGDF(GDFVector16)
  fGDF(GDFVector17) fGDF(GDFVector18)
  fGDFEnd
}

float
fIcosahedron (vec3 p, float r)
{
  fGDFBegin
  fGDF(GDFVector3) fGDF(GDFVector4) fGDF(GDFVector5) fGDF(GDFVector6)
  fGDF(GDFVector7) fGDF(GDFVector8) fGDF(GDFVector9) fGDF(GDFVector10)
  fGDF(GDFVector11) fGDF(GDFVector12)
  fGDFEnd
}

float
vmax (vec3 v)
{
  return max(max(v.x, v.y), v.z);
}

float
sgn (float x)
{
  return (x<0.)?-1.:1.;
}

float
fPlane (vec3 p, vec3 n, float distanceFromOrigin)
{
  return dot(p, n) + distanceFromOrigin;
}

float
fBox (vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return length(max(d, vec3(0))) + vmax(min(d, vec3(0)));
}

float
fLineSegment (vec3 p, vec3 a, vec3 b)
{
  vec3 ab = b - a;
  float t = saturate(dot(p - a, ab) / dot(ab, ab));
  return length((ab*t + a) - p);
}

float
fCapsule (vec3 p, vec3 a, vec3 b, float r)
{
  return fLineSegment(p, a, b) - r;
}

void
pR (inout vec2 p, float a)
{
  p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

float
pReflect (inout vec3 p, vec3 planeNormal, float offset)
{
  float t = dot(p, planeNormal)+offset;
  if (t < 0.)
  {
    p = p - (2.*t)*planeNormal;
  }
  return sign(t);
}

float
pModPolar (inout vec2 p, float repetitions)
{
  float angle = 2.*PI/repetitions;
  float a = atan(p.y, p.x) + angle/2.;
  float r = length(p);
  float c = floor(a/angle);
  a = mod(a,angle) - angle/2.;
  p = vec2(cos(a), sin(a))*r;
  if (abs(c) >= (repetitions/2.))
    c = abs(c);
  return c;
}

void
pModPolar (inout vec3 p, vec3 axis, float repetitions, float offset)
{
  vec3 z = vec3(0,0,1);
  mat3 m = orientMatrix(axis, z);
  p *= inverse(m);
  pR(p.xy, offset);
  pModPolar(p.xy, repetitions);
  pR(p.xy, -offset);
  p *= m;
}

int Type=5;
vec3 nc;
vec3 pbc;
vec3 pca;
void
initIcosahedron ()
{
  float cospin=cos(PI/float(Type)), scospin=sqrt(0.75-cospin*cospin);
  nc=vec3(-0.5,-cospin,scospin);
  pbc=vec3(scospin,0.,0.5);
  pca=vec3(0.,scospin,cospin);
  pbc=normalize(pbc);	pca=normalize(pca);
}

void
pModIcosahedron (inout vec3 p)
{
  p = abs(p);
  pReflect(p, nc, 0.);
  p.xy = abs(p.xy);
  pReflect(p, nc, 0.);
  p.xy = abs(p.xy);
  pReflect(p, nc, 0.);
}

float
indexSgn (float s)
{
  return s / 2. + 0.5;
}

bool
boolSgn (float s)
{
  return bool(s / 2. + 0.5);
}

float
pModIcosahedronIndexed (inout vec3 p, int subdivisions)
{
  float x = indexSgn(sgn(p.x));
  float y = indexSgn(sgn(p.y));
  float z = indexSgn(sgn(p.z));
  p = abs(p);
  pReflect(p, nc, 0.);

  float xai = sgn(p.x);
  float yai = sgn(p.y);
  p.xy = abs(p.xy);
  float sideBB = pReflect(p, nc, 0.);

  float ybi = sgn(p.y);
  float xbi = sgn(p.x);
  p.xy = abs(p.xy);
  pReflect(p, nc, 0.);

  float idx = 0.;

  float faceGroupAi = indexSgn(ybi * yai * -1.);
  float faceGroupBi = indexSgn(yai);
  float faceGroupCi = clamp((xai - ybi -1.), 0., 1.);
  float faceGroupDi = clamp(1. - faceGroupAi - faceGroupBi - faceGroupCi, 0., 1.);

  idx += faceGroupAi * (x + (2. * y) + (4. * z));
  idx += faceGroupBi * (8. + y + (2. * z));
#ifndef SEAMLESS_LOOP
  idx += faceGroupCi * (12. + x + (2. * z));
#endif
  idx += faceGroupDi * (12. + x + (2. * y));

  return idx;
}

vec3
pal (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b*cos( 6.28318*(c*t+d) );
}

vec3
spectrum (float n)
{
  return pal( n, vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,1.0,1.0),vec3(0.0,0.33,0.67) );
}

vec3
vMin (vec3 p, vec3 a, vec3 b, vec3 c)
{
  float la = length(p - a);
  float lb = length(p - b);
  float lc = length(p - c);
  if (la < lb)
  {
    if (la < lc)
      return a;
    return c;
  }
  else
  {
    if (lb < lc)
      return b;

    return c;
  }
}

vec3
icosahedronVertex (vec3 p)
{
  if (p.z > 0.)
  {
    if (p.x > 0.)
    {
      if (p.y > 0.)
      {
        return vMin(p, GDFVector13, GDFVector15, GDFVector17);
      }
      else
      {
        return vMin(p, GDFVector14, GDFVector15, GDFVector17b);
      }
    }
    else
    {
      if (p.y > 0.)
      {
        return vMin(p, GDFVector13, GDFVector16, GDFVector18);
      }
      else
      {
        return vMin(p, GDFVector14, GDFVector16, GDFVector18b);
      }
    }
  }
  else
  {
    if (p.x > 0.)
    {
      if (p.y > 0.)
      {
        return vMin(p, GDFVector13b, GDFVector15b, GDFVector17);
      }
      else
      {
        return vMin(p, GDFVector14b, GDFVector15b, GDFVector17b);
      }
    }
    else
    {
      if (p.y > 0.)
      {
        return vMin(p, GDFVector13b, GDFVector16b, GDFVector18);
      }
      else
      {
        return vMin(p, GDFVector14b, GDFVector16b, GDFVector18b);
      }
    }
  }
}

vec4
icosahedronAxisDistance (vec3 p)
{
  vec3 iv = icosahedronVertex(p);
  vec3 originalIv = iv;

  vec3 pn = normalize(p);
  pModIcosahedron(pn);
  pModIcosahedron(iv);

  float boundryDist = dot(pn, vec3(1, 0, 0));
  float boundryMax = dot(iv, vec3(1, 0, 0));
  boundryDist /= boundryMax;

  float roundDist = length(iv - pn);
  float roundMax = length(iv - vec3(0, 0, 1.));
  roundDist /= roundMax;
  roundDist = -roundDist + 1.;

  float blend = 1. - boundryDist;
  blend = pow(blend, 6.);

  float dist = mix(roundDist, boundryDist, blend);

  return vec4(originalIv, dist);
}

void
pTwistIcosahedron (inout vec3 p, float amount)
{
  vec4 a = icosahedronAxisDistance(p);
  vec3 axis = a.xyz;
  float dist = a.a;
  mat3 m = rotationMatrix(axis, dist * amount);
  p *= m;
}

struct Model
{
  float dist;
  vec3 colour;
  float id;
};

Model
fInflatedIcosahedron (vec3 p, vec3 axis)
{
  float d = 1000.;

#ifdef SEAMLESS_LOOP
  pModPolar(p, axis, 3., PI/2.);
#endif

  float idx = pModIcosahedronIndexed(p, 0);
  d = min(d, dot(p, pca) - .9);
  d = mix(d, length(p) - .9, .5);

#ifdef SEAMLESS_LOOP
  if (idx == 3.)
  {
    idx = 2.;
  }
  idx /= 10.;
#else
  idx /= 20.;
#endif
#ifdef COLOUR_CYCLE
  idx = mod(idx + t*1.75, 1.);
#endif
  vec3 colour = spectrum(idx);

  d *= .6;
  return Model(d, colour, 1.);
}

void
pTwistIcosahedron (inout vec3 p, vec3 center, float amount)
{
  p += center;
  pTwistIcosahedron(p, 5.5);
  p -= center;
}

Model
model (vec3 p)
{
  float rate = PI/6.;
  vec3 axis = pca;

  vec3 twistCenter = vec3(0);
  twistCenter.x = cos(t * rate * -3.) * .3;
  twistCenter.y = sin(t * rate * -3.) * .3;

  mat3 m = rotationMatrix(
      reflect(axis, vec3(0,1,0)),
      t * -rate
  );
  p *= m;
  twistCenter *= m;

  pTwistIcosahedron(p, twistCenter, 5.5);

  return fInflatedIcosahedron(p, axis);
}

const float MAX_TRACE_DISTANCE = 30.0;
const float INTERSECTION_PRECISION = 0.001;
const int NUM_OF_TRACE_STEPS = 100;

vec2
opU (vec2 d1, vec2 d2)
{
  return (d1.x<d2.x) ? d1 : d2;
}

Model
map (vec3 p)
{
  return model(p);
}

float
softshadow (vec3 ro, vec3 rd, float mint, float tmax)
{
  float res = 1.0;
  float t = mint;
  for( int i=0; i<16; i++ )
  {
    float h = map( ro + rd*t ).dist;
    res = min( res, 8.0*h/t );
    t += clamp( h, 0.02, 0.10 );
    if( h<0.001 || t>tmax )
      break;
  }
  return clamp( res, 0.0, 1.0 );
}

float
calcAO (vec3 pos, vec3 nor)
{
  float occ = 0.0;
  float sca = 1.0;
  for( int i=0; i<5; i++ )
  {
    float hr = 0.01 + 0.12*float(i)/4.0;
    vec3 aopos =  nor * hr + pos;
    float dd = map( aopos ).dist;
    occ += -(dd-hr)*sca;
    sca *= 0.95;
  }
  return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );    
}

vec3
gamma (vec3 color, float g)
{
  return pow(color, vec3(g));
}

const float GAMMA = 2.2;
vec3
linearToScreen (vec3 linearRGB)
{
  return gamma(linearRGB, 1.0 / GAMMA);
}

vec3
doLighting (vec3 col, vec3 pos, vec3 nor, vec3 ref, vec3 rd)
{
  float occ = calcAO( pos, nor );
  vec3  lig = normalize( vec3(-0.6, 0.7, 0.5) );
  float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );
  float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
  float bac = clamp( dot( nor, normalize(vec3(-lig.x,0.0,-lig.z))), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);
  //float dom = smoothstep( -0.1, 0.1, ref.y );
  float fre = pow( clamp(1.0+dot(nor,rd),0.0,1.0), 2.0 );

  dif *= softshadow( pos, lig, 0.02, 2.5 );

  vec3 lin = vec3(0.0);
  lin += 1.20*dif*vec3(.95,0.80,0.60);
  lin += 0.80*amb*vec3(0.50,0.70,.80)*occ;
  lin += 0.30*bac*vec3(0.25,0.25,0.25)*occ;
  lin += 0.20*fre*vec3(1.00,1.00,1.00)*occ;
  col = col*lin;

  return col;
}

struct Hit
{
  float len;
  vec3 colour;
  float id;
};

Hit
calcIntersection (vec3 ro, vec3 rd)
{
  float h =  INTERSECTION_PRECISION*2.0;
  float t = 0.0;
  float res = -1.0;
  float id = -1.;
  vec3 colour;

  for( int i=0; i< NUM_OF_TRACE_STEPS ; i++ )
  {
    if( abs(h) < INTERSECTION_PRECISION || t > MAX_TRACE_DISTANCE )
      break;
    Model m = map( ro+rd*t );
    h = m.dist;
    t += h;
    id = m.id;
    colour = m.colour;
  }

  if( t < MAX_TRACE_DISTANCE ) res = t;
  if( t > MAX_TRACE_DISTANCE ) id =-1.0;

  return Hit( res , colour , id );
}

mat3
calcLookAtMatrix (vec3 ro, vec3 ta, float roll)
{
  vec3 ww = normalize( ta - ro );
  vec3 uu = normalize( cross(ww,vec3(sin(roll),cos(roll),0.0) ) );
  vec3 vv = normalize( cross(uu,ww));
  return mat3( uu, vv, ww );
}

void
doCamera (out vec3 camPos, out vec3 camTar, out float camRoll, float time, vec2 mouse)
{
  float x = mouse.x;
  float y = mouse.y;

  x = .65;
  y = .44;

  float dist = 3.3;
  float height = 0.;
  camPos = vec3(0,0,-dist);
  vec3 axisY = vec3(0,1,0);
  vec3 axisX = vec3(1,0,0);
  mat3 m = rotationMatrix(axisY, -x * PI * 2.);
  axisX *= m;
  camPos *= m;
  m = rotationMatrix(axisX, -(y -.5) * PI*2.);
  camPos *= m;
  camPos.y += height;
  camTar = -camPos + vec3(.0001);
  camTar.y += height;
  camRoll = 0.;
}

vec3
calcNormal (vec3 pos)
{
  vec3 eps = vec3( 0.001, 0.0, 0.0 );
  vec3 nor = vec3(
      map(pos+eps.xyy).dist - map(pos-eps.xyy).dist,
      map(pos+eps.yxy).dist - map(pos-eps.yxy).dist,
      map(pos+eps.yyx).dist - map(pos-eps.yyx).dist );
  return normalize(nor);
}

vec2 ffragCoord;
vec3
render (Hit hit, vec3 ro, vec3 rd)
{
  vec3 pos = ro + rd * hit.len;

  vec3 color = vec3(.04,.045,.05);
  color = vec3(.35, .5, .65);
  vec3 colorB = vec3(.8, .8, .9);

  vec2 pp = (-iResolution.xy + 2.0*ffragCoord.xy)/iResolution.y;

  color = mix(colorB, color, length(pp)/1.5);

  if (hit.id == 1.)
  {
    vec3 norm = calcNormal( pos );
    vec3 ref = reflect(rd, norm);
    color = doLighting(hit.colour, pos, norm, ref, rd);
  }

  return color;
}


void
main ()
{
  initIcosahedron();
  t = iTime - .25;
  //t = mod(t, 4.);

  ffragCoord = gl_FragCoord.xy;

  vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
  vec2 m = iMouse.xy / iResolution.xy;

  vec3 camPos = vec3( 0., 0., 2.);
  vec3 camTar = vec3( 0. , 0. , 0. );
  float camRoll = 0.;

  doCamera(camPos, camTar, camRoll, iTime, m);

  mat3 camMat = calcLookAtMatrix (camPos, camTar, camRoll);

  vec3 rd = normalize( camMat * vec3(p.xy,2.0) );

  Hit hit = calcIntersection( camPos , rd  );

  vec3 color = render( hit , camPos , rd );
  color = linearToScreen(color);

  gl_FragColor = vec4(color,1.0);
}
