#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

vec2
hash22 (vec2 p)
{
  p = mod(p, 5.); 

  float n = sin(dot(p, vec2(41, 289))); 
  return fract(vec2(8, 1)*262144.*n);

  //p = fract(vec2(8, 1)*262144.*n);
  //return sin(p*6.283 + iTime)*0.5 + 0.5;
}

float
voronoi (vec2 p)
{
  vec2 g = floor(p), o; p -= g;

  vec3 d = vec3(1);

  for(int y = -1; y <= 1; y++)
    for(int x = -1; x <= 1; x++)
    {
      o = vec2(x, y);
      o += hash22(g + o) - p;

      d.z = dot(o, o);            
      d.y = max(d.x, min(d.y, d.z));
      d.x = min(d.x, d.z); 
    }

  return d.y - d.x;
}

void
main ()
{
  vec2 p = gl_FragCoord.xy/iResolution.xy;

  if (floor(hash22(p + iTime).x*8.)< 1. || iFrame==0)
  {
    vec3 tx = texture(iChannel1, p).xyz; tx *= tx;
    tx = smoothstep(0., .5, tx);

    p += sin(p*6.2831853*2. - cos(p.yx*6.2831853*4.))*.01;

    float c = voronoi(p*5. - .35)*.6 + voronoi((p.yx + .5)*10.)*.3 + (1.-voronoi(p*25.))*.075 + voronoi(p*60.)*.025;
 
    c += dot(tx, vec3(.299, .587, .114))*.1;

    gl_FragColor.xyz = tx;
    gl_FragColor.w = min(c / 1.1, 1.);
  }
  else
    gl_FragColor = texture(iChannel0, p);
}
