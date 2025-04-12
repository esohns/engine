uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

//#define ANIMATE

const float cellJitter = 0.65;
const float angularSegs = 9.;
#ifdef ANIMATE
const float animbias = 0.5;
const float animscale = 0.4;
#endif

vec2
hash (vec2 p)
{
  p = mod(p, angularSegs);
  return texture( iChannel0, (p+0.5)/200.0, -100.0 ).xy;	
}

vec3
voronoi (vec2 x, out vec2 cpId)
{
  vec2 n = floor(x);
  vec2 f = fract(x);

  vec2 mg, mr;

  float md = 8.0;
  for( int j=-1; j<=1; j++ )
  for( int i=-1; i<=1; i++ )
  {
    vec2 g = vec2(float(i),float(j));
    vec2 o = cellJitter * hash( n + g );
#ifdef ANIMATE
    o = animbias + animscale*sin( iTime*0.5 + 6.2831*o );
#endif
    vec2 r = g + o - f;
    float d = dot(r,r);

    if( d<md )
    {
      md = d;
      mr = r;
      mg = g;
    }
  }

  md = 8.0;
  for( int j=-2; j<=2; j++ )
  for( int i=-2; i<=2; i++ )
  {
    vec2 g = mg + vec2(float(i),float(j));
    vec2 o = cellJitter * hash( n + g );
#ifdef ANIMATE
    o = animbias + animscale*sin( iTime*0.5 + 6.2831*o );
#endif	
    vec2 r = g + o - f;

    if( dot(mr-r,mr-r)>0.000001 )
    {
      float d = dot( 0.5*(mr+r), normalize(r-mr) );

      md = min( md, d );
    }
  }

  cpId = n+mg;

  return vec3( md, mr );
}

float remap (float x) { return x * 0.5 + 0.5; }

mat2
rot2 (float theta)
{
  return mat2(cos(theta), -sin(theta),
              sin(theta), cos(theta));
}

float
atan01 (vec2 p)
{
  return atan(p.y, p.x)/6.28318530718 + 0.5;
}

float rand (float n) { return fract(sin(n) * 43758.5453123); }
float
noise (float p)
{
  float fl = floor(p);
  float fc = fract(p);
  return mix(rand(fl), rand(fl + 1.0), fc);
}

float
lowfreqnoise (float x)
{
  x = mod(x,1.0);
  float res = 10.;
  float ia = floor(x * res);
  float ib = floor(x * res) + 1.;

  // texture lookups have artifacts between segments, probably due to mipmapping.
  //float a = texture(iChannel0, vec2(ia/res, 0.)).r;
  //float b = texture(iChannel0, vec2(ib/res, 0.)).r;
  float a = noise(mod(ia, res));
  float b = noise(mod(ib, res));
    
  float t = fract(x * res);
  return mix(a, b, t) * 2.0 - 1.;
}

vec3
image (vec2 fragCoord)
{
  vec2 uvCenter = iResolution.xy / iResolution.x / 2. + vec2(-0.2, 0.1);
  vec2 uv = fragCoord.xy / iResolution.x - uvCenter;
  float r = length(uv);

  vec2 cyl = vec2(max(0.5,pow(r, 0.1)),
                  atan01(uv));

  cyl.x += 0.015 * abs(lowfreqnoise(cyl.y));
  vec2 freq = vec2(12., angularSegs);

  vec2 cpid;
  vec3 c = voronoi( cyl*freq, cpid );
  cpid = mod(cpid, angularSegs);
  float centerDist = length( c.yz );
  float borderDist = c.x;

  float e0 = mix(.1, .0, pow(r, .1));
  float edge = smoothstep(e0, e0+.0001, borderDist);

  float theta = iTime * 3.14 / 20.;
  vec3 W = vec3(uv, -0.2);
  W.xz = rot2(theta) * W.xz;
  vec3 V = normalize(vec3(0.) - W);
  vec3 normOffset = vec3(noise(cpid.x*7.), noise(cpid.y*13.), noise(27.*(cpid.x-cpid.y))) * 2. - 1.;
  vec3 N = normalize(vec3(0., 0., 1.) + 0.1*normOffset);
  vec3 env = texture(iChannel1, reflect(-V, N)).rgb;
  vec3 F = mix(vec3(1.), vec3(0.5, 0.9, 1.0)*0.4, 1.0-edge);
  vec3 lit = env * F;
  return lit;
}

void
main ()
{
  const int L = 3;
  const float Lf = float(L);
  vec3 sum = vec3(0.);
  for(int i = 0; i < L; i++)
    for(int j = 0; j < L; j++)
    {
      vec2 ofs = vec2(float(i), float(j)) / Lf;
      sum += image(gl_FragCoord.xy + ofs);
    }

  gl_FragColor = vec4(sum/(Lf*Lf),1.0);
}
