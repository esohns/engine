uniform vec2 iResolution;
uniform float iTime;

vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  p = fract(vec2(262144, 32768)*n); 
  return sin( p*6.2831853 + iTime )*.35 + .65; 
}

float
smin (float a, float b, float k)
{
  float h = clamp(.5 + .5*(b - a)/k, 0., 1.);
  return mix(b, a, h) - k*h*(1. - h);
}

float
Voronoi (vec2 p)
{
  vec2 g = floor(p), o; p -= g;

  vec3 d = vec3(1);

  float r = 0.;

  for(int y = -1; y <= 1; y++)
    for(int x = -1; x <= 1; x++)
    {
      o = vec2(x, y);
      o += hash22(g + o) - p;

      r = dot(o, o);

      d.z = max(d.x, max(d.y, min(d.z, r)));
      d.y = max(d.x, min(d.y, r));
      d.x = min(d.x, r);
    }

  d = sqrt(d);

  return min(2./(1./max(d.y - d.x, .001) + 1./max(d.z - d.x, .001)), 1.);
}

vec2
hMap (vec2 uv)
{
  float h = Voronoi(uv*6.);

  float c = smoothstep(0., fwidth(h)*2., h - .09)*h;
  c += (1.-smoothstep(0., fwidth(h)*3., h - .22))*c*.5; 

  return vec2(c, h);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.y + vec2(-.1, .025)*iTime;

  vec2 c = hMap(uv);
  vec2 c2 = hMap(uv + .004);

  float b = max(c2.x - c.x, 0.)*16.;

  vec3 col = vec3(1, .05, .25)*c.x;
  float sv = Voronoi(uv*16. + c.y)*.66 + (1.-Voronoi(uv*48. + c.y*2.))*.34;
  col = col*.85 + vec3(1, .7, .5)*sv*sqrt(sv)*.3;
  col += (1. - col)*(1.-smoothstep(0., fwidth(c.y)*3., c.y - .22))*c.x;
  col *= col;

  sv = col.x*Voronoi(uv*6. + .5);
  col += vec3(.7, 1, .3)*pow(sv, 4.)*8.;

  col += vec3(.5, .7, 1)*(b*b*.5 + b*b*b*b*.5);

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1);
}
