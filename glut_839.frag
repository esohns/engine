uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

// glut_839_common.glsl
const float PI = radians(180.);

struct Camera
{
  float fov, aspect;
  vec3 origin, target, up;
  mat3 vMat, mMat;

  float factor;
  vec3 forward, right, position, coord;
  mat3 pMat, uMat;
};

struct Object
{
  float distance;
  float id;
  vec3 position;
};

struct Ray
{
  vec3  origin;
  vec3  direction;
  float near;
  float far;
  float epsilon;
  float steps;
  float swing;

  float distance;
  vec3  position;
  vec3  normal;
  bool  hit;
  Object object;
};

Ray
lookAt (vec2 uv, inout Camera cam)
{
  cam.factor = 1.0/tan(radians(cam.fov/2.));
  cam.forward = normalize(cam.target-cam.origin); 
  cam.right = normalize(cross(cam.up, cam.forward));
  cam.up = cross(cam.forward, cam.right);
  cam.position = cam.origin + cam.factor * cam.forward;
  cam.coord = cam.position + uv.x * cam.right*cam.aspect + uv.y * cam.up;
  cam.pMat = mat3(cam.right, cam.up, cam.forward);

  Ray ray;
  {
    ray.origin = cam.mMat * cam.origin;
    ray.direction = cam.mMat * normalize( cam.pMat * cam.vMat * vec3(uv.x*cam.aspect, uv.y, cam.factor));
    ray.near = 0.01;
    ray.far = 100.;
    ray.epsilon = 0.001;
    ray.swing = 1.0;
    ray.steps = 200.;
  }

  return ray;
}

float EPS = 0.01;
#define TF_ROUND(d, R) (length(max(d, 0.)) - (R))
#define TF_BOX_ROUND(p, S, R) TF_ROUND(abs(p)-(S), R)
#define TF_BOX_ROUND1(p, S, R) TF_ROUND(abs(p)-(S) + R, R)
#define TF_BOX(p, S) TF_BOX_ROUND(p, S, 0.)
#define TF_BOX1(p, S) TF_BOX_ROUND(p, S, EPS)
#define TF_BOXE(p, S) TF_BOX_ROUND(p, S - EPS, EPS)
#define TF_BOX3D(p, S) max(abs((p).x)-vec3(S).x, max(abs((p).y)-vec3(S).y, abs((p).z)-vec3(S).z))
#define TF_BOX2D(p, S) max(abs((p).x)-vec2(S).x, abs((p).y)-vec2(S).y)
#define TF_BOX3D_SD(p, S) TF_BOX(p, S) + min(TF_BOX3D(p, S), 0.)
#define TF_BOX2D_SD(p, S) TF_BOX(p, S) + min(TF_BOX2D(p, S), 0.)

#define TF_ROUND_MIN(d, R) (-length(min(d, 0.)) + (R))
#define TF_CROSS_ROUND(p, S, R) TF_ROUND_MIN(abs(p)-(S), R)
#define TF_CROSS(p, S) TF_CROSS_ROUND(p, S, 0.)
#define TF_CROSS3D(p, S) min(abs((p).x)-vec3(S).x, min(abs((p).y)-vec3(S).y, abs((p).z)-vec3(S).z))
#define TF_CROSS2D(p, S) min(abs((p).x)-vec2(S).x, abs((p).y)-vec2(S).y)
#define TF_CROSS3D_SD(p, S) TF_CROSS(p, S) + max(TF_CROSS3D(p, S), 0.)
#define TF_CROSS2D_SD(p, S) TF_CROSS(p, S) + max(TF_CROSS2D(p, S), 0.)

#define TF_BALL(p, R) TF_ROUND(abs(p), R)
#define TF_ELLIPSE3D(p, r) min ((r).x, min((r).y, (r).z)) * TF_BALL((p)/(r), 1.)
#define TF_ELLIPSE2D(p, r) min ((r).x, (r).y) * TF_BALL((p)/(r), 1.)
#define TF_SEGMENT(p, a, b, r) TF_BALL((p)-(a) - ((b)-(a)) * clamp( dot((p)-(a),(b)-(a))/dot((b)-(a),(b)-(a)), 0.0, 1.0 ), r)

