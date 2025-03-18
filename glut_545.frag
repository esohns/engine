#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

vec3
hsv2rgb (vec3 c)
{
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3
rgb2hsv (vec3 c)
{
  vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
  vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
  vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

  float d = q.x - min(q.w, q.y);
  float e = 1.0e-10;
  return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

int
xorshift (int value)
{
  value ^= value << 13;
  value ^= value >> 17;
  value ^= value << 5;
  return value;
}

int
nextInt (inout int seed)
{
  seed = xorshift(seed);
  return seed;
}

float
nextFloat (inout int seed)
{
  seed = xorshift(seed);
  return abs(fract(float(seed) / 31416.592898653));
}

float
nextFloat (inout int seed, float max)
{
  return nextFloat(seed) * max;
}

vec3
nearby (vec2 fragCoord, vec2 offset)
{
  vec2 uv = (fragCoord + offset)/iResolution.xy;
  return vec3(texture(iChannel0, uv));
}

float
hueDiff (float a, float b)
{
  float diff=fract(a)-fract(b);
  if(abs(diff)<0.5)
    return diff;
  else
    return diff - 1.*sign(diff);
}

float
checkfunction (float near_hue, float prev_hue)
{
  return (sign(hueDiff(near_hue,prev_hue)) + 4.*hueDiff(near_hue,prev_hue))/abs(hueDiff(near_hue,prev_hue));
}

const float threshold = 0.4;
#define PI 3.14159
#define initial_cols (vec3(atan((uv.x-0.5)/(uv.y-0.5))/PI,1.,2.-2.*((uv.x-0.5)*(uv.x-0.5)+(uv.y-0.5)*(uv.y-0.5))))

void
main ()
{
  int rngSeed = int(gl_FragCoord.x) + int(gl_FragCoord.y) * int(iResolution.x) + int(iTime * 1000.0);

  float mouseDown = iMouse.z;
  vec2 uvMouse = vec2(iMouse)/iResolution.xy;

  vec2 uv = gl_FragCoord.xy/iResolution.xy;
  vec3 previous = rgb2hsv(vec3(texture(iChannel0, uv)));
  vec3 next = previous;

  if(next[2]>0.05)
    next[2]*=0.99;

  if(nextFloat(rngSeed, 1.0) > threshold)
  {
    vec3 up = rgb2hsv(nearby(gl_FragCoord.xy, vec2(0.0, -1.0)));
    vec3 down = rgb2hsv(nearby(gl_FragCoord.xy, vec2(0.0, 1.0)));
    vec3 left = rgb2hsv(nearby(gl_FragCoord.xy, vec2(-1.0, 0.0)));
    vec3 right = rgb2hsv(nearby(gl_FragCoord.xy, vec2(1.0, 0.0)));

    float upweight = up[2]*up[2]*up[1]*up[1]*(abs(hueDiff(up[0],previous[0]))-0.3)*(0.7-abs(hueDiff(up[0],previous[0])))*(0.1+0.6*up[2])*checkfunction(up[0],previous[0]);
    float downweight = down[2]*down[2]*down[1]*down[1]*(abs(hueDiff(down[0],previous[0]))-0.3)*(0.7-abs(hueDiff(down[0],previous[0])))*(0.1+0.6*down[2])*checkfunction(down[0],previous[0]);
    float leftweight = left[2]*left[2]*left[1]*left[1]*(abs(hueDiff(left[0],previous[0]))-0.3)*(0.7-abs(hueDiff(left[0],previous[0])))*(0.1+0.6*left[2])*checkfunction(left[0],previous[0]);
    float rightweight = right[2]*right[2]*right[1]*right[1]*(abs(hueDiff(right[0],previous[0]))-0.3)*(0.7-abs(hueDiff(right[0],previous[0])))*(0.1+0.6*right[2])*checkfunction(right[0],previous[0]);
    float _max = max(max(upweight,downweight),max(leftweight,rightweight));

    if(_max>((previous[2]*previous[1]))*3.0)
    {
      if( _max == upweight)
        next = up;

      if (_max == downweight)
        next = down;

      if (_max == leftweight)
        next = left;

      if (_max == rightweight)
        next = right;
      if(next[2]<1.)
        next[2]+=0.5+0.5*(abs(hueDiff(next[0],previous[0]))-1.);
      if(next[1]<1.)
        next[1]+=0.0008; 
    }
    else
    {
      vec3 up_rgb = (nearby(gl_FragCoord.xy, vec2(0.0, -1.0)));
      vec3 down_rgb = (nearby(gl_FragCoord.xy, vec2(0.0, 1.0)));
      vec3 left_rgb = (nearby(gl_FragCoord.xy, vec2(-1.0, 0.0)));
      vec3 right_rgb = (nearby(gl_FragCoord.xy, vec2(1.0, 0.0)));
      vec3 prev_rgb = vec3(texture(iChannel0, uv));
      next = rgb2hsv(prev_rgb*0.7 + 0.3*(up_rgb+down_rgb+left_rgb+right_rgb)/4.);
    }
  }

  if(length(uvMouse-uv)<.13 && mouseDown > 0.5) 
    next = vec3(0.,1.,0.);//vec3(nextFloat(rngSeed, 1.0),nextFloat(rngSeed, 1.0),nextFloat(rngSeed, 1.0));
  if((iFrame == 1)&&length(uv-0.5)>0.2)
  {
    next = initial_cols;
    previous = next;
  }

  gl_FragColor = vec4(hsv2rgb(next),1.0);
}
