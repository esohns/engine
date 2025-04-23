uniform vec2 iResolution;
uniform float iTime;

const vec4 backgroundColor = vec4(0.,0.,0.4,1.);
const vec4 foregroundColor = vec4(0.4, 0.4, 0.6, 1.);

const vec4 color3 = vec4(0.7, 0.4, 0.4, 1.);
const float radius = 0.06;
const float thickness2 =0.015;
const vec2 center = vec2(0.51);

const vec2 center2 = vec2(0.24,0.16);

vec2 vUV;

const float scale = 0.32;
vec4
circle (vec4 background, 
        vec4 circleColor,
        vec2 position,
        float radius,
        float thickness)
{
  vec2 relative = vUV-position;
  float distance = length(relative);
  distance = smoothstep(distance-thickness/2., distance+thickness/2.,radius);

  vec4 outColor = mix( circleColor,background,distance);
  return outColor;
}

vec4
circleBox (vec4 background, 
           vec4 circleColor,
           vec2 position,
           float radius,
           float thickness,
           vec2 sideLength)
{
  vec2 relative = vUV-position;
  float distance = length(max(vec2(0.), abs(relative)-sideLength));

  distance = smoothstep(distance-thickness/2., distance+thickness/2.,radius);
  vec4 outColor = mix( circleColor,background,distance);
  return outColor;
}

void
main ()
{
  float aspect = iResolution.x/iResolution.y;
  vUV= gl_FragCoord.xy/iResolution.xy;
  float thicknessTheta = iTime*5.;
  thicknessTheta+= vUV.x*10.;
  thicknessTheta+=floor(vUV.y/scale)*4.*scale;

  float thickness = 0.06+0.05*sin(thicknessTheta);
  float amt = (13.+floor(vUV.y/scale)*10.)/14.;
  vUV.x-=iTime/7.*amt;
  vUV = mod(vUV/vec2(scale*1.0, scale*1.),1.);
  vUV.x*=aspect;
  vUV.y = 1.-vUV.y;
  vec2 relative = vUV-center;
  gl_FragColor = mix( circleBox(backgroundColor, foregroundColor, center+vec2(thickness)*0.3, radius,thickness,center2),foregroundColor,thickness*4.);

  gl_FragColor = circleBox(color3,gl_FragColor , center-vec2(thickness), radius,thickness2, center2);
}
