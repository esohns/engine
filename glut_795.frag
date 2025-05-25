#version 420

// glut_795_common.glsl
#define HI_QUALITY

ivec2
imod (ivec2 x, ivec2 p)
{
  return x - p * (x/p);
}

#define MOUSE_RADIUS 0.0002

vec2
mousePath (float T)
{
  return vec2(0.5 + 0.3 * cos(T) / (1.0 + sin(T)*sin(T)), 0.5+0.5*cos(T)*sin(T) / (1.0 + sin(T)*sin(T)));
}
#define MOUSE_VEC vec4(mousePath(iTime),mousePath(iTime-2.0))

vec2
normz (vec2 x)
{
  return x == vec2(0) ? vec2(0) : normalize(x);
}

uint
packUnorm4x8 (vec4 x)
{
  x = round(clamp(x,0.0, 1.0) * 255.0);
  uvec4 r = uvec4(x) & uvec4(0xFF);
  return r.x << 24 | r.y << 16 | r.z << 8 | r.w;
}

vec4
unpackUnorm4x8 (uint x)
{
  uvec4 r = (uvec4(x) >> uvec4(24, 16, 8, 0)) & uvec4(0xFF);
  vec4 fmag = vec4(r) / 255.0;
  return fmag;
}

#define unpack(x) unpackUnorm4x8(floatBitsToUint(x))
#define pack(x) uintBitsToFloat(packUnorm4x8(x))

#define unpack16(x) unpackSnorm2x16(floatBitsToUint(x))
#define pack16(x) uintBitsToFloat(packSnorm2x16(x))

#define WAVELENGTHS 16

#define SQRT_TWO_PI 2.50662827463
float
gaussian (float x, float mu, float sig)
{
  return (1.0/(SQRT_TWO_PI*sig)) * exp(-0.5 * pow((x-mu)/sig,2.0));
}

#define SS 1.04
#define GSS 1.0
vec3
sampleWeights (float i)
{
  return vec3(gaussian(i,2.666,SS),gaussian(i,7.5,GSS*SS),gaussian(i,12.333,SS));
}

#define LOG_SCALE0 0.90
#define LOG_SCALE1 0.09
#define LOG_SCALE2 5.28

void
upsample (vec3 upsamp, inout float spectrum[WAVELENGTHS])
{
  vec3 col = vec3(0);
  vec3 dw = vec3(0);

  for(int i = 0; i < WAVELENGTHS; i++)
  {
    vec3 sw = sampleWeights(float(i));
    dw += sw;
  }

  for(int i = 0; i < WAVELENGTHS; i++)
  {
    vec3 sw = sampleWeights(float(i));
    spectrum[i] = -LOG_SCALE1*log(LOG_SCALE2*LOG_SCALE0*dot(sw/dw,1.0-upsamp));
  }
}

vec3
downsample (float spectrum[WAVELENGTHS])
{
  vec3 col = vec3(0);
  vec3 dw = vec3(0);
  for(int i = 0; i < WAVELENGTHS; i++)
  {
    vec3 sw = sampleWeights(float(i));
    dw += sw;
  }

  for(int i = 0; i < WAVELENGTHS; i++)
  {
    vec3 sw = sampleWeights(float(i));
    col += -(sw/dw)*spectrum[i];
  }

  return (1.0-exp(col/LOG_SCALE1))/LOG_SCALE0;
}

void
vec16toArray (inout float spectrum[WAVELENGTHS], vec4 spec0, vec4 spec1, vec4 spec2, vec4 spec3)
{
  spectrum[0] = spec0.x;
  spectrum[1] = spec0.y;
  spectrum[2] = spec0.z;
  spectrum[3] = spec0.w;
  spectrum[4] = spec1.x;
  spectrum[5] = spec1.y;
  spectrum[6] = spec1.z;
  spectrum[7] = spec1.w;
  spectrum[8] = spec2.x;
  spectrum[9] = spec2.y;
  spectrum[10] = spec2.z;
  spectrum[11] = spec2.w;
  spectrum[12] = spec3.x;
  spectrum[13] = spec3.y;
  spectrum[14] = spec3.z;
  spectrum[15] = spec3.w;
}

