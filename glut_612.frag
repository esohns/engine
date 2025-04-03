uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

//#define DEBUG
#define PI 3.14159265359
#define PHI (1.618033988749895)

float t;

#define saturate(x) clamp(x, 0., 1.)

#define GDFVector0 vec3(1, 0, 0)
#define GDFVector1 vec3(0, 1, 0)
#define GDFVector2 vec3(0, 0, 1)

#define GDFVector3 normalize(vec3(1, 1, 1 ))
#define GDFVector4 normalize(vec3(-1, 1, 1))
#define GDFVector5 normalize(vec3(1, -1, 1))
#define GDFVector6 normalize(vec3(1, 1, -1))

#define GDFVector7 normalize(vec3(0, 1, PHI+1.))
#define GDFVector8 normalize(vec3(0, -1, PHI+1.))
#define GDFVector9 normalize(vec3(PHI+1., 0, 1))
#define GDFVector10 normalize(vec3(-PHI-1., 0, 1))
#define GDFVector11 normalize(vec3(1, PHI+1., 0))
#define GDFVector12 normalize(vec3(-1, PHI+1., 0))

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

float vmax (vec3 v) { return max(max(v.x, v.y), v.z); }

float
fPlane (vec3 p, vec3 n, float distanceFromOrigin)
{
  return dot(p, n) + distanceFromOrigin;
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
    p = p - (2.*t)*planeNormal;
  return sign(t);
}

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

int Type=5;
vec3 nc;
void
initIcosahedron ()
{
  float  cospin=cos(PI/float(Type)),
        scospin=sqrt(0.75-cospin*cospin);
  nc=vec3(-0.5,-cospin,scospin);
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

  if (lb < lc)
    return b;
  return c;
}

vec3
icosahedronVertex (vec3 p)
{
  if (p.z > 0.)
  {
    if (p.x > 0.)
    {
      if (p.y > 0.)
        return vMin(p, GDFVector13, GDFVector15, GDFVector17);
      return vMin(p, GDFVector14, GDFVector15, GDFVector17b);
    }

    if (p.y > 0.)
      return vMin(p, GDFVector13, GDFVector16, GDFVector18);
    return vMin(p, GDFVector14, GDFVector16, GDFVector18b);
  }

  if (p.x > 0.)
  {
    if (p.y > 0.)
      return vMin(p, GDFVector13b, GDFVector15b, GDFVector17);
    return vMin(p, GDFVector14b, GDFVector15b, GDFVector17b);
  }

  if (p.y > 0.)
    return vMin(p, GDFVector13b, GDFVector16b, GDFVector18);
  return vMin(p, GDFVector14b, GDFVector16b, GDFVector18b);
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

float
model (vec3 p)
{
#ifndef DEBUG
  float wobble = sin(PI/2. * t);
  float wobbleX2 = sin(PI/2. * t*2.);
  pR(p.xy, wobbleX2 * .05);
  pR(p.xz, wobbleX2 * .05);
  float a = -wobble * 3.;
  pTwistIcosahedron(p, a);
#endif
  return fIcosahedron(p, 1.);
}

vec3
debugSpectrum (float n)
{
  vec3 c = spectrum(n);
  c *= 1. + min(sign(n), .0) * .3;
  c *= 1. + max(sign(n - 1.), 0.);
  return c;
}

vec3
material (vec3 p, vec3 norm, vec3 ref)
{
#ifdef DEBUG
  vec4 a = icosahedronAxisDistance(p);
  float dist = a.a;
  return debugSpectrum(dist);
#else
  return norm * 0.5 + 0.5;
#endif
}

const float MAX_TRACE_DISTANCE = 30.0;
const float INTERSECTION_PRECISION = 0.001;
const int NUM_OF_TRACE_STEPS = 100;

vec2
opU (vec2 d1, vec2 d2)
{
  return (d1.x<d2.x) ? d1 : d2;
}

vec2
map (vec3 p)
{
  return vec2(model(p), 1.);
}

vec2
calcIntersection (vec3 ro, vec3 rd)
{
  float h =  INTERSECTION_PRECISION*2.0;
  float t = 0.0;
  float res = -1.0;
  float id = -1.;

  for( int i=0; i< NUM_OF_TRACE_STEPS ; i++ )
  {
    if( h < INTERSECTION_PRECISION || t > MAX_TRACE_DISTANCE )
      break;
    vec2 m = map( ro+rd*t );
    h = m.x;
    t += h;
    id = m.y;
  }

  if( t < MAX_TRACE_DISTANCE ) res = t;
  if( t > MAX_TRACE_DISTANCE ) id = -1.0;

  return vec2( res , id );
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
    
  x += .68;
  y += .44;
    
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
    map(pos+eps.xyy).x - map(pos-eps.xyy).x,
    map(pos+eps.yxy).x - map(pos-eps.yxy).x,
    map(pos+eps.yyx).x - map(pos-eps.yyx).x );
  return normalize(nor);
}

vec3
render (vec2 res, vec3 ro, vec3 rd)
{
  vec3 color = vec3(.04,.045,.05);
  color = vec3(.7, .8, .8);
  vec3 pos = ro + rd * res.x;

  if (res.y == 1.)
  {
    vec3 norm = calcNormal( pos );
    vec3 ref = reflect(rd, norm);
    color = material(pos, norm, ref);
  }

  return color;
}


void
main ()
{
  initIcosahedron();
  t = iTime - .25;
  t = mod(t, 4.);

  vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
  vec2 m = iMouse.xy / iResolution.xy;

  vec3 camPos = vec3( 0., 0., 2.);
  vec3 camTar = vec3( 0. , 0. , 0. );
  float camRoll = 0.;

  doCamera(camPos, camTar, camRoll, iTime, m);

  mat3 camMat = calcLookAtMatrix( camPos, camTar, camRoll );

  vec3 rd = normalize( camMat * vec3(p.xy,2.0) );

  vec2 res = calcIntersection( camPos , rd  );

  vec3 color = render( res , camPos , rd );

  gl_FragColor = vec4(color,1.0);
}
