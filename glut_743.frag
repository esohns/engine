#version 330

// glut_743_common.glsl
#define T(p) texelFetch(iChannel0, ivec2(mod(p,R)), 0)
#define P(p) texture(iChannel0, mod(p,R)/R)
#define C(p) texture(iChannel1, mod(p,R)/R)
#define D(p) texture(iChannel2, mod(p,R)/R)

#define PI 3.14159265
#define dt 1.
#define R iResolution.xy

const vec2 dx = vec2(0, 1);

float
hash11 (float p)
{
  p = fract(p * .1031);
  p *= p + 33.33;
  p *= p + p;
  return fract(p);
}

#define rand_interval 250
#define random_gen(a, b, seed) ((a) + ((b)-(a))*hash11(seed + float(iFrame/rand_interval)))

#define distribution_size 1.4

#define sense_num 2
#define sense_ang (0.15 + 0.13*sin(iTime))
#define sense_dis 200.
#define sense_oscil 0.0
#define oscil_scale 1.
#define oscil_pow 1.
#define sense_force 0.5
#define distance_scale 2.
#define force_scale 0.5
#define trailing 0.
#define acceleration 0.0

#define GS(x) exp(-dot(x,x))
#define GSS(x) exp(-dot(x,x))
#define GS0(x) exp(-length(x))
#define Dir(ang) vec2(cos(ang), sin(ang))
#define Rot(ang) mat2(cos(ang), sin(ang), -sin(ang), cos(ang))
#define loop(i,x) for(int i = 0; i < x; i++)
#define range(i,a,b) for(int i = a; i <= b; i++)

#define Pressure(rho) 0.0*rho.z
#define fluid_rho 0.2

#define PACK(X) ( uint(round(65534.0*clamp(0.5*X.x+0.5, 0., 1.))) + \
           65535u*uint(round(65534.0*clamp(0.5*X.y+0.5, 0., 1.))) )   
               
#define UNPACK(X) (clamp(vec2(X%65535u, X/65535u)/65534.0, 0.,1.)*2.0 - 1.0)              

#define DECODE(X) UNPACK(floatBitsToUint(X))
#define ENCODE(X) uintBitsToFloat(PACK(X))
// glut_743_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform bvec4 iKeyboard; // space
uniform sampler2D iChannel0;

void
main ()
{
  vec4 U;
  vec2 pos = gl_FragCoord.xy;

  ivec2 p = ivec2(pos);

  vec2 X = vec2(0);
  vec2 V = vec2(0);
  float M = 0.;

  range(i, -2, 2) range(j, -2, 2)
  {
    vec2 tpos = pos + vec2(i, j);
    vec4 data = T(tpos);

    vec2 X0 = DECODE(data.x) + tpos;
    vec2 V0 = DECODE(data.y);
    vec2 M0 = data.zw;

    X0 += V0 * dt;

    float K = distribution_size;

    vec4 aabbX = vec4(max(pos - 0.5, X0 - K * 0.5), min(pos + 0.5, X0 + K * 0.5));
    vec2 center = 0.5 * (aabbX.xy + aabbX.zw);
    vec2 size = max(aabbX.zw - aabbX.xy, 0.);

    float m = M0.x * size.x * size.y / (K * K);

    X += center * m;
    V += V0 * m;

    M += m;
  }

  if (M != 0.)
  {
    X /= M;
    V /= M;
  }

  float prevM = M;
  M = mix(M, 0.1, 0.03);
  V = V * prevM / M;

  if (iFrame < 1 || iKeyboard.x) // space
  {
    X = pos;
    vec2 dx0 = (pos - R * 0.3);
    vec2 dx1 = (pos - R * 0.7);
    V = 0.5 * Rot(PI*0.5) * dx0 * GS(dx0/30.) - 0.5 * Rot(PI*0.5) * dx1 * GS(dx1/30.);
    V += 0.2 * Dir(2.*PI*hash11(floor(pos.x/20.) + R.x*floor(pos.y/20.)));
    M = 0.1 + pos.x / R.x * 0.01 + pos.y / R.x * 0.01;
  }

  X = clamp(X - pos, vec2(-0.5), vec2(0.5));
  U = vec4(ENCODE(X), ENCODE(V), M, 0.);

  gl_FragColor = U;
}