void
unpackSpectrum (vec4 packed_spectrum, inout float spectrum[WAVELENGTHS])
{
  vec4 unpack0 = unpack(packed_spectrum.x);
  vec4 unpack1 = unpack(packed_spectrum.y);
  vec4 unpack2 = unpack(packed_spectrum.z);
  vec4 unpack3 = unpack(packed_spectrum.w);

  vec16toArray(spectrum, unpack0, unpack1, unpack2, unpack3);
}

vec3
colorFromSpectrum (vec4 packed_spectrum)
{
  float spectrum[WAVELENGTHS];
  unpackSpectrum(packed_spectrum, spectrum);
  return downsample(spectrum);
}

vec4
packSpectrum (float spectrum[WAVELENGTHS])
{
  return vec4(
    pack(vec4(spectrum[0],spectrum[1],spectrum[2],spectrum[3])),
    pack(vec4(spectrum[4],spectrum[5],spectrum[6],spectrum[7])),
    pack(vec4(spectrum[8],spectrum[9],spectrum[10],spectrum[11])),
    pack(vec4(spectrum[12],spectrum[13],spectrum[14],spectrum[15]))
  );
}

vec4
getPackedSpectrum (sampler2D channel, vec2 fragCoord)
{
  return texelFetch(channel, ivec2(floor(fragCoord)), 0);
}

#define RESMOD(d,r) ivec2(imod((ivec2(r) + ivec2(imod(ivec2(d),ivec2(r)))), ivec2(r)))

vec4
sample_biquadratic_exact (sampler2D channel, vec2 iResolution, vec2 uv)
{
  ivec2 LODres = textureSize(channel, 0);
  vec2 q = fract(uv * vec2(LODres));
  ivec2 t = ivec2(floor(uv * vec2(LODres)));
  ivec3 e = ivec3(-1, 0, 1);
  vec4 s00 = texelFetch(channel, RESMOD(t + e.xx, LODres), 0);
  vec4 s01 = texelFetch(channel, RESMOD(t + e.xy, LODres), 0);
  vec4 s02 = texelFetch(channel, RESMOD(t + e.xz, LODres), 0);
  vec4 s12 = texelFetch(channel, RESMOD(t + e.yz, LODres), 0);
  vec4 s11 = texelFetch(channel, RESMOD(t + e.yy, LODres), 0);
  vec4 s10 = texelFetch(channel, RESMOD(t + e.yx, LODres), 0);
  vec4 s20 = texelFetch(channel, RESMOD(t + e.zx, LODres), 0);
  vec4 s21 = texelFetch(channel, RESMOD(t + e.zy, LODres), 0);
  vec4 s22 = texelFetch(channel, RESMOD(t + e.zz, LODres), 0);
  vec2 q0 = (q+1.0)/2.0;
  vec2 q1 = q/2.0;
  vec4 x0 = mix(mix(s00, s01, q0.y), mix(s01, s02, q1.y), q.y);
  vec4 x1 = mix(mix(s10, s11, q0.y), mix(s11, s12, q1.y), q.y);
  vec4 x2 = mix(mix(s20, s21, q0.y), mix(s21, s22, q1.y), q.y);
  return mix(mix(x0, x1, q0.x), mix(x1, x2, q1.x), q.x);
}

#define PM(d) fract(d)

