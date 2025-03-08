#version 130

// glut_507_common.glsl
#define N 30
#define L .28
#define R iResolution.xy
#define Main void main ()
#define A(U) texture(iChannel0,(U)/R)
#define ei(a) mat2(cos(a),sin(a),-sin(a),cos(a))

vec4
hash (vec4 p)
{
  const vec4 k = vec4(0.1031, 0.11369, 0.13787, 0.16379);
  p = fract(p * k);
  p += dot(p, p.wxyz + k);
  p = fract((p.xxyz + p.yzww) * p.zyxw);
  return fract(sin(p) * 43758.5453);
}

float
circle (vec3 p, vec3 d, float r)
{
  // |p + d*x| = r
  // p.p-rr + 2pdx+ddx^2 = 0
  float b = 2.*dot(p,d);
  float a = dot(p,p)-r*r;
  float det = b*b-4.*a;
  if (det < 0.) return -1.;
  vec2 o = 0.5*(-b+vec2(-1,1)*sqrt(det));
  if (o.x < 0.) return o.y;
  else return min(o.x,o.y);
}

float
intersectCube (vec3 p, vec3 d, vec3 lo, vec3 hi, out vec3 no)
{
  vec3 tLo = (lo - p) / d;
  vec3 tHi = (hi - p) / d;
  vec3 tMin = min(tLo, tHi);
  vec3 tMax = max(tLo, tHi);
  float tEntry = max(tMin.x, max(tMin.y, tMin.z));
  float tExit = min(tMax.x, min(tMax.y, tMax.z));

  if (tEntry > tExit || tExit < 0.0)
  {
    return -1.0; // No intersection
  }

  if (tEntry > 0.0)
  {
    if (tEntry == tMin.x) no = vec3(-sign(d.x), 0., 0.);
    else if (tEntry == tMin.y) no = vec3(0., -sign(d.y), 0.);
    else if (tEntry == tMin.z) no = vec3(0., 0., -sign(d.z));
    return tEntry;
  }
  else
  {
    if (tExit == tMax.x) no = vec3(sign(d.x), 0., 0.);
    else if (tExit == tMax.y) no = vec3(0., sign(d.y), 0.);
    else if (tExit == tMax.z) no = vec3(0., 0., sign(d.z));
    return tExit;
  }
}
// glut_507_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

Main
{
  if (gl_FragCoord.x > float(N)||gl_FragCoord.y>6.)
    discard;

  vec3 p[N];
  vec3 v[N];
  vec3 q[N];
  mat3 o[N];
  vec3 f[N];
  for (int i = 0; i< N; i++)
  {
      p[i] = A(vec2(i,0)+.5).xyz;
      v[i] = A(vec2(i,1)+.5).xyz;
      q[i] = A(vec2(i,2)+.5).xyz;
      o[i][0] = A(vec2(i,3)+.5).xyz;
      o[i][1] = A(vec2(i,4)+.5).xyz;
      o[i][2] = A(vec2(i,5)+.5).xyz;
  }
  for (int k = 0; k < 20;k++)
  {
    for (int i = 0; i < N; i++)
    {
      vec3 a = p[i];
      f[i] = vec3(0,-.00001,0);
      f[i].yz *= ei(-1.5*sin(.33*iTime));
      f[i].xz *= ei(iTime);
      if (a.x>1.-L) f[i].x -= .5*(a.x-(1.-L));
      if (a.y>1.-L) f[i].y -= .5*(a.y-(1.-L));
      if (a.z>1.-L) f[i].z -= .5*(a.z-(1.-L));
        
      if (-a.x>1.-L) f[i].x -= .5*(a.x+(1.-L));
      if (-a.y>1.-L) f[i].y -= .5*(a.y+(1.-L));
      if (-a.z>1.-L) f[i].z -= .5*(a.z+(1.-L));
      for (int j = 0; j < N; j++) if (j!=i)
      {
        vec3 b = p[j];
        vec3 r = b-a;
        float l = length(r);
        if (l>0.&&l<2.*L)
          f[i] -= .1*r/l*(2.*L-l);
      }
    }
    for (int i = 0; i < N; i++)
    {
      v[i] += f[i];
      p[i] += f[i]+v[i];
    }
  }

  if (iFrame == 1)
  {
    for (int i = 0; i< N; i++)
      p[i] = hash(vec4(i,i,0,0)).xyz*2.-1.;
  }
    
  int x = int(gl_FragCoord.x);
  int y = int(gl_FragCoord.y);
  for (int i = 0; i< N; i++)
  {
    if (x==i&&y==0) gl_FragColor.xyz = p[i];
    if (x==i&&y==1) gl_FragColor.xyz = v[i];
    if (x==i&&y==2) gl_FragColor.xyz = q[i];
    if (x==i&&y==3) gl_FragColor.xyz = o[i][0];
    if (x==i&&y==4) gl_FragColor.xyz = o[i][1];
    if (x==i&&y==5) gl_FragColor.xyz = o[i][2];
  }
}
