#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform bool iIsSpacePressed;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define _G0 0.25
#define _G1 0.125
#define _G2 0.0625
#define W0 -3.0
#define W1 0.5
#define TIMESTEP 0.1
#define ADVECT_DIST 2.0
#define DV 0.70710678

float
nl (float x)
{
  return 1.0 / (1.0 + exp (W0 * (W1 * x - 0.5)));
}

vec4
gaussian (vec4 x, vec4 x_nw, vec4 x_n, vec4 x_ne, vec4 x_w, vec4 x_e, vec4 x_sw, vec4 x_s, vec4 x_se)
{
    return _G0 * x + _G1 * (x_n + x_e + x_w + x_s) + _G2 * (x_nw + x_sw + x_ne + x_se);
}

bool
reset ()
{
//  return texture(iChannel3, vec2(32.5/256.0, 0.5) ).x > 0.5;
  return iIsSpacePressed;
}

vec2
normz (vec2 x)
{
  return x == vec2(0.0, 0.0) ? vec2(0.0, 0.0) : normalize(x);
}

vec4
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

  vec4 u =    texture(iChannel0, fract(aUv));
  vec4 u_n =  texture(iChannel0, fract(aUv+n));
  vec4 u_e =  texture(iChannel0, fract(aUv+e));
  vec4 u_s =  texture(iChannel0, fract(aUv+s));
  vec4 u_w =  texture(iChannel0, fract(aUv+w));
  vec4 u_nw = texture(iChannel0, fract(aUv+nw));
  vec4 u_sw = texture(iChannel0, fract(aUv+sw));
  vec4 u_ne = texture(iChannel0, fract(aUv+ne));
  vec4 u_se = texture(iChannel0, fract(aUv+se));

  return gaussian(u, u_nw, u_n, u_ne, u_w, u_e, u_sw, u_s, u_se);
}

#define SQRT_3_OVER_2 0.86602540378
#define SQRT_3_OVER_2_INV 0.13397459621

vec2
diagH (vec2 x, vec2 x_v, vec2 x_h, vec2 x_d)
{
  return 0.5 * ((x + x_v) * SQRT_3_OVER_2_INV + (x_h + x_d) * SQRT_3_OVER_2);
}

vec2
diagV (vec2 x, vec2 x_v, vec2 x_h, vec2 x_d)
{
  return 0.5 * ((x + x_h) * SQRT_3_OVER_2_INV + (x_v + x_d) * SQRT_3_OVER_2);
}