vec4
sample_biquadratic_exact_lod (sampler2D channel, vec2 uv, int lod)
{
  ivec2 LODres = textureSize(channel, lod);
  vec2 FC = uv * vec2(LODres);
  vec2 q = fract(FC);
  ivec2 t = ivec2(floor(FC));
  ivec3 e = ivec3(-1, 0, 1);
  vec4 s00 = texelFetch(channel, RESMOD(t + e.xx, LODres), lod);
  vec4 s01 = texelFetch(channel, RESMOD(t + e.xy, LODres), lod);
  vec4 s02 = texelFetch(channel, RESMOD(t + e.xz, LODres), lod);
  vec4 s12 = texelFetch(channel, RESMOD(t + e.yz, LODres), lod);
  vec4 s11 = texelFetch(channel, RESMOD(t + e.yy, LODres), lod);
  vec4 s10 = texelFetch(channel, RESMOD(t + e.yx, LODres), lod);
  vec4 s20 = texelFetch(channel, RESMOD(t + e.zx, LODres), lod);
  vec4 s21 = texelFetch(channel, RESMOD(t + e.zy, LODres), lod);
  vec4 s22 = texelFetch(channel, RESMOD(t + e.zz, LODres), lod);
  vec2 q0 = (q+1.0)/2.0;
  vec2 q1 = q/2.0;
  vec4 x0 = mix(mix(s00, s01, q0.y), mix(s01, s02, q1.y), q.y);
  vec4 x1 = mix(mix(s10, s11, q0.y), mix(s11, s12, q1.y), q.y);
  vec4 x2 = mix(mix(s20, s21, q0.y), mix(s21, s22, q1.y), q.y);
  return mix(mix(x0, x1, q0.x), mix(x1, x2, q1.x), q.x);
}

vec2
sample_biquadratic_gradient (sampler2D channel, vec2 res, vec2 uv, float mip)
{
  vec2 q = fract(uv * res);
  float sc = pow(2.0,mip);
  vec2 c = sc * (q*(q - 1.0) + 0.5) / res;
  vec2 w0 = uv - c;
  vec2 w1 = uv + c;
  vec2 cc = sc * 0.5 / res;
  vec2 ww0 = uv - cc;
  vec2 ww1 = uv + cc;
  float nx0 = textureLod(channel, fract(vec2(ww1.x, w0.y)), mip).r - textureLod(channel, fract(vec2(ww0.x, w0.y)), mip).r;
  float nx1 = textureLod(channel, fract(vec2(ww1.x, w1.y)), mip).r - textureLod(channel, fract(vec2(ww0.x, w1.y)), mip).r;

  float ny0 = textureLod(channel, fract(vec2(w0.x, ww1.y)), mip).r - textureLod(channel, fract(vec2(w0.x, ww0.y)), mip).r;
  float ny1 = textureLod(channel, fract(vec2(w1.x, ww1.y)), mip).r - textureLod(channel, fract(vec2(w1.x, ww0.y)), mip).r;
  return vec2(nx0 + nx1, ny0 + ny1) / 2.0;
}

#define SAMPLE_CHAN x

