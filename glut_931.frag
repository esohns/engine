#version 130

// glut_931_common.glsl
vec2
hash (vec2 p)
{
  p = vec2( dot(p,vec2(127.1,311.7)),
            dot(p,vec2(269.5,183.3)) );
  return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float
noise (vec2 p)
{
  const float K1 = 0.366025404;
  const float K2 = 0.211324865;

  vec2 i = floor( p + (p.x+p.y)*K1 );
  
  vec2 a = p - i + (i.x+i.y)*K2;
  vec2 o = step(a.yx,a.xy);    
  vec2 b = a - o + K2;
  vec2 c = a - 1.0 + 2.0*K2;

  vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );

  vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));

  return dot( n, vec3(70.0) );
}

float
G1V (float dnv, float k)
{
  return 1.0/(dnv*(1.0-k)+k);
}

float
ggx (vec3 n, vec3 v, vec3 l, float rough, float f0)
{
  float alpha = rough*rough;
  vec3 h = normalize(v+l);
  float dnl = clamp(dot(n,l), 0.0, 1.0);
  float dnv = clamp(dot(n,v), 0.0, 1.0);
  float dnh = clamp(dot(n,h), 0.0, 1.0);
  float dlh = clamp(dot(l,h), 0.0, 1.0);
  float f, d, vis;
  float asqr = alpha*alpha;
  const float pi = 3.14159;
  float den = dnh*dnh*(asqr-1.0)+1.0;
  d = asqr/(pi * den * den);
  dlh = pow(1.0-dlh, 5.0);
  f = f0 + (1.0-f0)*dlh;
  float k = alpha/1.0;
  vis = G1V(dnl, k)*G1V(dnv, k);
  float spec = dnl * d * f * vis;
  return spec;
}
// glut_931_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform bvec4 iKeys; // space
uniform sampler2D iChannel0;
uniform sampler2D iChannel2;

#define _G0 0.25
#define _G1 0.125
#define _G2 0.0625
#define W0 20.0
#define W1 0.5
#define TIMESTEP 0.4
#define ADVECT_DIST 1.0
#define DV 0.70710678
#define TWO_PI 6.28318530718
#define PI 3.14159265359

float
nl (float x)
{
  return 1.0 / (1.0 + exp(W0 * (W1 * x - 0.5))); 
}

vec3
gaussian (vec3 x, vec3 x_nw, vec3 x_n, vec3 x_ne, vec3 x_w, vec3 x_e, vec3 x_sw, vec3 x_s, vec3 x_se)
{
  return _G0*x + _G1*(x_n + x_e + x_w + x_s) + _G2*(x_nw + x_sw + x_ne + x_se);
}

vec3
advect (vec2 ab, vec2 vUv, vec2 step)
{
  vec2 aUv = vUv - ab * ADVECT_DIST * step;

  vec2 n  = vec2(0.0, step.y);
  vec2 ne = vec2(step.x, step.y);
  vec2 e  = vec2(step.x, 0.0);
  vec2 se = vec2(step.x, -step.y);
  vec2 s  = vec2(0.0, -step.y);
  vec2 sw = vec2(-step.x, -step.y);
  vec2 w  = vec2(-step.x, 0.0);
  vec2 nw = vec2(-step.x, step.y);

  vec3 u =    texture(iChannel0, fract(aUv)).xyz;
  vec3 u_n =  texture(iChannel0, fract(aUv+n)).xyz;
  vec3 u_e =  texture(iChannel0, fract(aUv+e)).xyz;
  vec3 u_s =  texture(iChannel0, fract(aUv+s)).xyz;
  vec3 u_w =  texture(iChannel0, fract(aUv+w)).xyz;
  vec3 u_nw = texture(iChannel0, fract(aUv+nw)).xyz;
  vec3 u_sw = texture(iChannel0, fract(aUv+sw)).xyz;
  vec3 u_ne = texture(iChannel0, fract(aUv+ne)).xyz;
  vec3 u_se = texture(iChannel0, fract(aUv+se)).xyz;

  return gaussian(u, u_nw, u_n, u_ne, u_w, u_e, u_sw, u_s, u_se);
}

bool
reset ()
{
  return iKeys.x; // space
}

vec3
normz (vec3 x)
{
	return x == vec3(0.0) ? vec3(0.0) : normalize(x);
}

