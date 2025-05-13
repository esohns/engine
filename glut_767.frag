#version 330

// glut_767_common.glsl
#define PI 3.14159265
#define dt 0.5
#define R iResolution.xy

#define T(p) texelFetch(iChannel0, ivec2(mod(p,R)), 0)
#define P(p) texture(iChannel0, mod(p,R)/R)
#define C(p) texture(iChannel1, mod(p,R)/R)

#define GS(x) exp(-dot(x,x))
#define GS0(x) exp(-length(x))
#define CI(x) smoothstep(1.0, 0.9, length(x))
#define Dir(ang) vec2(cos(ang), sin(ang))
#define Rot(ang) mat2(cos(ang), sin(ang), -sin(ang), cos(ang))
#define loop(i,x) for(int i = 0; i < x; i++)
#define range(i,a,b) for(int i = a; i <= b; i++)

#define cooling 1.5

float
MF (vec2 dx)
{
  return -GS(0.75*dx) + 0.13*GS(0.4*dx);
}

float
Ha (vec2 x)
{
  return ((x.x >= 0.)?1.:0.)*((x.y >= 0.)?1.:0.);
}

float
Hb (vec2 x)
{
  return ((x.x > 0.)?1.:0.)*((x.y > 0.)?1.:0.);
}

#define PACK(X) ( uint(round(65534.0*clamp(0.5*X.x+0.5, 0., 1.))) + 65535u*uint(round(65534.0*clamp(0.5*X.y+0.5, 0., 1.))) )
#define UNPACK(X) (clamp(vec2(X%65535u, X/65535u)/65534.0, 0.,1.)*2.0 - 1.0)              
#define DECODE(X) UNPACK(floatBitsToUint(X))
#define ENCODE(X) uintBitsToFloat(PACK(X))
// glut_767_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

vec3
PD (vec2 x, vec2 pos)
{
  return vec3(x, 1.0)*Ha(x - (pos - 0.5))*Hb((pos + 0.5) - x);
}

void
main ()
{
  vec4 U;
  vec2 pos = gl_FragCoord.xy;

  ivec2 p = ivec2(pos);

  vec2 X = vec2(0);
  vec2 V = vec2(0);
  float M = 0.;

  range(i, -1, 1) range(j, -1, 1)
  {
    vec2 tpos = pos + vec2(i,j);
    vec4 data = T(tpos);

    vec2 X0 = DECODE(data.x) + tpos;
    vec2 V0 = DECODE(data.y);
    int M0 = int(data.z);
    int M0H = M0/2;

    X0 += V0*dt;

    vec3 m = (M0 >= 2)
        ?(float(M0H)*PD(X0+vec2(0.5, 0.0), pos) + float(M0 - M0H)*PD(X0-vec2(0.5, 0.0), pos))
        :(float(M0)*PD(X0, pos)); 

    X += m.xy;
    V += V0*m.z;

    M += m.z;
  }

  if(M != 0.)
  {
    X /= M;
    V /= M;
  }

  if(iFrame < 1)
  {
    X = pos;
    V = vec2(0.);
    M = Ha(pos - (R*0.5 - R.x*0.15))*Hb((R*0.5 + R.x*0.15) - pos);
  }

  X = X - pos;
  U = vec4(ENCODE(X), ENCODE(V), M, 0.);

  gl_FragColor = U;
}
