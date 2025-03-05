uniform vec2 iResolution;
uniform float iTime;

vec3
firePalette (float i)
{
  float T = 1400. + 1300.*i;
  vec3 L = vec3(7.4, 5.6, 4.4);
  L = pow(L,vec3(5)) * (exp(1.43876719683e5/(T*L)) - 1.);
  return 1. - exp(-5e8/L);
}

vec3
hash33 (vec3 p)
{
  float n = sin(dot(p, vec3(7, 157, 113)));    
  return fract(vec3(2097152, 262144, 32768)*n); 
}

float
voronoi (vec3 p)
{
  vec3 b, r, g = floor(p);
  p = fract(p);
  
  float d = 1.; 
     
  for (int j = -1; j <= 1; j++)
    for (int i = -1; i <= 1; i++)
    {
      b = vec3(i, j, -1);
      r = b - p + hash33(g+b);
      d = min(d, dot(r,r));

      b.z = 0.0;
      r = b - p + hash33(g+b);
      d = min(d, dot(r,r));

      b.z = 1.;
      r = b - p + hash33(g+b);
      d = min(d, dot(r,r));
    }

  return d;
}

float
noiseLayers (vec3 p)
{
  vec3 t = vec3(0., 0., p.z + iTime*1.5);

  const int iter = 5;
  float tot = 0., sum = 0., amp = 1.;

  for (int i = 0; i < iter; i++)
  {
    tot += voronoi(p + t) * amp;
    p *= 2.;
    t *= 1.5;
    sum += amp;
    amp *= .5;
  }

  return tot/sum;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5) / iResolution.y;

  uv += vec2(sin(iTime*.5)*.25, cos(iTime*.5)*.125);

  vec3 rd = normalize(vec3(uv.x, uv.y, 3.1415926535898/8.));

  float cs = cos(iTime*.25), si = sin(iTime*.25);
  rd.xy = rd.xy*mat2(cs, -si, si, cs); 

  float c = noiseLayers(rd*2.);

  c = max(c + dot(hash33(rd)*2. - 1., vec3(.015)), 0.);

  c *= sqrt(c)*1.5;
  vec3 col = firePalette(c);
  //col = mix(col, col.zyx*.1+ c*.9, clamp((1.+rd.x+rd.y)*0.45, 0., 1.));
  col = mix(col, col.zyx*.15 + c*.85, min(pow(dot(rd.xy, rd.xy)*1.2, 1.5), 1.));
  col = pow(col, vec3(1.25));

  //c = pow(c*1.33, 1.25);
  //vec3 col =  firePalette(c);

  //c *= c*1.5;
  //vec3 col = vec3(c);

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1);
}
