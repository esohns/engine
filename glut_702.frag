#version 130

// glut_702_common.glsl
const int PARTICLE_COUNT = 5;

const float PARTICLE_SIZE = .2;

const float PARTICLE_EDGE_SMOOTHING = .003;

const float WALL_THINNESS = 60.;

const float MAX_VELOCITY = 0.004;

const float MAX_VELOCITY_CHANGE = 0.0003;

const float FOCAL_POINT_TENDENCY = .0002;


const float PI = 3.1415926535897932384626433832795;

const int POSITION_ROW = 0;

const int VELOCITY_ROW = 1;

const int COLOR_ROW = 2;

const int LAST_ROW = COLOR_ROW;

const vec4 COLOR_ZERO = vec4(0);

const vec2 VEC2_ZERO = vec2(0);

const vec2 HORIZONTAL_REVERSE = vec2(-1., 1.);

const vec2 VERTICAL_REVERSE = vec2(1., -1.);

vec2
encode (float v)
{
  vec2 c = vec2(0.);

  int signum = (v >= 0.) ? 128 : 0;
  v = abs(v);
  int exponent = 15;
  float limit = 1024.;
  for(int exp = 15; exp > 0; exp--)
    if( v < limit)
    {
      limit /= 2.;
      exponent--;
    }

  float rest;
  if(exponent == 0)
    rest = v / limit / 2.;
  else
    rest = (v - limit)/limit;

  int mantissa = int(rest * 2048.);
  int msb = mantissa / 256;
  int lsb = mantissa - msb * 256;

  c.x = float(signum + exponent * 8 + msb) / 255.;
  c.y = float(lsb) / 255.;

  if(v >= 2048.)
    c.y = 1.;

  return c;
}

float
decode (vec2 c)
{
  float v = 0.;

  int ix = int(c.x*255.);
  int iy = int(c.y*255.);

  int s = (c.x >= 0.5) ? 1 : -1;
  ix = (s > 0) ? ix - 128 : ix;
  int iexp = ix / 8;
  int msb = ix - iexp * 8;

  int norm = (iexp == 0) ? 0 : 2048;
  int mantissa = norm + msb * 256 + iy;
  norm = (iexp == 0) ? 1 : 0;
  float exponent = pow( 2., float(iexp + norm) - 16.);
  v = float( s * mantissa ) * exponent;

  return v;
}

vec4
encode2 (vec2 v)
{
  return vec4(encode(v.x), encode(v.y));
}

vec2
decode2 (vec4 c)
{
  return vec2(decode(c.rg), decode(c.ba));
}

float
randImpl (vec2 co)
{
  return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

#define tex(ch, x, y) texelFetch(ch, ivec2(x, y), 0)

#define rand(seed) randImpl(seed + mod(iTime, PI)) 

#define getParticleRawColor(row, index) tex(iChannel0, index, row) 

#define getParticleVector(row, index) decode2(getParticleRawColor(row, index))

#define getParticlePosition(index) getParticleVector(POSITION_ROW, index)

#define getParticleVelocity(index) getParticleVector(VELOCITY_ROW, index)

#define getParticleColor(index) getParticleRawColor(COLOR_ROW, index)
// glut_702_common.glsl

uniform float iTime;
uniform int iFrame;
uniform bvec4 iKeyboard; // space
uniform sampler2D iChannel0;

vec4
getInitialState (int row, int particle, vec2 seed)
{
  vec4 state;
  if (row == POSITION_ROW)
    state = encode2(VEC2_ZERO);
  else if (row == VELOCITY_ROW)
    state = encode2(VEC2_ZERO);
  else if (row == COLOR_ROW)
  {
    state = vec4(
      rand(seed + .101),
      rand(seed + .102),
      rand(seed + .103),
      1.);
  }
  else
    state = COLOR_ZERO;
  return state;
}

vec4
calculateNewState (int row, int particle, vec2 seed)
{
  vec4 state;
  if (row == POSITION_ROW)
    state = encode2(getParticlePosition(particle) + getParticleVelocity(particle));
  else if (row == VELOCITY_ROW)
  {
    vec2 velocity = getParticleVelocity(particle);
    vec2 coord  = getParticlePosition(particle);
    velocity += vec2(
      rand(seed + .111) * MAX_VELOCITY_CHANGE * 2. - MAX_VELOCITY_CHANGE,
      rand(seed + .112) * MAX_VELOCITY_CHANGE * 2. - MAX_VELOCITY_CHANGE
    );
    vec2 focalPoint = coord;
    velocity -= focalPoint * FOCAL_POINT_TENDENCY;
    velocity = vec2(
      clamp(velocity.x, -MAX_VELOCITY, MAX_VELOCITY),
      clamp(velocity.y, -MAX_VELOCITY, MAX_VELOCITY)
    );
    vec2 prediction = coord + velocity;
    if ((prediction.x < -.5) || (prediction.x > .5))
      velocity *= HORIZONTAL_REVERSE;
    if ((prediction.y < -.5) || (prediction.y > .5))
      velocity *= VERTICAL_REVERSE;
    state = encode2(velocity);
  }
  else if (row == COLOR_ROW)
    state = getParticleColor(particle);
  else
    state = COLOR_ZERO;
  return state;
}

vec4
getEncodedState (int row, int particle, vec2 seed)
{
  vec4 state;
  if ((iFrame == 0) || iKeyboard.x) // space
    state = getInitialState (row, particle, seed);
  else
    state = calculateNewState (row, particle, seed);
  return state;
}

void
main ()
{
  int particle = int(gl_FragCoord.x);
  int row = int(gl_FragCoord.y);
  if ((row <= LAST_ROW) && (particle < PARTICLE_COUNT))
    gl_FragColor = getEncodedState(row, particle, gl_FragCoord.xy);
}
