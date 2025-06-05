#version 140

// glut_823_common.glsl
#define texel(a, p) texelFetch(a, ivec2(p), 0)
#define ch0 iChannel0
#define ch1 iChannel1
#define ch2 iChannel2
#define ch3 iChannel3
#define R iResolution.xy
#define PI 3.14159265

#define dt 1.
#define loop(i,x) for(int i = min(0, iFrame); i < x; i++)

#define SC 3.

#define smoothR 2.5
#define density 0.05

struct obj
{
  int id;
  vec3 X;
  float Rho;
  vec3 V;
  float Pressure;
  vec3 Color;
  float Scale;
};

float
Force (float d)
{
  return 0.2*exp(-0.05*d)-2.*exp(-0.5*d);
}

#define P 0.5
#define SN ivec2(6, 2)

ivec2 N;
ivec2 sN;
int TN;

ivec2
i2xy (ivec3 sid)
{
  return sN*ivec2(sid.x%N.x, sid.x/N.x) + sid.yz;
}

ivec3
xy2i (ivec2 p)
{
  ivec2 pi = p/sN;
  return ivec3(pi.x + pi.y*N.x, p.x%sN.x, p.y%sN.y);
}

ivec2
cross_distribution (int i)
{
  return (1<<(i/4)) * ivec2( ((i&2)/2)^1, (i&2)/2 ) * ( 2*(i%2) - 1 );
}

float
sqr (float x)
{
  return x*x + 1e-2;
}

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);
  return fract((p3.xx+p3.yz)*p3.zy);
}

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * .1031);
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.x + p3.y) * p3.z);
}

vec3
hash32 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yxz+33.33);
  return fract((p3.xxy+p3.yzz)*p3.zyx);
}

vec3
hash33 (vec3 p3)
{
  p3 = fract(p3 * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yxz+33.33);
  return fract((p3.xxy + p3.yxx)*p3.zyx);
}

vec3
hash31 (float p)
{
  vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);
  return fract((p3.xxy+p3.yzz)*p3.zyx); 
}

const float PHI = 0.5*(sqrt(5.) + 1.);

vec2
inverseSF (vec3 p, float n)
{
  float m = 1.0 - 1.0/n;

  float phi = min(atan(p.y, p.x), PI), cosTheta = p.z;

  float k  = max(2.0, floor( log(n * PI * sqrt(5.0) * (1.0 - cosTheta*cosTheta))/ log(PHI+1.0)));
  float Fk = pow(PHI, k)/sqrt(5.0);
  vec2  F  = vec2( round(Fk), round(Fk * PHI) ); // k, k+1

  vec2 ka = 2.0*F/n;
  vec2 kb = 2.0*PI*( fract((F+1.0)*PHI) - (PHI-1.0) );    

  mat2 iB = mat2( ka.y, -ka.x, 
                  kb.y, -kb.x ) / (ka.y*kb.x - ka.x*kb.y);

  vec2 c = floor( iB * vec2(phi, cosTheta - m));
  float d = 8.0;
  float j = 0.0;
  for( int s=0; s<4; s++ ) 
  {
    vec2 uv = vec2( float(s-2*(s/2)), float(s/2) );

    float i = round(dot(F, uv + c));

    float phi = 2.0*PI*fract(i*PHI);
    float cosTheta = m - 2.0*i/n;
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    vec3 q = vec3( cos(phi)*sinTheta, sin(phi)*sinTheta, cosTheta );
    float squaredDistance = dot(q-p, q-p);
    if (squaredDistance < d) 
    {
      d = squaredDistance;
      j = i;
    }
  }

  return vec2( j, sqrt(d) );
}

