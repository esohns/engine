#version 130

// glut_656_common.glsl
#define pi 3.14159265
#define W(i,n) cexp(vec2(0,FFT_DIR*2.*pi*float(i)/float(n)))
#define R iResolution
#define fradix float(radix)
#define T(c,x,y) texelFetch(c, ivec2(x,y), 0)
#define SUM(expr, ind, len) sum = vec2(0); for(int ind = 0; ind < 64; ind++){ if (ind >= len) break; sum += expr; }
#define FFT_DIR float((iFrame%2)*2-1)
#define FORWARD 1.
#define BACKWARD -1.

vec2 sum;

int x_N0;
int y_N0;
int x_N1;
int y_N1;

float dot2(vec2 x) { return dot(x,x); }

float
factor (float x)
{
  x = floor(x);
  float f = floor(sqrt(x));
  while(fract(x/f)>.5/x) { f--; }
  return x/f;
}

void
setRadix (vec2 R)
{
  x_N0 = int(R.x/factor(R.x));
  y_N0 = int(R.y/factor(R.y));
  x_N1 = int(R.x)/x_N0;
  y_N1 = int(R.y)/y_N0;
}

vec2
cprod (vec2 a, vec2 b)
{
  return mat2(a,-a.y,a.x) * b;
}

vec2
cis (float t)
{
  return cos(t - vec2(0,pi/2.));
}

vec2
cexp (vec2 z)
{
  return exp(z.x)*cis(z.y);
}

int
IHash (int a)
{
	a = (a ^ 61) ^ (a >> 16);
	a = a + (a << 3);
	a = a ^ (a >> 4);
	a = a * 0x27d4eb2d;
	a = a ^ (a >> 15);
	return a;
}

float
Hash (int a)
{
	a = (a ^ 61) ^ (a >> 16);
	a = a + (a << 3);
	a = a ^ (a >> 4);
	a = a * 0x27d4eb2d;
	a = a ^ (a >> 15);
	return float(a) / float(0x7FFFFFFF);
}

vec2
rand2 (int seed)
{
  return vec2(Hash(seed^0x348C5F93),
              Hash(seed^0x8593D5BB));
}

vec2
randn (vec2 randuniform)
{
  vec2 r = randuniform;
  r.x = sqrt(-2.*log(1e-9+abs(r.x)));
  r.y *= 6.28318;
  r = r.x*vec2(cos(r.y),sin(r.y));
  return r;
}
// glut_656_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

vec4
inp (sampler2D ch,int x, int y)
{
  if (FFT_DIR==FORWARD)
  {
    vec2 v = T(ch, x, y).xz;
    return texture(ch, fract((-v + vec2(x, y) + rand2(IHash(x^IHash(y^IHash(iFrame)))))/R.xy));
  }

  return T(ch, x, y);
}

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  setRadix(R);
  O = vec4(0);
  int x = int(I.x);
  int y = int(I.y);  

  int n = (x/x_N1);
  SUM( cprod((inp(iChannel0, (x%x_N1)+i*x_N1, y).xy),W(i*n,x_N0)),i,x_N0 );
  O.xy = (cprod(sum, W((x%x_N1)*n,int(R.x))));

  SUM( cprod((inp(iChannel0, (x%x_N1)+i*x_N1, y).zw),W(i*n,x_N0)),i,x_N0 );
  O.zw = (cprod(sum, W((x%x_N1)*n,int(R.x))));

  gl_FragColor = O;
}
