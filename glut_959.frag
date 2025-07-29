uniform vec2 iResolution;
uniform float iTime;

mat2
rot (float a)
{
  float s = sin(a), c = cos(a);

  return mat2(c, s, -s, c);
}

vec3
path (float z)
{
  vec3 p = vec3(sin(z+cos(z)) * .5, cos(z * .5+sin(z)), z);

  return p;
}

vec3
fractal (vec3 p)
{
  float z = p.z * 2.;
  p.z = abs(.5 - fract(z));
  float m = 100.;
  for (int i = 0; i < 4; i++)
  {
    p = abs(p)/clamp(abs(p.x*p.y*p.z),.2,1.) - 2.;
    m = min(m, min(abs(p.z),min(abs(p.x),abs(p.y))));
  }
  m = exp(-10. * m)*2.;
  vec3 col=vec3(p.xz, 1.) * m;

  return col;
}

float
de (vec3 p)
{
  p.xy -= path(p.z).xy;
  float d = -length(p.xy) + .25 + sin(p.z*5.)*.1;

  return d*.5;
}

vec3
march (vec3 from, vec3 dir)
{
  float d, td = 0.;
  vec3 p, col = vec3(0);

  for (int i = 0; i < 80; i++)
  {
    p = from + dir * td;
    d = de(p);
    if (d < .001)
      break;
    td += d;
  }

  if (d < .1) 
  {
    p -= .001 * dir;
    col = fractal(p) * exp(-.7 * td * td);
  }

  col=normalize(2.+col)*length(col)*.5;
  col.rb*=rot(iTime);
  col.gb*=rot(iTime*.5);
  col=abs(col);

  return col;
}

mat3
lookat (vec3 dir, vec3 up)
{
  dir = normalize(dir);
  vec3 rt = normalize(cross(dir, normalize(up)));

  return mat3(rt, cross(rt, dir), dir);
}

void
main ()
{
  vec3 finalCol = vec3(0.0);
  float samples = 4.0;
  float offset = 1.0 / samples;
  float t = iTime * 0.5;

  for (float x = 0.0; x < samples; x++)
    for (float y = 0.0; y < samples; y++)
    {
      vec2 samplePos = gl_FragCoord.xy + vec2(x * offset, y * offset);
      vec2 uv = (samplePos - iResolution * 0.5) / iResolution.y;
      vec3 from = path(t);
      vec3 fw = normalize(path(t + 0.5) - from);
      vec3 dir = normalize(vec3(uv, 1));
      dir = lookat(fw, vec3(0, 1, 0)) * dir;
      vec3 col = march(from, dir);

      finalCol += col;
    }
  finalCol /= (samples * samples);

  gl_FragColor = vec4(finalCol, 1.0);
}