mat4
getPerspective (float fov, float aspect, float n, float f)
{
  float scale = tan(fov * PI / 360.) * n; 
  float r = aspect * scale, l = -r; 
  float t = scale, b = -t; 

  return mat4(2. * n / (r - l), 0, 0, 0,
              0, 2. * n / (t - b), 0, 0,
              (r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1,
              0, 0, -2. * f * n / (f - n), 0);
}

mat4
getRot (vec2 a)
{
  mat4 theta_rot = mat4(1, 0, 0, 0,
                        0, cos(a.y), sin(a.y), 0,
                        0, -sin(a.y), cos(a.y), 0,
                        0, 0, 0, 1); 
        
  mat4 phi_rot = mat4(cos(a.x), sin(a.x), 0, 0,
                      -sin(a.x), cos(a.x), 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1); 

  return transpose(phi_rot*theta_rot);
}

mat4
getModel (vec3 dx)
{
  return transpose(mat4(1, 0, 0, dx.x,
                        0, 1, 0, dx.y,
                        0, 0, 1, dx.z,
                        0, 0, 0, 1));
}
// glut_823_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

int ID;
obj O;

vec4 lnk0, lnk1;
vec4 d0, d1;

vec4 EA[SN.x];

vec4
saveObj (int i)
{
  switch(i)
  {
    case 0:  
      return vec4(O.X, O.Rho);
    case 1:
      return vec4(O.V, O.Pressure);
    case 2:
      return vec4(O.Color, O.Scale);
  }
}

obj
getObj (int id)
{
  obj o;
    
  vec4 a = texel(ch0, i2xy(ivec3(id, 0, 0))); 
  o.X = a.xyz; o.Rho = a.w;
  a = texel(ch0, i2xy(ivec3(id, 1, 0))); 
  o.V = a.xyz; o.Pressure = a.w; 
  a = texel(ch0, i2xy(ivec3(id, 2, 0))); 
  o.Color = a.xyz; o.Scale = a.w;
 
  o.id = id;
  return o;
}

void
insertion_sort (float t, int id)
{
  if(d0.x > t)
  {
    d0 = vec4(t, d0.xyz);
    lnk0 = vec4(id, lnk0.xyz);
  }
  else if(d0.y > t && d0.x < t)
  {
    d0.yzw = vec3(t, d0.yz);
    lnk0.yzw = vec3(id, lnk0.yz);
  }
  else if(d0.z > t&& d0.y < t)
  {
    d0.zw = vec2(t, d0.z);
    lnk0.zw = vec2(id, lnk0.z);
  }
  else if(d0.w > t && d0.z < t)
  {
    d0.w = t;
    lnk0.w = float(id);
  }
}

bool
iscoincidence (int id)
{
  return (id < 0) || 
          (id == ID) ||
          any(equal(lnk0,vec4(id)));
}

void
sort0 (int idtemp, int D)
{
  if (iscoincidence(idtemp))
    return;

  vec3 nbX = texel(ch0, i2xy(ivec3(idtemp, 0, 0))).xyz; 

  vec3 dx = nbX - O.X;
  int dir = int(inverseSF(dx, float(sN.x)).x);

  if(dir != D)
    return;

  float t = length(dx);

  insertion_sort(t, idtemp);
}

float
Kernel (float d, float h)
{
  return exp(-sqr(d/h))/(PI*sqr(h));
}

float
KernelGrad (float d, float h)
{
  return 2.*d*Kernel(d,h)/sqr(h);
}

vec2
borderF (vec2 p)
{
  float d = min(min(p.x,p.y),min(R.x-p.x,R.y-p.y));
  return exp(-max(d,0.)*max(d,0.))*((d==p.x)?vec2(1,0):((d==p.y)?vec2(0,1):((d==R.x-p.x)?vec2(-1,0):vec2(0,-1))));
}

void
UpdateParticle ()
{
  vec3 g = -1e-5*O.X; 
  vec3 F = g; 

  float scale = 0.21/density;
  float Rho = Kernel(0., scale);
  float avgP = 0.;
  vec3  avgC = vec3(O.Color);

  loop(j,6)
  {
    vec4 nb = texel(ch0, i2xy(ivec3(ID, j, 1)));
    loop(i,3)
    {
      if(nb[i] < 0. || nb[i] > float(TN)) continue;
      obj nbO = getObj(int(nb[i]));

      float d = distance(O.X, nbO.X);
      vec3 dv = (nbO.V - O.V);
      vec3 dx = (nbO.X - O.X);
      vec3 ndir = dx/(d+1e-3);

      float K = Kernel(d, scale);

      vec3 pressure = -0.5*( nbO.Pressure/sqr(nbO.Rho) + O.Pressure/sqr(O.Rho) )*ndir*K;
      vec3 viscosity = 3.*ndir*dot(dv,ndir)*K;

      Rho += K;
      avgC += nbO.Color;
      avgP += nbO.Pressure*K;

      F += pressure + viscosity;
    }
  }

  O.Rho = Rho;
  O.Scale = scale;

  float r = 7.;
  float D = 1.;
  float waterP = 0.08*density*(pow(abs(O.Rho/density), r) - D);
  O.Pressure = min(waterP,0.03);

  O.V += F*dt;
  O.V -= O.V*(0.5*tanh(8.*(length(O.V)-1.5))+0.5);
  O.X += O.V*dt;
}

void
main ()
{
  vec4 Q;
  vec2 pos = gl_FragCoord.xy;

  sN = SN;
  N = ivec2(R*P/vec2(sN));
  TN = N.x*N.y;
  int S = 3;

  ivec2 p = ivec2(floor(pos));
  if(any(greaterThan(p, sN*N-1)))
    discard;

  ivec3 sid = xy2i(p); ID = sid.x;
  O = getObj(ID);
  d0 = vec4(1e6); d1 = vec4(1e6);
  lnk0 = vec4(-1); lnk1 = vec4(-1);

  switch(sid.z)
  {
    case 0:
      if(sid.z >= 3)
        discard;
      float sk = 0.;

      UpdateParticle();

      if(iFrame<10)
      {
        O.X = 40.*(hash32(3.14159*pos) - 0.5);
        O.V = 0.*(hash32(3.14159*pos) - 0.5);
        O.Color = hash32(3.14159*pos);
        O.Pressure = 0.;
        O.Scale = 5.;
        O.Rho = 5.;
      }

      Q = saveObj(sid.y);
      break;

    case 1:
      vec4 nb0 = texel(ch0, i2xy(ivec3(ID, sid.y, 1)));
      loop(i,4)
      {
        sort0(int(nb0[i]), sid.y);
        vec4 nb1 = texel(ch0, i2xy(ivec3(nb0[i], (iFrame+ID)%4, 1)));
        loop(j,2)
          sort0(int(nb1[j]), sid.y);  
      }

      loop(i,4)
        sort0(int(float(TN)*hash13(vec3(iFrame, ID, i))), sid.y);

      Q = lnk0;
      break;
  }

  gl_FragColor = Q;
}

