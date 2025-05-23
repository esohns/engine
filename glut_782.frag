uniform vec2 iResolution;
uniform float iTime;

#define pi 3.14159

float
glow (float x, float str, float dist)
{
  return dist / pow(x, str);
}

float
sinSDF (vec2 st, float A, float offset, float f, float phi)
{
  return abs((st.y - offset) + sin(st.x * f + phi) * A);
}

void
main ()
{
  vec2 st = (gl_FragCoord.xy-0.5*iResolution.xy) / iResolution.y + 0.5;
  float col = 0.0;
  float time = iTime/2.0;
  float str = 0.6;
  float dist = 0.02;
  float nSin = 4.0;

  float timeHalfInv = -time * sign(st.x-0.5);
  float am = cos(st.x*3.0);
  float offset = 0.5+sin(st.x*12.0+time)*am*0.05;
  for(float i = 0.0; i<nSin ; i++)
    col += glow(sinSDF(st, am*0.2, offset, 6.0, timeHalfInv+i*2.0*pi/nSin), str, dist);

  vec3 s = cos( 6.*st.y*vec3(1,2,3) - time*vec3(1,-1,1) ) * 0.5;
  float cut = st.x+ (s.x+s.y+s.z) / 33.0;
  col = abs(smoothstep(-0.01,-0.03,0.5-cut) - clamp(col,0.0,1.0));

  gl_FragColor = vec4(vec3(col),1.0);
}