void
main ()
{
  vec2 vUv = gl_FragCoord.xy / iResolution.xy;
  vec2 texel = 1. / iResolution.xy;

  vec2 n  = vec2(0.0, 1.0);
  vec2 ne = vec2(1.0, 1.0);
  vec2 e  = vec2(1.0, 0.0);
  vec2 se = vec2(1.0, -1.0);
  vec2 s  = vec2(0.0, -1.0);
  vec2 sw = vec2(-1.0, -1.0);
  vec2 w  = vec2(-1.0, 0.0);
  vec2 nw = vec2(-1.0, 1.0);

  vec4 u =    texture(iChannel0, fract(vUv));
  vec4 u_n =  texture(iChannel0, fract(vUv+texel*n));
  vec4 u_e =  texture(iChannel0, fract(vUv+texel*e));
  vec4 u_s =  texture(iChannel0, fract(vUv+texel*s));
  vec4 u_w =  texture(iChannel0, fract(vUv+texel*w));
  vec4 u_nw = texture(iChannel0, fract(vUv+texel*nw));
  vec4 u_sw = texture(iChannel0, fract(vUv+texel*sw));
  vec4 u_ne = texture(iChannel0, fract(vUv+texel*ne));
  vec4 u_se = texture(iChannel0, fract(vUv+texel*se));

  const float vx = 0.5;
  const float vy = SQRT_3_OVER_2;
  const float hx = SQRT_3_OVER_2;
  const float hy = 0.5;

  float di_n  = nl(distance(u_n.xy + n, u.xy));
  float di_w  = nl(distance(u_w.xy + w, u.xy));
  float di_e  = nl(distance(u_e.xy + e, u.xy));
  float di_s  = nl(distance(u_s.xy + s, u.xy));

  float di_nne = nl(distance((diagV(u.xy, u_n.xy, u_e.xy, u_ne.xy) + vec2(+ vx, + vy)), u.xy));
  float di_ene = nl(distance((diagH(u.xy, u_n.xy, u_e.xy, u_ne.xy) + vec2(+ hx, + hy)), u.xy));
  float di_ese = nl(distance((diagH(u.xy, u_s.xy, u_e.xy, u_se.xy) + vec2(+ hx, - hy)), u.xy));
  float di_sse = nl(distance((diagV(u.xy, u_s.xy, u_e.xy, u_se.xy) + vec2(+ vx, - vy)), u.xy));    
  float di_ssw = nl(distance((diagV(u.xy, u_s.xy, u_w.xy, u_sw.xy) + vec2(- vx, - vy)), u.xy));
  float di_wsw = nl(distance((diagH(u.xy, u_s.xy, u_w.xy, u_sw.xy) + vec2(- hx, - hy)), u.xy));
  float di_wnw = nl(distance((diagH(u.xy, u_n.xy, u_w.xy, u_nw.xy) + vec2(- hx, + hy)), u.xy));
  float di_nnw = nl(distance((diagV(u.xy, u_n.xy, u_w.xy, u_nw.xy) + vec2(- vx, + vy)), u.xy));

  vec2 xy_n  = u_n.xy + n - u.xy;
  vec2 xy_w  = u_w.xy + w - u.xy;
  vec2 xy_e  = u_e.xy + e - u.xy;
  vec2 xy_s  = u_s.xy + s - u.xy;

  vec2 xy_nne = (diagV(u.xy, u_n.xy, u_e.xy, u_ne.xy) + vec2(+ vx, + vy)) - u.xy;
  vec2 xy_ene = (diagH(u.xy, u_n.xy, u_e.xy, u_ne.xy) + vec2(+ hx, + hy)) - u.xy;
  vec2 xy_ese = (diagH(u.xy, u_s.xy, u_e.xy, u_se.xy) + vec2(+ hx, - hy)) - u.xy;
  vec2 xy_sse = (diagV(u.xy, u_s.xy, u_e.xy, u_se.xy) + vec2(+ vx, - vy)) - u.xy;
  vec2 xy_ssw = (diagV(u.xy, u_s.xy, u_w.xy, u_sw.xy) + vec2(- vx, - vy)) - u.xy;
  vec2 xy_wsw = (diagH(u.xy, u_s.xy, u_w.xy, u_sw.xy) + vec2(- hx, - hy)) - u.xy;
  vec2 xy_wnw = (diagH(u.xy, u_n.xy, u_w.xy, u_nw.xy) + vec2(- hx, + hy)) - u.xy;
  vec2 xy_nnw = (diagV(u.xy, u_n.xy, u_w.xy, u_nw.xy) + vec2(- vx, + vy)) - u.xy;

  vec2 ma = di_nne * xy_nne + di_ene * xy_ene + di_ese * xy_ese + di_sse * xy_sse + di_ssw * xy_ssw + di_wsw * xy_wsw + di_wnw * xy_wnw + di_nnw * xy_nnw + di_n * xy_n + di_w * xy_w + di_e * xy_e + di_s * xy_s;

  vec4 u_blur = gaussian(u, u_nw, u_n, u_ne, u_w, u_e, u_sw, u_s, u_se);

  vec4 au = advect(u.xy, vUv, texel);
  vec4 av = advect(u.zw, vUv, texel);

  vec2 dv = av.zw + TIMESTEP * ma;
  vec2 du = au.xy + TIMESTEP * dv;

  if (iMouse.z > 0.0)
  {
    vec2 d = gl_FragCoord.xy - iMouse.xy;
    float m = exp(-length(d) / 50.0);
    du += 0.2 * m * normz(d);
  }

  vec2 init = texture(iChannel1, vUv, 4.0).xy;
  if ((length(u) < 0.001 && length(init) > 0.001) || reset())
    gl_FragColor = 8.0 * (vec4(-0.5) + vec4(init.xy, init.xy));
  else
  {
    du = length(du) > 1.0 ? normz(du) : du;
    gl_FragColor = vec4(du, dv);
  }
}
