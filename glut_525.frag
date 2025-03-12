// glut_525_common.glsl
#define TURBULENCE_SCALES 11
#define VORTICITY_SCALES 11
#define POISSON_SCALES 11

#define RECALCULATE_OFFSET
#define ADVECTION_STEPS 3
#define ADVECTION_SCALE 40.0
#define ADVECTION_TURBULENCE 1.0
#define VELOCITY_TURBULENCE 0.0000
#define VELOCITY_CONFINEMENT 0.01
#define VELOCITY_LAPLACIAN 0.02
#define ADVECTION_CONFINEMENT 0.6
#define ADVECTION_DIVERGENCE  0.0
#define ADVECTION_VELOCITY -0.05
#define DIVERGENCE_MINIMIZATION 0.1
#define DIVERGENCE_LOOKAHEAD 1.0
#define LAPLACIAN_LOOKAHEAD 1.0
#define DAMPING 0.0001
#define VELOCITY_SCALE 1.0
#define UPDATE_SMOOTHING 0.0

#define TURB_ISOTROPY 0.9
#define CURL_ISOTROPY 0.6
#define CONF_ISOTROPY 0.25
#define POIS_ISOTROPY 0.16

#define PREMULTIPLY_CURL

//#define BLUR_TURBULENCE
//#define BLUR_CONFINEMENT
//#define BLUR_VELOCITY

//#define TURB_W_FUNCTION 1.0/float(i+1)
#define TURB_W_FUNCTION 1.0
//#define TURB_W_FUNCTION float(i+1)

//#define CURL_W_FUNCTION 1.0/float(1 << i)
#define CURL_W_FUNCTION 1.0/float(i+1)
//#define CURL_W_FUNCTION 1.0

//#define CONF_W_FUNCTION 1.0/float(i+1)
#define CONF_W_FUNCTION 1.0
//#define CONF_W_FUNCTION float(i+1)
//#define CONF_W_FUNCTION float(1 << i)

//#define POIS_W_FUNCTION 1.0
#define POIS_W_FUNCTION 1.0/float(i+1)
//#define POIS_W_FUNCTION 1.0/float(1 << i)
//#define POIS_W_FUNCTION float(i+1)
//#define POIS_W_FUNCTION float(1 << i)

//#define USE_PRESSURE_ADVECTION
#define PRESSURE_ADVECTION 0.0002 // higher values more likely to cause blowup if laplacian > 0.0
#define PRESSURE_LAPLACIAN 0.1 // [0..0.3] higher values more likely to cause blowup
#define PRESSURE_UPDATE_SMOOTHING 0.0 // [0..1]

#define MOUSE_AMP 0.05
#define MOUSE_RADIUS 0.001

//#define CENTER_PUMP
#define PUMP_SCALE 0.001
#define PUMP_CYCLE 0.2

vec4
normz (vec4 x)
{
  return x.xyz == vec3(0) ? vec4(0,0,0,x.w) : vec4(normalize(x.xyz),0);
}

vec3
normz (vec3 x)
{
  return x == vec3(0) ? vec3(0) : normalize(x);
}

vec2
normz (vec2 x)
{
  return x == vec2(0) ? vec2(0) : normalize(x);
}

float
softmax (float a, float b, float k)
{
  return log(exp(k*a)+exp(k*b))/k;
}

float
softmin (float a, float b, float k)
{
  return -log(exp(-k*a)+exp(-k*b))/k;
}

vec4
softmax (vec4 a, vec4 b, float k)
{
  return log(exp(k*a)+exp(k*b))/k;
}

vec4
softmin (vec4 a, vec4 b, float k)
{
  return -log(exp(-k*a)+exp(-k*b))/k;
}

float
softclamp (float a, float b, float x, float k)
{
  return (softmin(b,softmax(a,x,k),k) + softmax(a,softmin(b,x,k),k)) / 2.0;
}

