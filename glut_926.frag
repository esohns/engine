uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

//#define IS_CINESHADER

const float pi = 3.1415926535897932384;

float a = 1.0;
float b = 1.0;

float
chladni (float m, float n, vec2 uv)
{	
  // cos()*cos() for modes of a plate fixed at its center
  // sin()*sin() for modes of a plate fixed at its border (boring)
  return cos(n*pi*uv.x/a)*cos(m*pi*uv.y/b);
}

float
omega (float m, float n)
{
  const float rho = 1.0;
  const float eta = 1.0;	
  return pi * sqrt( (rho/eta) * (m/a)*(m/a) + (n/b)*(n/b) );
}

void
main ()
{
  float aspect = iResolution.x / iResolution.y;
  vec2 uv = gl_FragCoord.xy / iResolution.y - vec2(aspect*0.5,0.5);

#ifdef IS_CINESHADER
  vec2 mn = (1.0+0.5*cos(0.25*iTime))*vec2(4.0,2.0); //21.0 * iMouse.xy / iResolution.xy;
#else
  vec2 mn = 7.0 * iMouse.xy / iResolution.xy;
#endif

  float theta = -iTime*0.5;
  mat2 R = mat2( cos(theta), sin(theta), -sin(theta), cos(theta) );
  vec2 c = R * vec2(1.0,-1.0);

  float u = c.x*chladni(mn.x,mn.y,uv) + c.y*chladni(mn.y,mn.x,uv);

  u = (0.5+u/2.0);
  
  float lines = 0.0;
  float thickness = 0.015;
  for(float delta=-0.6; delta < 1.2; delta+=0.2)
    //u += step( abs(v-delta), 0.5*thickness );
    lines += smoothstep(abs(u-delta)-thickness, abs(u-delta)+thickness, thickness);

  float alpha = 0.0;
#ifdef IS_CINESHADER
  float vignette = max(1.0-16.0*length(uv*uv*uv),0.0);
  lines *= vignette;
  u *= vignette;
  alpha = u*u+0.1*sqrt(sqrt(lines));
#endif
    
  gl_FragColor = vec4(lines*vec3(1.0),alpha);
}
