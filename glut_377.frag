//precision highp float;
#version 330

uniform vec2 u_resolution;
uniform float u_time;
uniform float u_mouse;

#define PI 3.141592653589793

#define cx_add(a, b) vec2(a.x + b.x, a.y + b.y)
#define cx_sub(a, b) vec2(a.x - b.x, a.y - b.y)
#define cx_mul(a, b) vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)
#define cx_div(a, b) vec2(((a.x * b.x + a.y * b.y) / (b.x * b.x + b.y * b.y)), ((a.y * b.x - a.x * b.y) / (b.x * b.x + b.y * b.y)))
#define cx_modulus(a) length(a)
#define cx_conj(a) vec2(a.x, -a.y)
#define cx_arg(a) atan2(a.y, a.x)
#define cx_sin(a) vec2(sin(a.x) * cosh(a.y), cos(a.x) * sinh(a.y))
#define cx_cos(a) vec2(cos(a.x) * cosh(a.y), -sin(a.x) * sinh(a.y))
#define hsv2rgb(v) abs(fract(v + vec3(3, 2, 1) / 3.) - .5) * 6. - 1.

float
hypot (vec2 z)
{
  float x = abs(z.x);
  float y = abs(z.y);
  float t = min(x, y);
  x = max(x, y);
  t = t / x;
  return x * sqrt(1.0 + t * t);

  // This conditional seems unnecessary on the non-cpu version
  //return (z.x == 0.0 && z.y == 0.0) ? 0.0 : x * sqrt(1.0 + t * t);
}

float
cosh (float val)
{
  float tmp = exp(val);
  float cosH = (tmp + 1.0 / tmp) / 2.0;
  return cosH;
}

float
sinh (float val)
{
  float tmp = exp(val);
  float sinH = (tmp - 1.0 / tmp) / 2.0;
  return sinH;
} 

float
tanh (float val)
{
  float tmp = exp(val);
  float tanH = (tmp - 1.0 / tmp) / (tmp + 1.0 / tmp);
  return tanH;
}

vec2
cx_tan (vec2 a)
{
  return cx_div (cx_sin (a), cx_cos (a));
}

vec2
cx_pow (vec2 a, vec2 b)
{
  float aarg = atan(a.y, a.x);
  float amod = hypot(a);

  float theta = log(amod) * b.y + aarg * b.x;

  return vec2 (cos(theta), sin(theta)) * pow (amod, b.x) * exp (-aarg * b.y);
}

vec2
cx_exp (vec2 z)
{
  return vec2(cos(z.y), sin(z.y)) * exp(z.x);
}

vec2
as_polar (vec2 z)
{
  return vec2 (length(z), atan (z.y, z.x));
}

vec2
cx_log (vec2 a)
{
  vec2 polar = as_polar (a);
  float rpart = polar.x;
  float ipart = polar.y;
  if (ipart > PI)
    ipart = ipart - (2.0 * PI);

  return vec2(log(rpart), ipart);
}

///////////////////////////////////////////////
// para generar paletas de colores cosenoidales
///////////////////////////////////////////////

vec3
palette (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b * cos(0.38*2.*PI * (c * t + d));
}

float
map (float value, float min1, float max1, float min2, float max2)
{
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

/////////////////////////////////////////
// iteracion de la funcion sin(z)*(1 + i)
/////////////////////////////////////////

vec2
f (vec2 z)
{
  vec2 a = vec2(1.0,0.0) + vec2(0.0,1.0);
  vec2 par = z + vec2(u_time*0.001,u_time*0.001);
  vec2 b = cx_sin(par);
  return cx_mul(a,b);
}

vec2
iter_sin (vec2 z)
{
  for (float i=1.0;i < 5.;i++)
    z = f(z);

  return z;
}

vec2
g (vec2 f)
{
  return ceil (cx_log (f)) - log (abs (f)); 
}

vec3
pal1 (float par)
{
  return palette (par, vec3 (0.5,0.5,0.5), vec3 (0.5,0.5,0.5), vec3 (1.0,1.0,1.0), vec3 (0.0,0.33,0.67));
}

vec3
pal2 (float par)
{
  return palette (par, vec3 (0.5,0.5,0.5), vec3 (0.5,0.5,0.5), vec3 (1.0,1.0,1.0), vec3 (0.00, 0.10, 0.20));
}

vec3 pal3(float par)
{
  return palette(par,vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,1.0,1.0),vec3(0.30, 0.20, 0.20));
}

vec3
pal4 (float par)
{
  return palette(par,vec3(0.5, 0.5, 0.5),vec3(0.5, 0.5, 0.5),vec3(1.0, 1.0, 0.5),vec3(0.80, 0.90, 0.30));
}

vec3
pal5 (float par)
{
  return palette(par,vec3(0.5, 0.5, 0.5),vec3(0.5, 0.5, 0.5),vec3(1.0, 0.7, 0.4),vec3(0.00, 0.15, 0.20));
}

vec3
pal6 (float par)
{
  return palette(par,vec3(0.5, 0.5, 0.5),vec3(0.5, 0.5, 0.5),vec3(2.0, 1.0, 0.0),vec3(0.50, 0.20, 0.25));
}

vec3
pal7 (float par)
{
  return palette(par,vec3(0.8, 0.5, 0.4),vec3(0.2, 0.4, 0.2),vec3(2.0, 1.0, 1.0), vec3(0.00, 0.25, 0.25));
}

mat2
rotate2d (float _angle)
{
  return mat2 (cos (_angle), -sin (_angle),
               sin (_angle),  cos (_angle));
}

void
main () 
{
  vec2 st = (gl_FragCoord.xy - 1.0 * u_resolution.xy) / min (u_resolution.y, u_resolution.x);
  st *= 3.0;
  //st.y *= 0.5 * sin (u_time * 0.01);
  st.x += 0.005 * u_time;
  //st *= rotate2d (u_time * 0.005);
  vec2 z = st;
  vec2 f = g (iter_sin (z)); 
  vec2 zpolar = as_polar (f);
  float phase = f.y;
  float mag   = f.x;
  phase = map (phase,-PI,PI,0.0,1.0);
  vec3 col1 = pal2 (phase);
  vec3 col2 = pal2 (mag);
  vec3 col = mix (col1, col2, 0.5);
  gl_FragColor = vec4 (col, 1.0);
}