#define TF_BEFORE(p, p1) (p - (p1))
#define TF_BEFORE_EPS(p, p1) TF_ROUND(p - (p1) + EPS, EPS)
#define TF_BEFORE_PLANE(p, normal, off) dot(p, normal) - (off)
#define TF_BEFORE_ROTATE(p, ang, off) dot((p).xy, cos ((ang) + vec2 (0, 0.5 * PI))) - (off)

#define TF_AFTER(p, p1) (-p + (p1))
#define TF_AFTER_EPS(p, p1) TF_ROUND(-p + (p1) + EPS, EPS)
#define TF_AFTER_PLANE(p, normal, off) dot(-p, normal) + (off)
#define TF_AFTER_ROTATE(p, ang, off) dot((-p).xy, cos ((ang) + vec2 (0, 0.5 * PI))) + (off)

//#define TF_BETWEEN(p, p1) TF_ROUND(abs(p) - (p1) + EPS, EPS)
#define TF_BETWEEN(p, p1) (abs(p) - (p1))
#define TF_BETWEEN_EPS(p, p1) TF_ROUND(abs(p) - (p1) + EPS, EPS)
//#define TF_BETWEEN2(p, p1, p2) TF_ROUND( abs(p - 0.5*(p1+p2)) - 0.5*(p2-p1) + EPS, EPS)
#define TF_BETWEEN2(p, p1, p2) (abs(p - 0.5*((p1)+(p2))) - 0.5*((p2)-(p1)))
#if 0
#define TF_BETWEEN_PLANE(p, normal, off) AND(TF_AFTER_PLANE(p, normal, -off), TF_BEFORE_PLANE(p, normal, off))
#else
#define TF_BETWEEN_PLANE(p, normal, off) TF_BEFORE_PLANE(abs(p), normal, off)
#endif
#define TF_BETWEEN_ROTATE(p, ang, off) AND(TF_AFTER_ROTATE((p).xy, ang, -0.5*off), TF_BEFORE_ROTATE((p).xy, ang, 0.5*off))

#define TF_TRANSLATE(p, d) p -= d;
#define TF_SCALE(p, s) p /= s
#if 0
#define TF_CYL(p, R, n) {\
  float l = length(p.xy);\
  p /= l;\
  p.x = (R)*atan(p.x,p.y);\
  p.y = l - (R);\
}
#else
  #if 0
    #define TF_CYL(p, R, n) {\
      float r = min (vec2(R).x, vec2(R).y);\
      float l = r * length(p.xy/vec2(R));\
      p /= l;\
      p.x = (r) * atan(p.x,p.y);\
      p.y = l - (r);\
    }
  #else
    #define TF_CYL(p, R, n) {\
      float r = min (vec2(R).x, vec2(R).y);\
      float l = r * pow(length(pow(abs(p.xy)/vec2(R),vec2(n))), 1./float(n));\
      p /= l;\
      p.x = (r) * atan(p.x,p.y);\
      p.y = l - (r);\
    }
  #endif
#endif
#define TF_CYL_TO_DEC(p, R) p.xy = (p.y + R) * vec2(sin(p.x/R), cos(p.x/R))

#if 0
#define TF_SPH(p, R, n) {\
  float l = length(p.xyz);\
  p /= l;\
  p.x = (R)*atan(p.x,p.z);\
  p.z = (R)*acos(p.y);\
  p.y = l-(R);\
}
#else
  #if 0
    #define TF_SPH(p, R, n) {\
      float r = min (vec3(R).x, min(vec3(R).y, vec3(R).z));\
      float rr = max (vec3(R).x, max(vec3(R).y, vec3(R).z));\
      float l = r * length(p.xyz/vec3(R));\
      p /= l;\
      p.x = (r)*atan(p.x,p.z);\
      p.z = (r)*acos(p.y);\
      p.y = l - (r);\
    }
  #else
    #define TF_SPH(p, R, n) {\
      float r = min (vec3(R).x, min(vec3(R).y, vec3(R).z));\
      float l = r * pow(length(pow(abs(p)/vec3(R),vec3(n))), 1./float(n));\
      p /= l;\
      p.x = (r)*atan(p.x,p.z);\
      p.z = (r)*acos(p.y);\
      p.y = l - (r);\
    }
  #endif
