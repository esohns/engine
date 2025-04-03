uniform vec2 iResolution;
uniform float iTime;

mat2 r2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

vec2
Mobius (vec2 p, vec2 z1, vec2 z2)
{
  z1 = p - z1;
  p -= z2;

  return mat2(z1, z1.y, -z1.x)*p/dot(p, p); 
}

vec2
spiralZoom (vec2 p, vec2 offs, float n, float spiral, float zoom, vec2 phase)
{
  p -= offs;
  float a = atan(p.y, p.x)/6.283 + iTime/32.;
  float d = length(p);

  return mat2(n, 2, spiral,-zoom)*vec2(a, log(d)) + phase;
}

float
noise3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);

  vec3 ip = floor(p);

  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);

  p -= ip;

  p = p*p*(3. - 2.*p);

  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);

  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  uv = r2(3.14159/6.*sin(iTime/3.))*uv*1.2;    

  float r = min(length(uv - vec2(.5, 0)), length(uv - vec2(-.5, 0)));

  uv = Mobius(uv, vec2(-1, -.5), vec2(.5, 0));
  uv = spiralZoom(uv, vec2(-.5), 4., 3.14159*.2, .5, vec2(-1, 1)*iTime*.125);

  const float sc = 4.;
  vec2 p = uv*sc;

  vec2 ip = floor(p);

  p -= ip + .5;

  if(mod(ip.x*ip.y*.5 + (ip.x + ip.y + 1.)*.75, sc/2.)>.5)
    p.xy = p.yx;

  float cx = abs(p.y) - .21;
  float cy = abs(p.x) - .21;

  float pat = clamp(cos((uv.x - uv.y)*6.283*24.)*.35 + .65, 0., 1.)*.8 + .2;
  vec2 uv2 = r2(-3.14159/4.)*uv;
  float pat3 = clamp(cos((uv2.x + uv2.y)*6.283*48.)*.75 + .75, 0., 1.)*.8 + .2;

  float stripeX = clamp(cos(p.y*6.238*6.)*.6 + .57, 0., 1.);
  float stripeY = clamp(cos(p.x*6.238*6.)*.6 + .57, 0., 1.);

  vec3 col = vec3(.25)*(1. - pat3*.9)*pat;

  col = mix(col, vec3(0), (1. -smoothstep(0., .1, cx - .125))*.7);
  col = mix(col, vec3(0), 1. -smoothstep(0., .025, cx - .05));

  col = mix(col, vec3(.6)*(cos(p.y*6.283) + 1.)*stripeX*pat, 1. -smoothstep(0., .025, cx)); 

  col = mix(col, vec3(0), (1. -smoothstep(0., .05, cx + .175))*.65);

  col = mix(col, vec3(0), (1. -smoothstep(0., .1, cy - .125))*.7);
  col = mix(col, vec3(0), 1. -smoothstep(0., .025, cy - .05));
  col = mix(col, vec3(.6)*(cos(p.x*6.283) + 1.)*stripeY*pat, 1. -smoothstep(0., .025, cy));
  col = mix(col, vec3(0), (1. -smoothstep(0., .05, cy + .175))*.65);

  col *= noise3D(vec3(uv*256., r))*.75 + .75;

  vec3 fogCol = vec3(0);
  col = mix(col, fogCol, 1./(1. + r*.25 + r*r*8.));
  col = mix(col, fogCol, smoothstep(0., .99, .03/r));

  col *= vec3(1.1, 1, .95);

  uv = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y), .25)*1.15;

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
