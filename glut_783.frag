uniform vec2 iResolution;
uniform float iTime;

/*
  v = 67 , r = 29
  v = 139, r = 42
  v = 151, r = 32
  v = 331, r = 31
*/
const float v = 151.0; // v should be a prime of the form 4n-1
const float r = 32.0;

//#define INCREMENT_R
#define BORDERS

#define time iTime

vec4
hexagon (vec2 p)
{
  vec2 q = vec2( p.x*2.0*0.5773503, p.y + p.x*0.5773503 );
  vec2 pi = floor(q), pf = fract(q);
  float v = mod(pi.x + pi.y, 3.0);
  float ca = step(1.0,v), cb = step(2.0,v);
  vec2  ma = step(pf.xy,pf.yx);
  float e = dot( ma, 1.0-pf.yx + ca*(pf.x+pf.y-1.0) + cb*(pf.yx-2.0*pf.xy) );
  p = vec2( q.x + floor(0.5+p.y/1.5), 4.0*p.y/3.0 )*0.5 + 0.5;
  float f = length( (fract(p) - 0.5)*vec2(1.0,0.85) );			
  return vec4( pi + ca - cb*ma, e, f );
}

float
URA (vec2 p)
{
#ifdef INCREMENT_R
  float l = mod(p.y + floor(time*1.5)*p.x, v);
#else
  float l = mod(p.y + r*p.x, v);
#endif
  float rz = 1.;

  for(float i = 1.;i<(v/2.);i++)
    if (mod(i*i,v) == l) rz= .0;

  return rz;
}

void
main () 
{
  vec2 p = gl_FragCoord.xy/iResolution.xy*2.-1.;
  vec2 q = p;
  p.x *= iResolution.x/iResolution.y;

  vec4 h = hexagon(p*21.);

  vec3 col = vec3(URA(h.xy));
#ifdef BORDERS
  if (dot(col,vec3(1)) > 1.)
    col *= smoothstep(-0.2,.13,h.z);
  else
    col = 1.-(1.-col)*smoothstep(-0.2,.13,h.z);
#endif

  col *= smoothstep(1.01,.95,max(abs(q.x),abs(q.y)));

  gl_FragColor = vec4(col, 1.0);
}