vec3
diagH (vec3 x, vec3 x_v, vec3 x_h, vec3 x_d)
{
  const float xd = sqrt(3.0) / 2.0;
  const float xi = 1.0 - xd;
  return 0.5 * ((x + x_v) * xi + (x_h + x_d) * xd);
}

vec3
diagV (vec3 x, vec3 x_v, vec3 x_h, vec3 x_d)
{
  const float xd = sqrt(3.0) / 2.0;
  const float xi = 1.0 - xd;
  return 0.5 * ((x + x_h) * xi + (x_v + x_d) * xd);
}

void
main ()
{
  vec2 vUv = gl_FragCoord.xy / iResolution;
  vec2 texel = 1. / iResolution;

  vec3 n  = vec3(0.0,   1.0, 0.0);
  vec3 ne = vec3(1.0,   1.0, 0.0);
  vec3 e  = vec3(1.0,   0.0, 0.0);
  vec3 se = vec3(1.0,  -1.0, 0.0);
  vec3 s  = vec3(0.0,  -1.0, 0.0);
  vec3 sw = vec3(-1.0, -1.0, 0.0);
  vec3 w  = vec3(-1.0,  0.0, 0.0);
  vec3 nw = vec3(-1.0,  1.0, 0.0);

  vec3 u =    texture(iChannel2, fract(vUv)).xyz;
  vec3 u_n =  texture(iChannel2, fract(vUv+texel*n.xy)).xyz;
  vec3 u_e =  texture(iChannel2, fract(vUv+texel*e.xy)).xyz;
  vec3 u_s =  texture(iChannel2, fract(vUv+texel*s.xy)).xyz;
  vec3 u_w =  texture(iChannel2, fract(vUv+texel*w.xy)).xyz;
  vec3 u_nw = texture(iChannel2, fract(vUv+texel*nw.xy)).xyz;
  vec3 u_sw = texture(iChannel2, fract(vUv+texel*sw.xy)).xyz;
  vec3 u_ne = texture(iChannel2, fract(vUv+texel*ne.xy)).xyz;
  vec3 u_se = texture(iChannel2, fract(vUv+texel*se.xy)).xyz;
    
  vec3 v =    texture(iChannel0, fract(vUv)).xyz;
  vec3 v_n =  texture(iChannel0, fract(vUv+texel*n.xy)).xyz;
  vec3 v_e =  texture(iChannel0, fract(vUv+texel*e.xy)).xyz;
  vec3 v_s =  texture(iChannel0, fract(vUv+texel*s.xy)).xyz;
  vec3 v_w =  texture(iChannel0, fract(vUv+texel*w.xy)).xyz;
  vec3 v_nw = texture(iChannel0, fract(vUv+texel*nw.xy)).xyz;
  vec3 v_sw = texture(iChannel0, fract(vUv+texel*sw.xy)).xyz;
  vec3 v_ne = texture(iChannel0, fract(vUv+texel*ne.xy)).xyz;
  vec3 v_se = texture(iChannel0, fract(vUv+texel*se.xy)).xyz;

  const float vx = 0.5;
  const float vy = sqrt(3.0) / 2.0;
  const float hx = vy;
  const float hy = vx;

  float di_n  = nl(distance(u_n + n, u));
  float di_w  = nl(distance(u_w + w, u));
  float di_e  = nl(distance(u_e + e, u));
  float di_s  = nl(distance(u_s + s, u));

  float di_nne = nl(distance((diagV(u, u_n, u_e, u_ne) + vec3(+ vx, + vy, 0.0)), u));
  float di_ene = nl(distance((diagH(u, u_n, u_e, u_ne) + vec3(+ hx, + hy, 0.0)), u));
  float di_ese = nl(distance((diagH(u, u_s, u_e, u_se) + vec3(+ hx, - hy, 0.0)), u));
  float di_sse = nl(distance((diagV(u, u_s, u_e, u_se) + vec3(+ vx, - vy, 0.0)), u));    
  float di_ssw = nl(distance((diagV(u, u_s, u_w, u_sw) + vec3(- vx, - vy, 0.0)), u));
  float di_wsw = nl(distance((diagH(u, u_s, u_w, u_sw) + vec3(- hx, - hy, 0.0)), u));
  float di_wnw = nl(distance((diagH(u, u_n, u_w, u_nw) + vec3(- hx, + hy, 0.0)), u));
  float di_nnw = nl(distance((diagV(u, u_n, u_w, u_nw) + vec3(- vx, + vy, 0.0)), u));

  vec3 xy_n  = u_n + n - u;
  vec3 xy_w  = u_w + w - u;
  vec3 xy_e  = u_e + e - u;
  vec3 xy_s  = u_s + s - u;

  vec3 xy_nne = (diagV(u, u_n, u_e, u_ne) + vec3(+ vx, + vy, 0.0)) - u;
  vec3 xy_ene = (diagH(u, u_n, u_e, u_ne) + vec3(+ hx, + hy, 0.0)) - u;
  vec3 xy_ese = (diagH(u, u_s, u_e, u_se) + vec3(+ hx, - hy, 0.0)) - u;
  vec3 xy_sse = (diagV(u, u_s, u_e, u_se) + vec3(+ vx, - vy, 0.0)) - u;
  vec3 xy_ssw = (diagV(u, u_s, u_w, u_sw) + vec3(- vx, - vy, 0.0)) - u;
  vec3 xy_wsw = (diagH(u, u_s, u_w, u_sw) + vec3(- hx, - hy, 0.0)) - u;
  vec3 xy_wnw = (diagH(u, u_n, u_w, u_nw) + vec3(- hx, + hy, 0.0)) - u;
  vec3 xy_nnw = (diagV(u, u_n, u_w, u_nw) + vec3(- vx, + vy, 0.0)) - u;

  float t0 = clamp(acos(dot(xy_n, xy_nne)   / (length(xy_n)   * length(xy_nne))), 0.0, PI);
  float t1 = clamp(acos(dot(xy_nne, xy_ene) / (length(xy_nne) * length(xy_ene))), 0.0, PI);
  float t2 = clamp(acos(dot(xy_ene, xy_e)   / (length(xy_ene) * length(xy_e)))  , 0.0, PI);
  float t3 = clamp(acos(dot(xy_e, xy_ese)   / (length(xy_e)   * length(xy_ese))), 0.0, PI);
  float t4 = clamp(acos(dot(xy_ese, xy_sse) / (length(xy_ese) * length(xy_sse))), 0.0, PI);
  float t5 = clamp(acos(dot(xy_sse, xy_s)   / (length(xy_sse) * length(xy_s)))  , 0.0, PI);
  float t6 = clamp(acos(dot(xy_s, xy_ssw)   / (length(xy_s)   * length(xy_ssw))), 0.0, PI);
  float t7 = clamp(acos(dot(xy_ssw, xy_wsw) / (length(xy_ssw) * length(xy_wsw))), 0.0, PI);
  float t8 = clamp(acos(dot(xy_wsw, xy_w)   / (length(xy_wsw) * length(xy_w)))  , 0.0, PI);
  float t9 = clamp(acos(dot(xy_w, xy_wnw)   / (length(xy_w)   * length(xy_wnw))), 0.0, PI);
  float t10= clamp(acos(dot(xy_wnw, xy_nnw) / (length(xy_wnw) * length(xy_nnw))), 0.0, PI);
  float t11= clamp(acos(dot(xy_nnw, xy_n)   / (length(xy_nnw) * length(xy_n)))  , 0.0, PI);

  float gcurve = TWO_PI - (t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11);

  vec3 ma = di_nne * xy_nne + di_ene * xy_ene + di_ese * xy_ese + di_sse * xy_sse + di_ssw * xy_ssw + di_wsw * xy_wsw + di_wnw * xy_wnw + di_nnw * xy_nnw + di_n * xy_n + di_w * xy_w + di_e * xy_e + di_s * xy_s;

//  vec3 v_blur = gaussian(v, v_nw, v_n, v_ne, v_w, v_e, v_sw, v_s, v_se);

  float gcs = 2.87+10000.0*gcurve;

  vec3 auv = advect(v.xy, vUv, texel);
  auv = advect(48.0 * gcs * (u - auv).xy, vUv, texel);

  vec3 dv = auv + TIMESTEP * ma;
  if (iMouse.z > 0.0)
  {
    vec2 d = gl_FragCoord.xy - iMouse.xy;
    float m = exp(-length(d) / 50.0);
    dv += m * normz(vec3(d, 0.0));
  }

  if (iFrame < 10 || reset ())
  {
    vec3 rnd = vec3(noise(16.0 * vUv + 1.1), noise(16.0 * vUv + 2.2), noise(16.0 * vUv + 3.3));
    gl_FragColor = vec4(rnd,0);
  }
  else
  {
    float ld = length(dv);
    dv = dv - 0.005* ld*ld*ld*normz(dv);
    gl_FragColor = vec4(dv, gcurve);
  }
}