vec4
softclamp (vec4 a, vec4 b, vec4 x, float k)
{
  return (softmin(b,softmax(a,x,k),k) + softmax(a,softmin(b,x,k),k)) / 2.0;
}

vec4
softclamp (float a, float b, vec4 x, float k)
{
  return (softmin(vec4(b),softmax(vec4(a),x,k),k) + softmax(vec4(a),softmin(vec4(b),x,k),k)) / 2.0;
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

vec3
light (vec2 uv, float BUMP, float SRC_DIST, vec2 dxy, float time, inout vec3 avd)
{
  vec3 sp = vec3(uv-0.5, 0);
  vec3 light = vec3(cos(time/2.0)*0.5, sin(time/2.0)*0.5, -SRC_DIST);
  vec3 ld = light - sp;
  float lDist = max(length(ld), 0.001);
  ld /= lDist;
  avd = reflect(normalize(vec3(BUMP*dxy, -1.0)), vec3(0,1,0));
  return ld;
}

float
hash1 (uint n)
{
  n = (n << 13U) ^ n;
  n = n * (n * n * 15731U + 789221U) + 1376312589U;
  return float( n & uvec3(0x7fffffffU))/float(0x7fffffff);
}

vec3
hash3 (uint n)
{
  n = (n << 13U) ^ n;
  n = n * (n * n * 15731U + 789221U) + 1376312589U;
  uvec3 k = n * uvec3(n,n*16807U,n*48271U);
  return vec3( k & uvec3(0x7fffffffU))/float(0x7fffffff);
}

vec4
rand4 (vec2 fragCoord, vec2 resolution, int frame )
{
  uvec2 p = uvec2(fragCoord);
  uvec2 r = uvec2(resolution);
  uint c = p.x + r.x*p.y + r.x*r.y*uint(frame);
  return vec4(hash3(c),hash1(c + 75132895U));
}
// glut_525_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

#define TURBULENCE_SAMPLER iChannel3
#define CONFINEMENT_SAMPLER iChannel2
#define POISSON_SAMPLER iChannel1
#define VELOCITY_SAMPLER iChannel0

#define V(d) texture(TURBULENCE_SAMPLER, fract(uv+(d+0.))).xy

vec2
gaussian_turbulence (vec2 uv)
{
  vec2 texel = 1.0/iResolution.xy;
  vec4 t = vec4(texel, -texel.y, 0);

  vec2 d =    V( t.ww); vec2 d_n =  V( t.wy); vec2 d_e =  V( t.xw);
  vec2 d_s =  V( t.wz); vec2 d_w =  V(-t.xw); vec2 d_nw = V(-t.xz);
  vec2 d_sw = V(-t.xy); vec2 d_ne = V( t.xy); vec2 d_se = V( t.xz);
    
  return 0.25 * d + 0.125 * (d_e + d_w + d_n + d_s) + 0.0625 * (d_ne + d_nw + d_se + d_sw);
}

#define C(d) texture(CONFINEMENT_SAMPLER, fract(uv+(d+0.))).xy

vec2
gaussian_confinement (vec2 uv)
{
  vec2 texel = 1.0/iResolution.xy;
  vec4 t = vec4(texel, -texel.y, 0);

  vec2 d =    C( t.ww); vec2 d_n =  C( t.wy); vec2 d_e =  C( t.xw);
  vec2 d_s =  C( t.wz); vec2 d_w =  C(-t.xw); vec2 d_nw = C(-t.xz);
  vec2 d_sw = C(-t.xy); vec2 d_ne = C( t.xy); vec2 d_se = C( t.xz);
    
  return 0.25 * d + 0.125 * (d_e + d_w + d_n + d_s) + 0.0625 * (d_ne + d_nw + d_se + d_sw);
}

#define D(d) texture(POISSON_SAMPLER, fract(uv+d)).x

vec2
diff (vec2 uv)
{
  vec2 texel = 1.0/iResolution.xy;
  vec4 t = vec4(texel, -texel.y, 0);

  float d =    D( t.ww); float d_n =  D( t.wy); float d_e =  D( t.xw);
  float d_s =  D( t.wz); float d_w =  D(-t.xw); float d_nw = D(-t.xz);
  float d_sw = D(-t.xy); float d_ne = D( t.xy); float d_se = D( t.xz);
    
  return vec2(0.5 * (d_e - d_w) + 0.25 * (d_ne - d_nw + d_se - d_sw),
              0.5 * (d_n - d_s) + 0.25 * (d_ne + d_nw - d_se - d_sw));
}

#define N(d) texture(VELOCITY_SAMPLER, fract(uv+(d+0.)))

vec4
gaussian_velocity (vec2 uv)
{
  vec2 texel = 1.0/iResolution.xy;
  vec4 t = vec4(texel, -texel.y, 0);

  vec4 d =    N( t.ww); vec4 d_n =  N( t.wy); vec4 d_e =  N( t.xw);
  vec4 d_s =  N( t.wz); vec4 d_w =  N(-t.xw); vec4 d_nw = N(-t.xz);
  vec4 d_sw = N(-t.xy); vec4 d_ne = N( t.xy); vec4 d_se = N( t.xz);

  return 0.25 * d + 0.125 * (d_e + d_w + d_n + d_s) + 0.0625 * (d_ne + d_nw + d_se + d_sw);
}

vec2
vector_laplacian (vec2 uv)
{
  const float _K0 = -20.0/6.0, _K1 = 4.0/6.0, _K2 = 1.0/6.0;
  vec2 texel = 1.0/iResolution.xy;
  vec4 t = vec4(texel, -texel.y, 0);

  vec4 d =    N( t.ww); vec4 d_n =  N( t.wy); vec4 d_e =  N( t.xw);
  vec4 d_s =  N( t.wz); vec4 d_w =  N(-t.xw); vec4 d_nw = N(-t.xz);
  vec4 d_sw = N(-t.xy); vec4 d_ne = N( t.xy); vec4 d_se = N( t.xz);

  return (_K0 * d + _K1 * (d_e + d_w + d_n + d_s) + _K2 * (d_ne + d_nw + d_se + d_sw)).xy;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;
  vec2 tx = 1.0/iResolution.xy;

  vec2 turb, confine, div, delta_v, offset, lapl = vec2(0);
  vec4 vel, adv = vec4(0);
  vec4 init = N(0.0);

#ifdef RECALCULATE_OFFSET
  for (int i = 0; i < ADVECTION_STEPS; i++)
  {
#ifdef BLUR_TURBULENCE
    turb = gaussian_turbulence(uv + tx * offset);
#else
    turb = V(tx * offset);
#endif

#ifdef BLUR_CONFINEMENT
    confine = gaussian_confinement(uv + tx * offset);
#else
    confine = C(tx * offset);
#endif

#ifdef BLUR_VELOCITY
    vel = gaussian_velocity(uv + tx * offset);
#else
    vel = N(tx * offset);
#endif

    // an alternative, but seems to give less smooth results:
    // offset += (1.0 / float(ADVECTION_STEPS)) * ...
    offset = (float(i+1) / float(ADVECTION_STEPS)) * - ADVECTION_SCALE * (ADVECTION_VELOCITY * vel.xy + ADVECTION_TURBULENCE * turb - ADVECTION_CONFINEMENT * confine + ADVECTION_DIVERGENCE * div);

    div = diff(uv + tx * DIVERGENCE_LOOKAHEAD * offset);

    lapl = vector_laplacian(uv + tx * LAPLACIAN_LOOKAHEAD * offset);

    adv += N(tx * offset);

    delta_v += VELOCITY_LAPLACIAN * lapl + VELOCITY_TURBULENCE * turb + VELOCITY_CONFINEMENT * confine - DAMPING * vel.xy - DIVERGENCE_MINIMIZATION * div;
  }
  adv /= float(ADVECTION_STEPS);
  delta_v /= float(ADVECTION_STEPS);
#else
#ifdef BLUR_TURBULENCE
  turb = gaussian_turbulence(uv);
#else
  turb = V();
#endif

#ifdef BLUR_CONFINEMENT
  confine = gaussian_confinement(uv);
#else
  confine = C();
#endif

#ifdef BLUR_VELOCITY
  vel = gaussian_velocity(uv);
#else
  vel = N();
#endif

  offset = - ADVECTION_SCALE * (ADVECTION_VELOCITY * vel.xy + ADVECTION_TURBULENCE * turb - ADVECTION_CONFINEMENT * confine + ADVECTION_DIVERGENCE * div);

  div = diff(uv + tx * DIVERGENCE_LOOKAHEAD * offset);

  lapl = vector_laplacian(uv + tx * LAPLACIAN_LOOKAHEAD * offset);

  delta_v += VELOCITY_LAPLACIAN * lapl + VELOCITY_TURBULENCE * turb + VELOCITY_CONFINEMENT * confine - DAMPING * vel.xy - DIVERGENCE_MINIMIZATION * div;

  for (int i = 0; i < ADVECTION_STEPS; i++)
  {
    adv += N((float(i+1) / float(ADVECTION_STEPS)) * tx * offset);   
  }   
  adv /= float(ADVECTION_STEPS);
#endif

  vec2 pq = 2.0*(uv*2.0-1.0) * vec2(1,tx.x/tx.y);
#ifdef CENTER_PUMP
  vec2 pump = sin(PUMP_CYCLE*iTime)*PUMP_SCALE*pq.xy / (dot(pq,pq)+0.01);
#else
  vec2 pump = vec2(0);
  #define AMP 15.0
  #define SCL -50.0
  float uvy0 = exp(SCL*pow(pq.y,2.0));
  float uvx0 = exp(SCL*pow(uv.x,2.0));
  pump += -AMP*vec2(max(0.0,cos(PUMP_CYCLE*iTime))*PUMP_SCALE*uvx0*uvy0,0);

  float uvy1 = exp(SCL*pow(pq.y,2.0));
  float uvx1 = exp(SCL*pow(1.0 - uv.x,2.0));
  pump += AMP*vec2(max(0.0,cos(PUMP_CYCLE*iTime + 3.1416))*PUMP_SCALE*uvx1*uvy1,0);

  float uvy2 = exp(SCL*pow(pq.x,2.0));
  float uvx2 = exp(SCL*pow(uv.y,2.0));
  pump += -AMP*vec2(0,max(0.0,sin(PUMP_CYCLE*iTime))*PUMP_SCALE*uvx2*uvy2);

  float uvy3 = exp(SCL*pow(pq.x,2.0));
  float uvx3 = exp(SCL*pow(1.0 - uv.y,2.0));
  pump += AMP*vec2(0,max(0.0,sin(PUMP_CYCLE*iTime + 3.1416))*PUMP_SCALE*uvx3*uvy3);
#endif

  gl_FragColor = mix(adv + vec4(VELOCITY_SCALE * (delta_v + pump), offset), init, UPDATE_SMOOTHING);

  if (iMouse.z > 0.0)
  {
    vec4 mouseUV = iMouse / iResolution.xyxy;
    vec2 delta = normz(mouseUV.zw - mouseUV.xy);
    vec2 md = (mouseUV.xy - uv) * vec2(1.0,tx.x/tx.y);
    float amp = exp(max(-12.0,-dot(md,md)/MOUSE_RADIUS));
    gl_FragColor.xy += VELOCITY_SCALE * MOUSE_AMP * clamp(amp * delta,-1.0,1.0);
  }

  if (iFrame == 1)
    gl_FragColor = 1e-6*rand4(gl_FragCoord.xy, iResolution.xy, iFrame);
}