#endif
#if 0
  #define TF_ROTATE(p, a) p = mat2(cos(a), sin(a), -sin(a), cos(a)) * p
#else
  #define TF_ROTATE(p, a) p = p.xy * cos (a) * vec2 (1., 1.) + p.yx * sin (a) * vec2 (-1., 1.)
#endif
#define TF_ROTATE_MAT2(a) mat2(cos(a), sin(a), -sin(a), cos(a))
#define TF_ROTATE_X(a) mat3(1.0,    0.0, 0.0,       0.0, cos(a), -sin(a),   0.0, sin(a), cos(a))
#define TF_ROTATE_Y(a) mat3(cos(a), 0.0, -sin(a),   0.0, 1.0, 0.0,   	    sin(a), 0.0, cos(a))
#define TF_ROTATE_Z(a) mat3(cos(a), -sin(a), 0.0,   sin(a), cos(a), 0.0,   	0.0, 0.0, 1.0)
#define TF_ROTATE_X_90(p) p.xyz = p.xzy
#define TF_ROTATE_Y_90(p) p.xyz = p.zyx
#define TF_ROTATE_Z_90(p) p.xyz = p.yxz

#define TF_MIRROR(p, d) p = abs(p) - (d)
#if 1
  #if 1
    #define TF_REPLICA(p, d)\
      floor((p)/(d) + 0.5);\
      p -= (d)*floor((p)/(d) + 0.5)
  #else
    #define TF_REPLICA(p, d)\
      floor((p)/(d) + 0.5);\
      p = (fract((p)/(d) + 0.5) - 0.5) * (d)
  #endif
#else
  #define TF_REPLICA(p, d) \
    floor((p)/(d) + 0.5);\
    p = mod((p) + 0.5*(d), d) - 0.5*(d)
#endif

#define TF_REPLICA_LIMIT(p, d, ida, idb) \
  clamp(floor((p)/(d) + 0.5), ida, idb);\
  p -= (d)*clamp(floor((p)/(d) + 0.5), ida, idb)
#define TF_REPLICA_LIMIT_MIRROR(p, d, id) \
  clamp(floor((p=p-0.5*(d))/(d) + 0.5), -id, id-1.);\
  p -= (d)*clamp(floor((p)/(d) + 0.5), -id, id-1.)

#define TF_REPLICA_ANGLE_POLAR(p, n, off)\
  floor(mod(atan(p.x, p.y) + off + PI /(n), 2.*PI)/(2.*PI/(n)));\
  p = vec2(atan(p.x, p.y) + off, length(p.xy));\
  p.x = mod(p.x + 0.5*(2.*PI/(n)), (2.*PI/(n))) - 0.5*(2.*PI/(n))
#if 1
  #if 1
    #define TF_REPLICA_ANGLE(p, n, off) \
      floor(mod(atan(p.x, p.y) + off + PI /(n), 2.*PI)/(2. * PI/(n)));\
      TF_ROTATE(p.xy, -off + (2.*PI/(n)) * floor(mod(atan(p.x, p.y) + off + PI /(n), 2.*PI)/(2.*PI/(n))))
  #else
    #define TF_REPLICA_ANGLE(p, n, off, id) \
      TF_CYL(p, 1., 1.);\
      p.x -= -off;\
      id = TF_REPLICA(p.x , 2.*PI/(n) );\
      TF_CYL_TO_DEC(p.xy, 1.)
      p = (p.y + 1.) * vec2(sin(p.x), cos(p.x))
  #endif
#else
  #define TF_REPLICA_ANGLE(p, n, off) \
    TF_REPLICA_ANGLE_POLAR(p, n, off);\
    p = p.y * vec2(sin(p.x), cos(p.x))
#endif

#if 0
  #define TF_HELIX_Y(p, N, R, S, A)\
    TF_CYL(p.xz, R, 1.);\
    {\
      float a = (N)*p.x/(2.*PI*(R));\
      TF_TRANSLATE(p.y, (S)*a);\
      float id = TF_REPLICA(p.y, (S));\
      p.x -= -id*(2.*PI*R)/(N);\
      TF_ROTATE(p.yz, (A) * PI * a);\
    }\
    p = p.zxy
