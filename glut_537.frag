uniform vec2 iResolution;
uniform float iTime;

vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  p = fract(vec2(2097152, 262144)*n);
  return cos(p*6.283 + iTime)*.5;
  //return abs(fract(p+ iTime*.25)-.5)*2. - .5; // Snooker.
  //return abs(cos(p*6.283 + iTime))*.5; // Bounce.
}

float
smoothVoronoi (vec2 p, float falloff)
{
  vec2 ip = floor(p); p -= ip;

  float d = 1., res = 0.0;

  for(int i = -1; i <= 2; i++)
    for(int j = -1; j <= 2; j++)
    {
      vec2 b = vec2(i, j);

      vec2 v = b - p + hash22(ip + b);

      d = max(dot(v,v), 1e-4);

      res += 1.0/pow( d, falloff );
    }

  return pow( 1./res, .5/falloff );
}

float
func2D (vec2 p)
{
  float d = smoothVoronoi(p*2., 4.)*.66 + smoothVoronoi(p*6., 4.)*.34;
  return sqrt(d);
}

float
smoothFract (float x, float sf)
{
  x = fract(x); return min(x, x*(1.-x)*sf);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy-iResolution.xy*.5) / iResolution.y;

  vec2 e = vec2(0.001, 0); 

  float f = func2D(uv); // Range [0, 1]

  float g = length( vec2(f - func2D(uv-e.xy), f - func2D(uv-e.yx)) )/(e.x);
  g = 1./max(g, 0.001);

  float freq = 12.; 
  float smoothFactor = iResolution.y*0.0125; 

#ifdef GLOSSY
  float c = smoothFract(f*freq, g*iResolution.y/16.); // Range [0, 1]
  //float c = fract(f*freq); // Aliased version, for comparison.
#else
  float c = clamp(cos(f*freq*3.14159*2.)*g*smoothFactor, 0., 1.); // Range [0, 1]
  //float c = clamp(cos(f*freq*3.14159*2.)*2., 0., 1.); // Blurry contours, for comparison.
#endif

  vec3 col = vec3(c);
  vec3 col2 = vec3(c*0.64, c, c*c*0.1);

#ifdef GLOSSY
  col = mix(col, col2, -uv.y + clamp(fract(f*freq*0.5)*2.-1., 0., 1.0));
#else
  col = mix(col, col2, -uv.y + clamp(cos(f*freq*3.14159)*2., 0., 1.0));
#endif

  f = f*freq;

#ifdef GLOSSY
  if(f>8. && f<9.) col *= vec3(1, 0, .1);
#else
  if(f>8.5 && f<9.5) col *= vec3(1, 0, .1);
#endif 

#ifdef GLOSSY
  col += g*g*g*vec3(.3, .5, 1)*.25*.25*.25*.1;
#endif 

  gl_FragColor = vec4( sqrt(clamp(col, 0., 1.)), 1.0 );
}
