uniform vec2 iResolution;
uniform float iTime;

float
WaveletNoise (vec2 p, float z, float k)
{
  float d=0.,s=1.,m=0., a;
  for(float i=0.; i<4.; i++)
  {
    vec2 q = p*s, g=fract(floor(q)*vec2(123.34,233.53));
    g += dot(g, g+23.234);
    a = fract(g.x*g.y)*1e3;// +z*(mod(g.x+g.y, 2.)-1.);
    q = (fract(q)-.5)*mat2(cos(a),-sin(a),sin(a),cos(a));
    d += sin(q.x*10.+z)*smoothstep(.25, .0, dot(q,q))/s;
    p = p*mat2(.54,-.84, .84, .54)+i;
    m += 1./s;
    s *= k;
  }

  return d/m;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy-.5*iResolution)/iResolution.y;

  vec3 col = vec3(0);
  col += WaveletNoise(uv*5., iTime, 1.24)*.5+.5; 
  if(col.r>.99) col= vec3(1,0,0); 
  if(col.r<0.01) col = vec3(0,0,1);

  gl_FragColor = vec4(col,1.0);
}