#else
  #define TF_HELIX_Y(p, N, R, S, A)\
    TF_CYL(p.xz, R, 1.);\
    p.y -= (p.x-(A))*(S)*(N)/(2.*PI*(R));\
    p.x += (2.*PI*R)/(N) * TF_REPLICA(p.y, (S));\
    p = p.zxy
#endif

#define TF_TORUS_XZ(p, R, r) \
  TF_CYL(p.xz, R, 1.);\
  TF_CYL(p.yz, r, 1.);\
  p = p.xzy

#define TF_REPLICA_SPIN_Y(p, n, R, step, sect) \
  {\
    float a = atan(p.x, p.z)/(2.*PI);\
    p.y -= a * (n) * (step);\
    TF_REPLICA(p.y, step);\
    TF_REPLICA_ANGLE(p.xz, sect, 0.);\
    p.z -= R;\
  }\
  p = p.zxy;

#define TF_REPLICA_SPIN(p1, p2, n, step) \
  p1 -= (atan((p2).x,(p2).y)/(2. * PI))*(n)*(step);\
  TF_REPLICA(p1, (step))
#define TF_REPLICA_SPIN_LIMIT(p1, p2, n, step, ida, idb) \
  p1 -= (atan((p2).x,(p2).y)/(2. * PI))*(n)*(step);\
  TF_REPLICA_LIMIT(p1, (step), ida, idb)

#define TF_SPIRAL(p, N, R, A)\
  1.;\
  {\
    TF_CYL(p.xy, 1., 1.);\
    p.x -= A;\
    p.y -= (N)/(2.*PI)*p.x*(R) - 1.;\
    p.x += (2.*PI)/(N) * TF_REPLICA(p.y, (R));\
    p.x = 0.5*(R)*(p.x*sqrt(1.0+p.x*p.x) + log(p.x+sqrt(1.0+p.x*p.x)))*(N)/(2.*PI);\
  }
#define TF_SPIRAL_LOG0(p, N, R, A)\
  length(p.xy);\
  {\
    TF_CYL(p.xy, 1., 1.);\
    p.x -= A;\
    p.x *= (N)/(2.*PI);\
    p.y = log(p.y + 1.) - (R)*p.x;\
    p.x += TF_REPLICA(p.y, R);\
  }
#define TF_SPIRAL_LOG(p, N, R, A)\
  length(p.xy);\
  {\
    TF_CYL(p.xy, 1., 1.);\
    p.x -= A;\
    p.y = log(p.y + 1.) - (N)/(2.*PI)*p.x*(R);\
    p.x += (2.*PI)/(N) * TF_REPLICA(p.y, R);\
  }

#define TF_SPIRAL_SPH_Y(p, N, R, Sy) \
  sin(acos(p.y/length(p)));\
  {\
    float s = (2.*PI*(R))/(N);\
    TF_SPH(p, R, 1.);\
    p.z -= p.x*(Sy)/s;\
    p.x += s*TF_REPLICA(p.z, (Sy));\
    p = p.yxz;\
  }

#define TF_REPLICA_ANGLE_SPHERIC(p, nx, nz, id)\
  p = vec3( atan (p.x, p.z)*(nx)/(2. * PI), length (p), asin (p.y / length (p))*(nz)/PI );\
  if (mod(nz, 2.)==0.) p.z -= 0.5;\
  id = TF_REPLICA(p.xz, 1.)

#define TF_STEP_AFTER(p, a) step(a, p)
#define TF_STEP_BEFORE(p, a) step(p, a)
#define TF_STEP_BETWEEN(p, a) TF_STEP_BEFORE(abs(p), a)
#define TF_STEP_BETWEEN2(p, a, b) TF_STEP_AFTER(p, a)*TF_STEP_BEFORE(p, b)

