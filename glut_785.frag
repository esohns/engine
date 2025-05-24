#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define DARK_MODE

const float MAX_DISPERSE = 5.;
const float MAX_BOUNCE = 10.;

mat4 cameraMatrix = mat4(
    0.9780874848365784,
    -0.07870610803365707,
    -0.19274398684501648,
    0,
    0.20812205970287323,
    0.3452317416667938,
    0.9151507019996643,
    0,
    -0.005486522801220417,
    -0.935211718082428,
    0.35404732823371887,
    0,
    -4.901630745735019e-7,
    0.00000874467059475137,
    -9.502106666564941,
    1
);

#define PI 3.14159265359
#define TAU 6.28318530718

#define saturate(x) clamp(x, 0., 1.)

void
pR (inout vec2 p, float a)
{
  p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

float
smax (float a, float b, float r)
{
  vec2 u = max(vec2(r + a,r + b), vec2(0));
  return min(-r, max (a, b)) + length(u);
}

float
vmax (vec2 v)
{
  return max(v.x, v.y);
}

float
fBox (vec2 p, vec2 b)
{
  vec2 d = abs(p) - b;
  return length(max(d, vec2(0))) + vmax(min(d, vec2(0)));
}

float
range (float vmin, float vmax, float value)
{
  return clamp((value - vmin) / (vmax - vmin), 0., 1.);
}

mat4
rotX (float a)
{
  return mat4(1,0,0,0, 0,cos(a),-sin(a),0, 0,sin(a),cos(a),0, 0,0,0,1);
}

mat4
rotZ (float a)
{
  return mat4(cos(a),-sin(a),0,0, sin(a),cos(a),0,0, 0,0,1,0, 0,0,0,1);
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

int Type=3;
vec3 nc,pab,pbc,pca;
void
initPoly ()
{
  float cospin=cos(PI/float(Type)), scospin=sqrt(0.75-cospin*cospin);
  nc=vec3(-0.5,-cospin,scospin);
  pab=vec3(0.,0.,1.);
  pbc=vec3(scospin,0.,0.5);
  pca=vec3(0.,scospin,cospin);
  pbc=normalize(pbc);	pca=normalize(pca);
}

vec3
fold (vec3 pos)
{
  for(int i=0;i<3 /*Type*/;i++)
  {
    pos.xy=abs(pos.xy);
    pos-=2.*min(0.,dot(pos,nc))*nc;
  }

  return pos;
}

float time;

float
tweenOffset (float t, float start, float duration)
{
  t = range(start, start + duration, t);
  t = pow(t, 2.);
  return t;
}

float
tweenBlend (float t, float start, float duration)
{
  t = range(start, start + duration, t);
  t = pow(t, .5);
  t = smoothstep(0., 1., t);
  return t;
}

const float STEP_SCALE = 1./3.;
float
tetAnim (vec3 p, float time)
{
  p = fold(p);

  float sz = .3;
  float rBase = .04;
  float rInner = rBase * STEP_SCALE;
  float blendDuration = .75;
  float offsetDuration = .75;
  float t = time * (blendDuration + offsetDuration);
  offsetDuration *= 2.;
  float offsetDistance = .6;

  float blend = tweenBlend(t, .0, blendDuration);
  float offsetT = tweenOffset(t, blendDuration, offsetDuration);
  float offset = offsetT * offsetDistance;

  if (t < 0. || offsetT >= 1.)
    return 1e12;

  vec3 n1 = pca;
  vec3 n2 = normalize(pca * vec3(-1,-1,1));
  vec3 n3 = normalize(pbc * vec3(1,-1,-1));
  vec3 n4 = normalize(pbc * vec3(-1,-1,-1));

  float sep = .001 * (1. - offsetT);
  float scale = 1. - offsetT;

  float bound = (dot((p + (n4 + n3) * offset) / scale, n1) - sz) * scale;
  if (bound > .004)
    return bound;

  vec3 pp = p;

  p = pp + n4 * offset;
  p /= scale;
  float oct = dot(p, n1) - sz;
  oct = smax(oct, (dot(p, n2) - sz), rBase);
  oct = smax(oct, -(dot(p, n4) + .5 - sep), rInner);
  oct = smax(oct, -(dot(p, n3) + .1 - sep), rInner);
  oct = smax(oct, (dot(p, n4) + .1 + sep), rInner);
  oct *= scale;

  p = pp + (n4 + n3) * offset;
  p /= scale;
  float edge = dot(p, n1) - sz;
  edge = smax(edge, (dot(p, n2) - sz), rBase);    

  p = pp + (n4 + n3) * offset;
  p /= scale;
  float side = edge;
  side = smax(side, (dot(p, n3) + .1 + sep), rInner);
  side = smax(side, (dot(p, n4) + .1 + sep), rInner);
  side *= scale;

  p = pp + n4 * (offset + offset);
  p /= scale;
  float vert = edge; vert = smax(vert, (dot(p, n3) - sz), rBase);
  vert = smax(vert, (dot(p, n4) + .5 + sep), rInner);
  vert *= scale;

  float sliced = 1e12;
  sliced = min(sliced, oct);
  sliced = min(sliced, vert);
  sliced = min(sliced, side);

  if (time < 1.)
  {
    p = pp;
    float inner = -(dot(p, n4) + .1 - sep);
    inner = smax(inner, -(dot(p, n3) + .1 - sep), rInner);
    inner = smax(inner, -(dot(p, n2) + .1 - sep), rInner);
    sliced = min(sliced, inner);
  }

  if (blend >= 1.)
    return sliced;

  float base = dot(p, n1) - sz;
  base = smax(base, (dot(p, n2) - sz), rBase);
  base = smax(base, (dot(p, n3) - sz), rBase);

  float surface = 1. - saturate(-base / sz);

  float surfaceBlend = saturate(blend * .66 * range(.9, 1., surface));
  base = mix(base, sliced, surfaceBlend);

  float slicedS = min(sliced, -base - (.3 - .3 * blend));
  float d = max(base, slicedS);
  d = mix(d, sliced, smoothstep(.9, 1., blend));

  return d;
}

float
tetLoop (vec3 p)
{
  pR(p.xy, PI/2. * -time + PI/2.);

  float t = time;
  float scale = pow(STEP_SCALE, t);
  float d = tetAnim(p * scale, time) / scale;

  scale *= STEP_SCALE;
  pR(p.xy, PI/2. * -1.);
  d = min(d, tetAnim(p * scale, time + 1.) / scale);

  return d;
}

vec2
map (vec3 p)
{
  float d = tetLoop(p);
  return vec2(d, 1.);
}

float
intersectPlane (vec3 rOrigin, vec3 rayDir, vec3 origin, vec3 normal, vec3 up, out vec2 uv)
{
  float d = dot(normal, (origin - rOrigin)) / dot(rayDir, normal);
  vec3 point = rOrigin + d * rayDir;
  vec3 tangent = cross(normal, up);
  vec3 bitangent = cross(normal, tangent);
  point -= origin;
  uv = vec2(dot(tangent, point), dot(bitangent, point));
  return max(sign(d), 0.);
}

mat3 envOrientation;

vec3
light (vec3 origin, vec3 rayDir)
{
  origin = -(cameraMatrix * vec4(origin, 1)).xyz;
  rayDir = -(cameraMatrix * vec4(rayDir, 0)).xyz;

  origin *= envOrientation;
  rayDir *= envOrientation;

  vec2 uv;
  float hit = intersectPlane(origin, rayDir, vec3(5,-2,-8), normalize(vec3(1,-.5,-.1)), normalize(vec3(0,1,0)), uv);
  float l = smoothstep(.75, .0, fBox(uv, vec2(.4,1.2) * 6.));
  return vec3(l) * hit;
}

vec3
env (vec3 origin, vec3 rayDir)
{
  origin = -(cameraMatrix * vec4(origin, 1)).xyz;
  rayDir = -(cameraMatrix * vec4(rayDir, 0)).xyz;

  origin *= envOrientation;
  rayDir *= envOrientation;

  float l = smoothstep(.0, 1.7, dot(rayDir, vec3(.5,-.3,1))) * .4;
  return vec3(l) * vec3(1,1,1);
}

#define ZERO (min(iFrame,0))
vec3
normal (vec3 pos)
{
  vec3 n = vec3(0.0);
  for( int i=ZERO; i<4; i++ )
  {
    vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
    n += e*map(pos+0.001*e).x;
  }
  return normalize(n);
}

struct Hit
{
  vec2 res;
  vec3 p;
  float len;
  float steps;
};

Hit
march (vec3 origin, vec3 rayDir, float invert, float maxDist, float understep)
{
  vec3 p;
  float len = 0.;
  float dist = 0.;
  vec2 res = vec2(0.);
  vec2 candidate = vec2(0.);
  float steps = 0.;

  for (float i = 0.; i < 100.; i++)
  {
    len += dist * understep;
    p = origin + len * rayDir;
    candidate = map(p);
    dist = candidate.x * invert;
    steps += 1.;
    if (dist < .001)
    {
      res = candidate;
      break;
    }
    if (len >= maxDist)
    {
      len = maxDist;
      break;
    }
  }   

  return Hit(res, p, len, steps);
}

mat3
sphericalMatrix (vec2 tp)
{
  float theta = tp.x;
  float phi = tp.y;
  float cx = cos(theta);
  float cy = cos(phi);
  float sx = sin(theta);
  float sy = sin(phi);
  return mat3(
      cy, -sy * -sx, -sy * cx,
      0, cx, sx,
      sy, cy * -sx, cy * cx
  );
}

mat3
calcLookAtMatrix (vec3 ro, vec3 ta, vec3 up)
{
  vec3 ww = normalize(ta - ro);
  vec3 uu = normalize(cross(ww,up));
  vec3 vv = normalize(cross(uu,ww));
  return mat3(uu, vv, ww);
}

void
main ()
{
  initPoly();

  time = iTime / 2.;
  //time = 0.5;
  //time = 0.1;
  //time = 1.;
  time = fract(time + .4);
    
#ifndef DARK_MODE
  envOrientation = sphericalMatrix(((vec2(81.5, 119) / vec2(187)) * 2. - 1.) * 2.);
#else
  envOrientation = sphericalMatrix((vec2(0.7299465240641712,0.3048128342245989) * 2. - 1.) * 2.);
#endif

  vec2 uv = (2. * gl_FragCoord.xy - iResolution.xy) / iResolution.y;

  Hit hit, firstHit;
  vec2 res;
  vec3 p, rayDir, origin, sam, ref, raf, nor, camOrigin, camDir;
  float invert, ior, offset, extinctionDist, maxDist, firstLen, bounceCount, wavelength;

  vec3 col = vec3(0);
  float focal = 3.8;
  bool refracted;

  vec3 bgCol = vec3(.22);

  invert = 1.;
  maxDist = 15.; 
    
  if (iMouse.z > 0.)
  {
    cameraMatrix *= rotX(((iMouse.y / iResolution.y) * 2. - 1.) * 2.);
    cameraMatrix *= rotZ(((iMouse.x / iResolution.x) * 2. - 1.) * 2.);
  }    

  camOrigin = -(cameraMatrix[3].xyz) * mat3(cameraMatrix);
  camDir = normalize(vec3(uv * .168, -1.) * mat3(cameraMatrix));

  firstHit = march(camOrigin, camDir, invert, maxDist, .9);
  firstLen = firstHit.len;

  float steps = 0.;

  for (float disperse = 0.; disperse < MAX_DISPERSE; disperse++)
  {
    invert = 1.;
    sam = vec3(0);

    origin = camOrigin;
    rayDir = camDir;

    extinctionDist = 0.;
    wavelength = disperse / MAX_DISPERSE;
    float rand = texture(iChannel0, gl_FragCoord.xy / vec2(textureSize(iChannel0, 0))).r;
    rand = fract(rand + float(iFrame) * 1.61803398875);

    wavelength += (rand * 2. - 1.) * (.5 / MAX_DISPERSE);
        
    bounceCount = 0.;

    for (float bounce = 0.; bounce < MAX_BOUNCE; bounce++)
    {
      if (bounce == 0.)
        hit = firstHit;
      else
        hit = march(origin, rayDir, invert, maxDist / 2., 1.);

      steps += hit.steps;

      res = hit.res;
      p = hit.p;

      if (invert < 0.)
        extinctionDist += hit.len;

      if ( res.y == 0.)
        break;

      vec3 nor = normal(p) * invert;
      ref = reflect(rayDir, nor);

      sam += light(p, ref) * .5;
      sam += pow(1. - abs(dot(rayDir, nor)), 5.) * .1;
      sam *= vec3(.85,.85,.98);

      float ior = mix(1.3, 1.6, wavelength);
      ior = invert < 0. ? ior : 1. / ior;
      raf = refract(rayDir, nor, ior);
      bool tif = raf == vec3(0);
      rayDir = tif ? ref : raf;
      offset = .01 / abs(dot(rayDir, nor));
      origin = p + offset * rayDir;
      invert *= -1.;

      bounceCount = bounce;
    }

#ifndef DARK_MODE
    sam += bounceCount == 0. ? bgCol : env(p, rayDir);	
#endif

    if (bounceCount == 0.)
    {
      col += sam * MAX_DISPERSE / 2.;
      break;
    }
    else
    {
      vec3 extinction = vec3(.3,.3,1.) * .5;
      extinction = 1. / (1. + (extinction * extinctionDist));	
      col += sam * extinction * spectrum(-wavelength+.2);
    }
  }

  col /= MAX_DISPERSE;

  gl_FragColor = vec4(col, range(4., 12., firstLen));
}