vec2
sample_biquadratic_gradient_lod (sampler2D channel, vec2 uv, int lod)
{
  ivec2 LODres = textureSize(channel, lod);
  vec2 FC = uv * (vec2(LODres));
  vec2 q = fract(FC);
  ivec2 t = ivec2(floor(FC));
  ivec3 e = ivec3(-1, 0, 1);
  float s_sw = texelFetch(channel, RESMOD(t + e.xx, LODres), lod).SAMPLE_CHAN;
  float s_w  = texelFetch(channel, RESMOD(t + e.xy, LODres), lod).SAMPLE_CHAN;
  float s_nw = texelFetch(channel, RESMOD(t + e.xz, LODres), lod).SAMPLE_CHAN;
  float s_n  = texelFetch(channel, RESMOD(t + e.yz, LODres), lod).SAMPLE_CHAN;
  float s_c  = texelFetch(channel, RESMOD(t + e.yy, LODres), lod).SAMPLE_CHAN;
  float s_s  = texelFetch(channel, RESMOD(t + e.yx, LODres), lod).SAMPLE_CHAN;
  float s_se = texelFetch(channel, RESMOD(t + e.zx, LODres), lod).SAMPLE_CHAN;
  float s_e  = texelFetch(channel, RESMOD(t + e.zy, LODres), lod).SAMPLE_CHAN;
  float s_ne = texelFetch(channel, RESMOD(t + e.zz, LODres), lod).SAMPLE_CHAN;

  vec2 p = q * (q-1.0) + 0.5;
  vec2 nq  = 1.0 - q;
  vec2 nq2 = 1.0 - 2.0*q;
  vec2 np  = 1.0 - p;
  vec2 ppq = p+q-0.5;
  vec2 pmq = p-q+0.5;

  vec2 W_c  = vec2(2.0 * np.y * nq2.x, 2.0 * np.x * nq2.y);
  vec2 W_n  = vec2(ppq.y * nq2.x, 2.0 * q.y * np.x);
  vec2 W_s  = vec2(pmq.y * nq2.x, -2.0 * nq.y * np.x);
  vec2 W_e  = vec2(2.0 * q.x * np.y, ppq.x * nq2.y);
  vec2 W_w  = vec2(-2.0 * nq.x * np.y, pmq.x * nq2.y);
  vec2 W_nw = vec2(-nq.x * ppq.y, pmq.x * q.y);
  vec2 W_se = vec2(q.x * pmq.y, -ppq.x * nq.y);
  vec2 W_ne = vec2(q.x * ppq.y, ppq.x * q.y);
  vec2 W_sw = vec2(-nq.x * pmq.y, -pmq.x * nq.y);

  vec2 sum = W_c * s_c + W_n * s_n + W_s * s_s + W_e * s_e + W_w * s_w
          + W_nw * s_nw + W_se * s_se + W_ne * s_ne + W_sw * s_sw;

  return sum / 2.0;
}

vec4
biquad (vec2 q, vec4 s00, vec4 s01, vec4 s02, vec4 s10, vec4 s11, vec4 s12, vec4 s20, vec4 s21, vec4 s22)
{
  vec2 q0 = (q+1.0)/2.0;
  vec2 q1 = q/2.0;
  vec4 x0 = mix(mix(s00, s01, q0.y), mix(s01, s02, q1.y), q.y);
  vec4 x1 = mix(mix(s10, s11, q0.y), mix(s11, s12, q1.y), q.y);
  vec4 x2 = mix(mix(s20, s21, q0.y), mix(s21, s22, q1.y), q.y);
  return mix(mix(x0, x1, q0.x), mix(x1, x2, q1.x), q.x);
}

void
sample_biquadratic_exact_unpack (sampler2D channel, vec2 uv, vec2 iResolution, out float spectrum[WAVELENGTHS])
{
  vec2 q = fract(uv * iResolution);
  ivec2 t = ivec2(floor(uv * iResolution));
  ivec3 e = ivec3(-1, 0, 1);

  vec4 p00 = texelFetch(channel, RESMOD(t + e.xx, iResolution), 0);
  vec4 p01 = texelFetch(channel, RESMOD(t + e.xy, iResolution), 0);
  vec4 p02 = texelFetch(channel, RESMOD(t + e.xz, iResolution), 0);
  vec4 p12 = texelFetch(channel, RESMOD(t + e.yz, iResolution), 0);
  vec4 p11 = texelFetch(channel, RESMOD(t + e.yy, iResolution), 0);
  vec4 p10 = texelFetch(channel, RESMOD(t + e.yx, iResolution), 0);
  vec4 p20 = texelFetch(channel, RESMOD(t + e.zx, iResolution), 0);
  vec4 p21 = texelFetch(channel, RESMOD(t + e.zy, iResolution), 0);
  vec4 p22 = texelFetch(channel, RESMOD(t + e.zz, iResolution), 0);

  vec4 spec0 = biquad(q,
                      unpack(p00.x), unpack(p01.x), unpack(p02.x),
                      unpack(p10.x), unpack(p11.x), unpack(p11.x),
                      unpack(p20.x), unpack(p21.x), unpack(p21.x));

  vec4 spec1 = biquad(q,
                      unpack(p00.y), unpack(p01.y), unpack(p02.y),
                      unpack(p10.y), unpack(p11.y), unpack(p11.y),
                      unpack(p20.y), unpack(p21.y), unpack(p21.y));

  vec4 spec2 = biquad(q,
                      unpack(p00.z), unpack(p01.z), unpack(p02.z),
                      unpack(p10.z), unpack(p11.z), unpack(p11.z),
                      unpack(p20.z), unpack(p21.z), unpack(p21.z));

  vec4 spec3 = biquad(q,
                      unpack(p00.w), unpack(p01.w), unpack(p02.w),
                      unpack(p10.w), unpack(p11.w), unpack(p11.w),
                      unpack(p20.w), unpack(p21.w), unpack(p21.w));

  vec16toArray(spectrum, spec0, spec1, spec2, spec3);
}