#define TF_SMOOTHSTEP_AFTER_EPS(p, a, eps) smoothstep(a-0.5*eps, a+0.5*eps, p)
#define TF_SMOOTHSTEP_AFTER(p, a, b) smoothstep(a, b, p)
#define TF_SMOOTHSTEP_BEFORE_EPS(p, a, eps) (1.-smoothstep(a-0.5*eps, a+0.5*eps, p))
#define TF_SMOOTHSTEP_BEFORE(p, a, b) (1.-smoothstep(a, b, p))
#define TF_SMOOTHSTEP_BETWEEN_EPS(p, a, eps) TF_SMOOTHSTEP_BEFORE_EPS(abs(p), a, eps)
#define TF_SMOOTHSTEP_BETWEEN(p, a, b) TF_SMOOTHSTEP_BEFORE(abs(p), a, b)
#define TF_SMOOTHSTEP_BETWEEN2_EPS(p, a, b, eps) TF_SMOOTHSTEP_AFTER_EPS(p, a, eps)*TF_SMOOTHSTEP_BEFORE_EPS(p, b, eps)
#define TF_SMOOTHSTEP_BETWEEN2(p, a1, b1, a2, b2) TF_SMOOTHSTEP_AFTER(p, a1, b1)*TF_SMOOTHSTEP_BEFORE(p, a2, b2)

#define TF_TIMER(u_time, intervalCount, intervalDuration, intervalId) \
  mod(u_time, intervalDuration*intervalCount)/(intervalDuration*intervalCount);\
  intervalId = mod(floor(u_time/(intervalDuration)), (intervalCount));\
  u_time = mod(u_time, intervalDuration)
#define TF_TIMER_VIEW(col, pix, uv0_value) \
  {\
    vec2 uv0 = gl_FragCoord.xy/iResolution.xy;\
    if (uv0.y<pix/iResolution.y && uv0.x<uv0_value) col = vec3(1,0,0);\
  }

#define TF_ROUND_UP(x, f) (floor((x)/(f) + 0.5)*(f))

float
OR (float distA, float distB)
{
  return min(distA, distB);
}

float
AND (float distA, float distB)
{
  return max(distA, distB);
}

float
OR (float distA, float distB, float k)
{
  float h = clamp( 0.5 + 0.5*(distB-distA)/k, 0., 1. );
  return mix( distB, distA, h ) - k*h*(1.-h);
}

#if 0
float
AND (float distA, float distB, float k)
{
  float h = clamp( 0.5 - 0.5*(distB-distA)/k, 0., 1. );
  return mix( distB, distA, h ) + k*h*(1.-h);
}
#else
float
AND (float distA, float distB, float k)
{
  return - OR (- distA, - distB, k);
}
#endif

float
NOT (float dist)
{
  return -dist;
}

Object
OR (Object objectA, Object objectB)
{
  if (objectB.distance<objectA.distance) return objectB;
  return objectA;
}

Object
OR (Object objectA, Object objectB, float k)
{
  Object object = objectA;
  if (objectB.distance<objectA.distance) object = objectB;
  if (k!=0.) object.distance = OR(objectA.distance, objectB.distance, k);
  return object;
}

Object
AND (Object objectA, Object objectB)
{
  if (objectB.distance>objectA.distance) return objectB;
  return objectA;
}

Object
AND (Object objectA, Object objectB, float k)
{
  Object object = objectA;
  if (objectB.distance>objectA.distance) object = objectB;
  if (k!=0.) object.distance = AND(objectA.distance, objectB.distance, k);
  return object;
}

Object
NOT (Object object)
{
  object.distance = -object.distance;
  return object;
}
// glut_839_common.glsl

#define SOFT_SHADOW

float time;
#define FAR 50.
#define ID_NONE -1.

#define ID_CABLE 0.
#define ID_BOX 100.
#define ID_BALL 101.
#define ID_RECT 102.
#define ID_CAPSULE 103.

