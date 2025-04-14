#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

const float _K0 = -20.0/6.0;
const float _K1 = 4.0/6.0;
const float _K2 = 1.0/6.0;
const float timestep = 0.7;
const float a0 = -0.1;
const float a1 = 2.0;
const float epsilon = 0.05;
const float delta = 4.0;
const float k1 = 1.0;
const float k2 = 0.0;
const float k3 = 1.0;
const float tc = 0.8;

void
main ()
{
  vec2 mouse = iMouse.xy / iResolution.xy;
  vec2 vUv = gl_FragCoord.xy / iResolution.xy;
  vec2 texel = 1. / iResolution.xy;

  float step_x = texel.x;
  float step_y = texel.y;
  vec2 n  = vec2(0.0, step_y);
  vec2 ne = vec2(step_x, step_y);
  vec2 e  = vec2(step_x, 0.0);
  vec2 se = vec2(step_x, -step_y);
  vec2 s  = vec2(0.0, -step_y);
  vec2 sw = vec2(-step_x, -step_y);
  vec2 w  = vec2(-step_x, 0.0);
  vec2 nw = vec2(-step_x, step_y);

  vec4 uv =    texture(iChannel0, vUv);
  vec4 uv_n =  texture(iChannel0, vUv+n);
  vec4 uv_e =  texture(iChannel0, vUv+e);
  vec4 uv_s =  texture(iChannel0, vUv+s);
  vec4 uv_w =  texture(iChannel0, vUv+w);
  vec4 uv_nw = texture(iChannel0, vUv+nw);
  vec4 uv_sw = texture(iChannel0, vUv+sw);
  vec4 uv_ne = texture(iChannel0, vUv+ne);
  vec4 uv_se = texture(iChannel0, vUv+se);

  vec4 lapl  = _K0*uv + _K1*(uv_n + uv_e + uv_w + uv_s) + _K2*(uv_nw + uv_sw + uv_ne + uv_se);

  float a = uv.x;
  float b = uv.y;
  float c = uv.z;
  float d = uv.w;

  float d_a = k1*a - k2*a*a - a*a*a - b + lapl.x;
  float d_b = epsilon*(k3*a - a1*b - a0) + delta*lapl.y;
  c = tc * c + (1.0 - tc) * d_a;
  d = tc * d + (1.0 - tc) * d_b;

  a = a + timestep * c;
  b = b + timestep * d;

  if (iMouse.z > 0.0)
  {
    float mLen = length(iMouse.xy - gl_FragCoord.xy);
    a += exp(-mLen * mLen / 100.0);
  }

  if (iFrame<10)
  {
    gl_FragColor = -0.5 + texture(iChannel1, vUv);
  }
  else
  {
    gl_FragColor = clamp(vec4(a, b, c, d), -1., 1.);
  }
}
