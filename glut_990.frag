#version 130

// glut_990_common.glsl
const float dissipation 	= 0.95;

const float ballRadius		= 0.06;
const float fogHeigth		= ballRadius * 2.;
const int	nbSlice			= 32;
const float fogSlice		= fogHeigth / float(nbSlice);
const int	nbSphere 		= 3;
const float ShadowDensity 	= 25.;
const float FogDensity 		= 20.;

const float tau =  radians(360.);

vec2
rotate (float angle, float radius)
{
  return vec2(cos(angle),-sin(angle)) * radius;
}

bool
floorIntersect (vec3 ro, vec3 rd, float floorHeight, out float t)
{
  ro.y -= floorHeight;
  if(rd.y < -0.01)
  {
    t = ro.y / - rd.y;
    return true;
  }

  return false;
} 

vec2
sphIntersect (vec3 ro, vec3 rd, vec3 ce, float ra)
{
  vec3 oc = ro - ce;
  float b = dot( oc, rd );
  float c = dot( oc, oc ) - ra*ra;
  float h = b*b - c;
  if ( h<0.0 ) return vec2(-1.0);
  h = sqrt( h );

  return vec2( -b-h, -b+h );
}

vec2
boxIntersection (vec3 ro, vec3 rd, vec3 rad, vec3 center, out vec3 oN)
{
  ro -= center;
  vec3 m = 1.0/rd;
  vec3 n = m*ro;
  vec3 k = abs(m)*rad;
  vec3 t1 = -n - k;
  vec3 t2 = -n + k;

  float tN = max( max( t1.x, t1.y ), t1.z );
  float tF = min( min( t2.x, t2.y ), t2.z );

  if( tN>tF || tF<0.0) return vec2(-1.0);

  oN = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);

  return vec2( tN, tF );
}
// glut_990_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

vec3
sampleMinusGradient (vec2 coord)
{
  vec3	veld	= texture(iChannel1, coord / iResolution).xyz;
  float	left	= texture(iChannel0,(coord + vec2(-1, 0)) / iResolution).x;
  float	right	= texture(iChannel0,(coord + vec2( 1, 0)) / iResolution).x;
  float	bottom	= texture(iChannel0,(coord + vec2( 0,-1)) / iResolution).x;
  float	top 	= texture(iChannel0,(coord + vec2( 0, 1)) / iResolution).x;
  vec2	grad 	= vec2(right - left,top - bottom) * 0.5;

  return	vec3(veld.xy - grad, veld.z);
}

vec3
vignette (vec3 color, vec2 q, float v)
{
  color *= 0.99 + 0.01 * pow(16.0 * q.x * q.y * (1.0 - q.x) * (1.0 - q.y), v);

  return color;
}

void
main ()
{
  vec2 velocity = sampleMinusGradient(gl_FragCoord.xy).xy;
  vec3 veld = sampleMinusGradient(gl_FragCoord.xy - dissipation * velocity).xyz;
  float	density = veld.z;
  velocity = veld.xy;

  vec2	uv = (2. * gl_FragCoord.xy - iResolution) / iResolution.y;

  for(int i = 0 ; i < nbSphere ; i++)
  {
    vec2 p = rotate(float(i) * tau / float(nbSphere) + iTime * 0.2, 0.8);
    float dist = distance(uv, p);
    if(dist < ballRadius)
    {
      density += ((ballRadius - dist) / ballRadius) * 0.20;
      density = min(density, 1.);
      velocity = normalize(-p) * 3.;
    }
  }

  gl_FragColor = vec4(vignette(vec3(velocity, density), gl_FragCoord.xy / iResolution, 1.), 1.);
}
