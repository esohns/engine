uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  //vec2 temp = gl_FragCoord.xy / iResolution.xy;
  //temp = temp - vec2(0.5, 0.5);
  //vec2 uv = vec2(atan(temp.y, temp.x)/(2.0*3.14159), 2.0 * sqrt(temp.x * temp.x + temp.y * temp.y)/sqrt(2.0));

  vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.y;

  float a = atan(p.x,p.y);
  float r = length(p);
  vec2 uv = vec2(a/(2.0 * 3.1415926535),r);

  float xCol = (uv.x - (iTime / 8.0)) * 3.0;
  xCol = mod(xCol, 3.0);
  vec3 horColour = vec3(0.25, 0.25, 0.25);

  if (xCol < 1.0)
  {
    horColour.r += 1.0 - xCol;
    horColour.g += xCol;
  }
  else if (xCol < 2.0)
  {
    xCol -= 1.0;
    horColour.g += 1.0 - xCol;
    horColour.b += xCol;
  }
  else
  {
    xCol -= 2.0;
    horColour.b += 1.0 - xCol;
    horColour.r += xCol;
  }

  float backValue = 1.0;
  float aspect = iResolution.x / iResolution.y;
  if (mod(uv.y * 100.0, 1.0) > 0.75 || mod(uv.x * 100.0 * aspect, 1.0) > 0.75)
  {
    backValue = 1.15;	
  }

  vec3 backLines  = vec3(backValue);

  uv = (2.0 * uv) - 1.0;
  float beamWidth = abs(3.0 / (30.0 * uv.y));
  vec3 horBeam = vec3(beamWidth);

  gl_FragColor = vec4(((backLines * horBeam) * horColour), 1.0);
}
