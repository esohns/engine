#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

float ten_r = 0.04;

float
GetAngle (float i, float t)
{
//float amp = .75; //.25 is subtle wriggling
//t += amp*sin(1.*t + 4.*iTime + float(i));
  float dir = mod(i,2.) < 0.5 ? 1. : -1.;
  return dir * (1./(.5*i+1.)+1.) * t + i/2.;
}

#define POS_CNT 5
vec2 pos[POS_CNT];

float
Potential (int numNodes, vec2 x)
{
  if( numNodes == 0 ) return 0.;
    
  float res = 0.;
  float k = 16.;
  for( int i = 0; i < POS_CNT; i++ )
  {
    if( i == numNodes ) break;
    res += exp( -k * length( pos[i]-x ) );
  }
  return -log(res) / k;
}

void
ComputePos_Soft (float t)
{
  for( int i = 0; i < POS_CNT; i++ )
  {
    float a = GetAngle( float(i), t );
    vec2 d = vec2(cos(a),sin(a));
    float r = ten_r;
        
    for( int j = 0; j < 3; j++ )
      r += ten_r-Potential(i,r*d);
        
    pos[i] = r * d;
  }
}

vec3
drawSlice (vec2 uv)
{
  float t = iTime/2.;
  ComputePos_Soft(t);
  float pot = Potential(POS_CNT,uv);
  return vec3(smoothstep(0.03,0.01,pot));
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;

  gl_FragColor = textureLod( iChannel0, uv*.992, 0. );

  if( iFrame == 0 ) gl_FragColor = vec4(0.);

  uv.x += .1*sin(.7*iTime);
  uv.y += .05*sin(.3*iTime);
  uv = 2. * uv - 1.;
  uv.x *= iResolution.x/iResolution.y;

  vec3 spots = drawSlice( uv );

  gl_FragColor.rgb = gl_FragColor.rgb * .95 + spots;
}
