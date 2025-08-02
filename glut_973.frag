#version 130

uniform vec2 iResolution;
uniform float iTime;

float
map (vec3 p)
{
  p=mod(p,8.)-4.;

  return length(p) - 2.;
}

mat2
rot (float deg)
{
  return mat2(cos(deg),-sin(deg),
              sin(deg), cos(deg));
}

float
trace (vec3 o, vec3 r)
{
  float t = 0.0;
  for(int i = 0; i<32; i++)
  {
    vec3 p = o+r * t;
    float d = map(p);
    t += d*0.5;
  }

  return t;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution;
  uv = uv * 2.0 - 1.0;
  uv.x *= iResolution.x/iResolution.y;
    
  gl_FragColor = vec4(0.0);

  vec3 o = vec3(0.0, 0.0,-9.0);
  o.xz*=rot(iTime*.2);
  o.yz*=rot(iTime*.575);

  vec3 r = normalize(vec3(uv, 1.0 - dot(uv, uv)*.25));
  r.xy*=rot(iTime*.33);
  r.xz*=rot(-iTime*.5);

  float t = trace(o, r);
    
  float fog = 1.0 / (1.0 + t * t * 0.075);
  vec3 fc = vec3(fog);
  float d = map(o+r*t);

  if(abs(d)<.5)
    gl_FragColor = vec4(fc,1.0);
  else
  {
    float lp = length(.5*uv+vec2(sin(iTime*.7)*.3,-cos(iTime*.2)*.3))-.5;
    gl_FragColor = vec4(-lp*2.5)*vec4(.5,.69,1.,1.);
  }
}