vec3
hash13 (float p)
{
   vec3 p3 = fract(vec3(p) * vec3(.1031,.11369,.13787));
   p3 += dot(p3, p3.yzx + 19.19);
   return fract(vec3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}

float
map (vec3 p, inout Object object)
{
  vec3 q;
  float d;
  
  object = Object(FAR, ID_NONE, p);

  const float N = 1.;
  const float R = 5.;
  const float S = 3.5;
  const float A = 0.;
  TF_HELIX_Y(p, N, R, S, A);

  Object OBJ1 = Object(FAR, 100., p);
  {
    q = p;
    q.y -= 2.*time;
    float id = TF_REPLICA(q.y, 2.*PI*R/5.);
    id = mod(id, 4.);
    if (id==0.)
    {
      TF_ROTATE(q.yz, time);
      TF_ROTATE(q.xz, time);
      d = TF_BOX3D(q, 0.6);
      OBJ1.id = ID_BOX;
    }
    else if (id==1.)
    {
      TF_ROTATE(q.yz, time);
      d = TF_BALL(q, 0.9);
      OBJ1.id = ID_BALL;
    }
    else if (id==2.)
    {
      TF_ROTATE(q.yz, time);
      d = TF_BOX3D(q, vec3(0.1, 0.8, 0.8));
      OBJ1.id = ID_RECT;
    }
    else if (id==3.)
    {
      TF_ROTATE(q.yz, time*2.);
      d = TF_BOX_ROUND(q, vec3(0.5, 0, 0), 0.5);
      OBJ1.id = ID_CAPSULE;
    }
    OBJ1.distance = d;
    OBJ1.position = q;
  }
  object = OR(object, OBJ1);

  const float cables = 18.;
  for (float i = 0.; i < cables; i++) {
  Object OBJ2 = Object(FAR, ID_CABLE + i, p);
  {
    float dir = i<8. ? 1. : -1.;
    q = p;
    q.y -= 2.*PI / cables*2. * i;
    TF_ROTATE(q.xz, q.y*dir);
    float dx = (sin(i * 5.) + sin(time + i)) * 0.2;
    q.x -= 0.5 - dx; 
    q.z -= (1.+dx)*TF_SMOOTHSTEP_BETWEEN(OBJ1.distance, -0.5, 1.0); 
    d = TF_BALL(q.xz, 0.05);
        OBJ2.distance = d * 0.3;
    OBJ2.position = q;
  }
  object = OR(object, OBJ2);
  }

  return object.distance;
}

float
map (vec3 p)
{
  Object object;
  return map (p, object);
}

vec3
mapNormal (vec3 p, float eps)
{
  vec2 e = vec2 (eps, -eps);
  vec4 v = vec4 (
    map (p + e.xxx), 
    map (p + e.xyy),
    map (p + e.yxy), 
    map (p + e.yyx)
  );
  return normalize (vec3 (v.x - v.y - v.z - v.w) + 2. * vec3 (v.y, v.z, v.w));
}

float
rayMarch (inout Ray ray)
{
  ray.distance = ray.near;
  float steps;
  for (float i = 0.; i < 128.; ++i)
  {
    ray.position = ray.origin + ray.direction * ray.distance;
    ray.object.distance = map(ray.position, ray.object);
    ray.hit = abs(ray.object.distance) < ray.epsilon;
    if (ray.hit) break;
    ray.distance += ray.object.distance*ray.swing;
    if (ray.distance>ray.far) break;
    steps = i + 1.;
    if (steps>ray.steps) break;
  }
  return steps;
}

float
softShadow (Ray ray, float k)
{
  float shade = 1.0;
  ray.distance = ray.near;    
  float steps = 1.;
  for ( int i = 0; i < 50; i++ )
  {
    ray.position = ray.origin + ray.direction * ray.distance;
    ray.object.distance = map(ray.position);
    shade = min( shade, smoothstep( 0.0, 1.0, k * ray.object.distance / ray.distance)); 
    ray.hit = ray.object.distance < ray.epsilon;
    if (ray.hit)
      break;
    ray.distance += min( ray.object.distance, ray.far / ray.steps * 2. ); 
    if (ray.distance > ray.far )
      break; 
    steps++;
    if (steps>ray.steps)
      break;
  }
#if 0
  return shade;
#else
  return min( max( shade, 0.0 ) + 0.5, 1.0 ); 
#endif
}

float
softShadow (Ray ray, vec3 lightDir, float k)
{
  ray.origin = ray.position;
  ray.direction = lightDir;
  return softShadow(ray, k);
}

vec3
bgCol (vec3 ro, vec3 rd)
{
  return mix(vec3 (0.5, 0.75, 1.), vec3(1.0), (0.5-rd.y));
}

vec3
lighting (Ray ray, vec3 lightDir, vec3 mCol)
{
  float sh = 1.;
#ifdef SOFT_SHADOW
  sh = softShadow(ray, lightDir, 80.);
#endif
  float diff = max (dot (ray.normal, lightDir), 0.);
  float back = max (dot (ray.normal, - normalize (vec3 (lightDir.x, 0., lightDir.z))), 0.);
  float spec = pow (max (0., dot (lightDir, reflect (ray.direction, ray.normal))), 64.);
  return mCol * ( 0.2 + 0.2 *back + diff * 0.8 * sh)  +  sh * 0.7 * spec;
}

vec3
getMaterial (Ray ray)
{
  vec3 mCol = vec3(1);

  vec3 p = ray.position;
  vec3 q = ray.object.position;
  if (ray.object.id<100.)
  {
    mCol = cos(hash13(ray.object.id)*4.) * 0.5 + 0.5;
  }
  else if (ray.object.id==ID_BOX)
  {
    mCol = vec3(0.0,0.0,1);
    float s = 0.3;
    q -= s*0.5;
    vec3 id = TF_REPLICA(q, s);
    if (mod(id.x + id.y + id.z, 2.)==0.) mCol = vec3(1);
  }
  else if (ray.object.id==ID_BALL)
  {
    mCol = vec3(1,0.0,0.0);
    float s = 1.*0.9;
    TF_SPH(q, s, 1.);
    vec2 id = TF_REPLICA(q.xz, 2.*PI*s/16.);
    if (mod(id.x + id.y, 2.)==0.) mCol = vec3(1);
  }
  else if (ray.object.id==ID_RECT)
  {
    mCol = vec3(0,1,0);
    float r_sph = 1.*0.9;
    vec2 id = TF_REPLICA(q.yz, 2.*PI*r_sph/16.);
    if (mod(id.x + id.y, 2.)==0.) mCol = vec3(1);
  }
  else if (ray.object.id==ID_CAPSULE)
  {
    mCol = vec3(1,0,1);
    float s = 0.4;
    TF_SPH(q.yxz, s, 1.);
    vec2 id = TF_REPLICA(q.yz, 2.*PI*s/16.);
    if (mod(id.x + id.y, 2.)==0.) mCol = vec3(1);
  }
  return mCol;
}

vec3
render (Ray ray)
{
  vec3 col;
  vec3 bg = bgCol (ray.origin, ray.direction);

  rayMarch(ray);

  if (ray.distance<FAR)
  {
    ray.normal = mapNormal(ray.position, 0.01);

    vec3 mCol = getMaterial(ray);

#if 1
    vec3 lightDir = -ray.direction;
#else
    vec3 lightDir = normalize(vec3(0, 2, -1));
#endif	
    col = lighting(ray, lightDir, mCol);

    float fogStart = 30.;
    vec3 fogColor = bg;
    col = mix(col, fogColor, 1.-exp(-pow(ray.distance/fogStart, 3.)));
  }
  else
    col = bg;

  return col;
}

void
main ()
{
  time = iTime;

  float aspect = iResolution.x/iResolution.y;
  vec2 uv = gl_FragCoord.xy/iResolution.xy - 0.5;

  vec2 mouse = iMouse.xy / iResolution.xy - 0.5;
  if (iMouse.xy==vec2(0)) mouse = vec2(0);

  vec2 ori = vec2(
    iMouse.z==0. ? radians(-00.) : radians(-00.) + mouse.y*PI*2.,
    iMouse.z==0. ? -0.1*time : -0.1*time + mouse.x*PI*2.
  );
#if 1
  ori.x = clamp(ori.x, -0.5*PI, 0.5*PI);
#else
  ori.x = clamp(ori.x, -radians(20.), radians(20.));
#endif

  Camera cam;
  {
    cam.fov     = 45.;
    cam.aspect  = aspect;
    cam.origin  = vec3(0,0,-24);
    cam.target  = vec3(0,0,0);
    cam.up 		= vec3(0,1,0);
    cam.vMat 	= mat3(1);//TF_ROTATE_Y(ori.y) * TF_ROTATE_X(ori.x);
    cam.mMat	= TF_ROTATE_Y(ori.y) * TF_ROTATE_X(ori.x);
  }

  Ray ray = lookAt(uv, cam);
  {
    ray.near 	= 0.01;
    ray.far  	= FAR;
    ray.epsilon = 0.001;
    ray.swing	= 1.;
    ray.steps 	= 150.;
  }
  vec3 ro = ray.origin;
  vec3 rd = ray.direction;

  vec3 color = render(ray);

  gl_FragColor = vec4(color, 1.0);
}
