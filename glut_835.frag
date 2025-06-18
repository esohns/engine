#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

vec3
random3 (vec3 c)
{
  float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
  vec3 r;
  r.z = fract(512.0*j);
  j *= .125;
  r.x = fract(512.0*j);
  j *= .125;
  r.y = fract(512.0*j);
  return r-0.5;
}

const float F3 =  0.3333333;
const float G3 =  0.1666667;
float
simplex3d (vec3 p)
{
   vec3 s = floor(p + dot(p, vec3(F3)));
   vec3 x = p - s + dot(s, vec3(G3));

   vec3 e = step(vec3(0.0), x - x.yzx);
   vec3 i1 = e*(1.0 - e.zxy);
   vec3 i2 = 1.0 - e.zxy*(1.0 - e);

   vec3 x1 = x - i1 + G3;
   vec3 x2 = x - i2 + 2.0*G3;
   vec3 x3 = x - 1.0 + 3.0*G3;

   vec4 w, d;

   w.x = dot(x, x);
   w.y = dot(x1, x1);
   w.z = dot(x2, x2);
   w.w = dot(x3, x3);

   w = max(0.6 - w, 0.0);

   d.x = dot(random3(s), x);
   d.y = dot(random3(s + i1), x1);
   d.z = dot(random3(s + i2), x2);
   d.w = dot(random3(s + 1.0), x3);

   w *= w;
   w *= w;
   d *= w;

   return dot(d, vec4(52.0));
}

vec2
pen (float t)
{
  t *= 0.05;
  return 0.5 * iResolution.xy * vec2(simplex3d(vec3(t,0,0)) + 1.,
                                     simplex3d(vec3(0,t,0)) + 1.);
}

#define T(p) texture(iChannel0,(p)/iResolution.xy)
#define length2(p) dot(p,p)

#define dt 0.15
#define K 0.2
#define nu 0.5
#define kappa 0.1

void
main ()
{
  vec4 c;
  vec2 p = gl_FragCoord.xy;
  
  if(iFrame < 10)
  {
    c = vec4(0,0,1,0);
    gl_FragColor = c;
    return;
  }

  c = T(p);

  vec4 n = T(p + vec2(0,1));
  vec4 e = T(p + vec2(1,0));
  vec4 s = T(p - vec2(0,1));
  vec4 w = T(p - vec2(1,0));

  vec4 laplacian = (n + e + s + w - 4.*c);

  vec4 dx = (e - w)/2.;
  vec4 dy = (n - s)/2.;

  float div = dx.x + dy.y;

  c.z -= dt*(dx.z * c.x + dy.z * c.y + div * c.z);

  c.xyw = T(p - dt*c.xy).xyw;

  c.xyw += dt * vec3(nu,nu,kappa) * laplacian.xyw;

  c.xy -= K * vec2(dx.z,dy.z);

  vec2 m = pen(iTime);
  c.xyw += dt * exp(-length2(p - m)/50.) * vec3(m - pen(iTime-0.1), 1);

  c.w -= dt*0.0005;

  c.xyzw = clamp(c.xyzw, vec4(-5,-5,0.5,0), vec4(5,5,3,5));

  gl_FragColor = c;
}
