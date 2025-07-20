uniform vec2 iResolution;
uniform float iTime;

float
rand (float n)
{
  return fract(cos(n)*4145.92653);
}

float
noise (vec2 p)
{
  vec2 f  = smoothstep(0.0, 1.0, fract(p));
  p  = floor(p);
  float n = p.x + p.y*57.0;
  return mix(mix(rand(n+0.0), rand(n+1.0),f.x), mix( rand(n+57.0), rand(n+58.0),f.x),f.y);
}

float
fbm (vec2 p)
{
  mat2 m2 = mat2(1.6,-1.2,1.2,1.6);	
  float f = 0.5000*noise( p ); p = m2*p;
  f += 0.2500*noise( p ); p = m2*p;
  f += 0.1666*noise( p ); p = m2*p;
  f += 0.0834*noise( p );
  return f;
}

vec2
rotate (vec2 p, float a)
{
  vec2 r;
  r.x = p.x*cos(a) - p.y*sin(a);
  r.y = p.x*sin(a) + p.y*cos(a);
  return r;
}

float
dir (vec2 a, vec2 b, vec2 c)
{
  return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

bool
insideQuad (vec2 p, vec2 a, vec2 b, vec2 c, vec2 d)
{
  bool b1 = dir(p, a, b) < 0.0;
  bool b2 = dir(p, b, c) < 0.0;
  bool b3 = dir(p, c, d) < 0.0;
  bool b4 = dir(p, d, a) < 0.0;
  return ((b1 == b2) && (b2 == b3) && (b3 == b4));
}

float
background (vec2 uv)
{
  float i = smoothstep(0.0, 0.5, 0.1-uv.y);
  float z = 0.3 - uv.y;

  uv.x = (0.2*uv.x - 0.1) / z;
  uv.y = (1.9*uv.y - 0.3) / z;

  i *= smoothstep(-0.8, 0.2, uv.x);

  float n = iResolution.x / 24.0;    
  vec2 g = floor(uv * n) / n;

  return 1.0 - 0.8 * i * fbm(g * 80.0);    
}

float
seg (float id, vec2 p, float time)
{
  float t = 0.8*time - 0.18 * id;
  float s = 0.022;
  float r = t + 6.283 * rand(id);
  float z = 1.0+0.3*sin(t*0.6) + 0.3*sin(t*1.9);

  p.x /= z;
  p.y /= z;

  vec2 q = p;
  q.y = abs(q.y + 0.2) - 0.33;

  vec2 pos = vec2(0.5 * sin(t) + 0.4*sin(t*1.6), 
                  0.2 * abs(sin(2.2*t)) - 0.3);

  vec2 scale = vec2(sin(t*3.0), cos(t*3.0));

  vec2 a = rotate(scale*vec2(-s, -s), r) + pos;
  vec2 b = rotate(scale*vec2( s, -s), r) + pos;
  vec2 c = rotate(scale*vec2( s,  s), r) + pos;
  vec2 d = rotate(scale*vec2(-s,  s), r) + pos;

  return insideQuad(q, a, b, c, d) ? 0.1 + 0.6 *step(0.0, p.y+0.2) : 0.0;
}

vec2
worm (vec2 uv, float time)
{
  for(int i=0; i < 15; i++)
  {
    float s = seg(float(i), uv, time);
    if (s > 0.0)
      return vec2(s, step(float(i), s-0.5));
  }

  return vec2(0.0);
}

void
main ()
{
  gl_FragColor = vec4(0.0);
    
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 2; ++j)
    {
      vec2 uv = 2.0 * ((gl_FragCoord.xy + vec2(j, i) * 0.5) / iResolution.xy) - 1.0;
      uv.x *= iResolution.x / iResolution.y;

      vec3 col = vec3(1.0);

      float time = iTime + mod(iTime, 0.01);

      col *= background(uv);
      vec2 w = worm(uv, time);
      col -= w.x;
      col = mix(col, vec3(1.0, 0.0, 0.0), w.y);

      col *= smoothstep(0.48, 0.481, 1.0-abs(uv.y));
      col -= 0.6*smoothstep(0.3,2.0,length(uv));
      col -= vec3(0.01, 0.01, 0.0);
      col = clamp(col, 0.0, 1.0);
      gl_FragColor.xyz += col;
    }
  gl_FragColor /= 4.0;
}