vec4
bilin (vec2 q, vec4 p11, vec4 p12, vec4 p21, vec4 p22)
{
  vec4 x1 = mix(p11, p21, q.x);
  vec4 x2 = mix(p12, p22, q.x);
  return mix(x1, x2, q.y);
}

void
sample_bilinear_unpack (sampler2D channel, vec2 uv, vec2 iResolution, out float spectrum[WAVELENGTHS])
{
  vec2 p = uv * iResolution - 0.5;
  vec2 q = fract(p);
  vec2 tx = 1.0 / iResolution;

  ivec2 t = ivec2(floor(p));

  ivec3 e = ivec3(-1, 0, 1);

  vec4 p12 = texelFetch(channel, RESMOD(t + e.yz, iResolution), 0);
  vec4 p11 = texelFetch(channel, RESMOD(t + e.yy, iResolution), 0);
  vec4 p21 = texelFetch(channel, RESMOD(t + e.zy, iResolution), 0);
  vec4 p22 = texelFetch(channel, RESMOD(t + e.zz, iResolution), 0);

  vec4 spec0 = bilin(q,
                     unpack(p11.x), unpack(p12.x),
                     unpack(p21.x), unpack(p22.x));

  vec4 spec1 = bilin(q,
                     unpack(p11.y), unpack(p12.y),
                     unpack(p21.y), unpack(p22.y));

  vec4 spec2 = bilin(q,
                     unpack(p11.z), unpack(p12.z),
                     unpack(p21.z), unpack(p22.z));

  vec4 spec3 = bilin(q,
                     unpack(p11.w), unpack(p12.w),
                     unpack(p21.w), unpack(p22.w));

  vec16toArray(spectrum, spec0, spec1, spec2, spec3);
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
rand4 (vec2 fragCoord, vec2 iResolution, int iFrame )
{
  uvec2 p = uvec2(fragCoord);
  uvec2 r = uvec2(iResolution);
  uint c = p.x + r.x*p.y + r.x*r.y*uint(iFrame);
  return vec4(hash3(c),hash1(c + 75132895U));
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
// glut_795_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform bvec4 iKeyboard; // space
uniform sampler2D iChannel0;
uniform sampler2D iChannel2;

#define K_FUNC_VEL 1.0
#define K_FUNC_OFF 0.0
#define OFFSET_SCALE_OFF 1.0
#define OFFSET_SCALE_VEL 1.0
#define OFFSET_TEMPORAL_SMOOTH 0.0
#define ADVECT_TIMESTEP 1.0
#define DIVERGENCE_MIN -0.02
#define VELOCITY_FEED 1.0
#define VISCOSITY 0.0015
#define DAMPING 0.005
#define MOUSE_AMP 1.0

bool reset ()
{
  return iKeyboard.x; // space
}

#ifdef HI_QUALITY
#define E(d) sample_biquadratic_exact_lod(iChannel0, uv + texel * (d+0.), 0)
#else
#define E(d) textureLod(iChannel0, uv + texel * (d+0.), 0.0)
#endif
#define K_F(k) (K_FUNC_VEL*k.xy+K_FUNC_OFF*k.zw)
vec4
advect (vec2 uv, vec2 v, float ts)
{
  vec2 texel = 1.0/iResolution.xy;
  uv -= v * texel;
  vec4 k1 = E(0.);
  vec4 k2 = E(-0.5*K_F(k1)*ts);
  vec4 k3 = E(-0.5*K_F(k2)*ts);
  vec4 k4 = E(-K_F(k3)*ts);
  return 1.0*(k1+2.0*k2+2.0*k3+k4)/6.0;
}

#ifdef HI_QUALITY
#define C(x,y) sample_biquadratic_exact_lod(iChannel0, t*(U+float(1<<s)*vec2(x,y)), s)
#else
#define C(x,y) textureLod(iChannel0, t*(U+float(1<<s)*vec2(x,y)), float(s))
#endif
#define D() sample_biquadratic_gradient_lod(iChannel2, uv, s)
void
main ()
{
  vec4 O = vec4(0.);
  vec2 U = gl_FragCoord.xy;

  vec2 t = 1./iResolution.xy;
  vec2 uv = U*t;

  int s = int(max(log2(iResolution.x),log2(iResolution.y)));
  vec2 mdiff = vec2(0);
  vec4 sample_prev = textureLod(iChannel0, uv, 0.0);
  vec2 offset_prev = sample_prev.zw;
  const float L0 = -3.0;
  const float L1 = 0.5;
  const float L2 = 0.25;
  vec4 lapl = vec4(0);
  for (; s >= 0; s--)
  {
    vec4 c_n = C(0,1);
    vec4 c_s = C(0,-1);
    vec4 c_e = C(1,0);
    vec4 c_w = C(-1,0);
    vec4 c_ne = C(1,1);
    vec4 c_se = C(1,-1);
    vec4 c_nw = C(-1,1);
    vec4 c_sw = C(-1,-1);
    vec4 c = C(0,0);
    lapl += pow(float(s+1),1.0) * (L0 * c + L1 * (c_n+c_e+c_w+c_s) + L2 * (c_ne+c_se+c_nw+c_sw));
    O.xy += pow(float(s+1),1.0) * (2.0 * vec2(c_n.x + c_s.x, c_e.y + c_w.y) -4.0 * c.xy + (c_se - c_ne - c_sw + c_nw).yx);
    mdiff += pow(float(s+1),1.0) * D();
  }
  vec2 offset = OFFSET_SCALE_OFF*O.xy + OFFSET_SCALE_VEL*sample_prev.xy;
  vec2 new_offset = mix(offset,offset_prev,OFFSET_TEMPORAL_SMOOTH);
  O = vec4(advect(uv,new_offset,ADVECT_TIMESTEP).xy 
          + DIVERGENCE_MIN * mdiff 
          + VELOCITY_FEED * t.xx * new_offset 
          + VISCOSITY * lapl.xy 
          - DAMPING * sample_prev.xy
          , new_offset
  );

  vec4 mouseUV;
  if (iMouse.z > 0.0)
    mouseUV = iMouse / iResolution.xyxy;
  else
    mouseUV = MOUSE_VEC;
  vec2 delta = -normz(mouseUV.xy - abs(mouseUV.zw));
  vec2 md = (mouseUV.xy - uv) * vec2(1.0,t.x/t.y);
  float amp = clamp(exp(max(-24.0,-dot(md,md)/MOUSE_RADIUS)),0.0,1.0);
  O += vec4(MOUSE_AMP * delta * amp,0,0);

  if(iFrame < 1 || reset())
    O = vec4(1e-4);

  gl_FragColor = O;
}
