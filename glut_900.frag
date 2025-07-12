uniform vec2 iResolution;
uniform float iTime;

vec3
palette (float t)
{
  vec3 a = vec3(0.5,0.5,0.5);
  vec3 b = vec3(0.5,0.5,0.5);
  vec3 c = vec3(2.000,1.000,0.000);
  vec3 d = vec3(0.5,0.2,0.25);

  return a + b*cos( 6.28318*(c*t+d) );
}

float
SDF_Triangle (vec2 p, float r)
{
  float k = sqrt(3.0);
  p.x = abs(p.x) - r;
  p.y = p.y + r/k;
  if( p.x+k*p.y>0.0 ) p = vec2(p.x-k*p.y,-k*p.x-p.y)/2.0;
  p.x -= clamp( p.x, -2.0*r, 0.0 );
  return -length(p)/sign(p.y);
}

float
spiral (vec2 p)
{
  float x = p.x*3.;
  float m = min (fract (x), fract (3. -x)) ;
  return smoothstep (-0.2, .5, m*.5+.2-p.y) ;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy-.5*iResolution) / iResolution.y;

  vec2 st = vec2 (atan(uv.x, uv.y), length (uv)) ;
  uv = vec2 (st.x / 6.2831+.5 - sin(-iTime + st.y), st.y);
  float c = 0.0;

  float triangle = SDF_Triangle((gl_FragCoord.xy-.5*iResolution) / iResolution.y, .3);

  c = spiral(uv) * 2. * spiral(vec2(spiral(uv / 0.6), triangle));

  c += triangle * 0.6;

  vec3 col = palette(length(st.y) + .4 + iTime*.4);
  col *= c;

  uv = (gl_FragCoord.xy-.5*iResolution) / iResolution.y;
  c *= smoothstep(0.0, 0.05, length(uv));

  col *= c;

  st.x=fract(9.*sin(8.*(ceil(st.x*384.)/128.)));

  float t=iTime*2.*((st.x+.5)/2.), b=pow(1.-fract(st.y+st.x+t),4.);

  b=(b*.5)+step(1.-b,.05);
  col += st.y * b*vec3(0.,.5,1.);

  gl_FragColor = vec4 (col, 1.) ;
}
