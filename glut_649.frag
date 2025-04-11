#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

const float waveStrength = 0.02;
const float frequency = 30.0;
const float waveSpeed = 5.0;
const vec4 sunlightColor = vec4(1.0,0.91,0.75, 1.0);
const float sunlightStrength = 5.0;

void
main ()
{
  vec2 tapPoint = vec2(iMouse.x/iResolution.x,iMouse.y/iResolution.y);
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  float modifiedTime = iTime * waveSpeed;
  float aspectRatio = iResolution.x/iResolution.y;
  vec2 distVec = uv - tapPoint;
  distVec.x *= aspectRatio;
  float distance = length(distVec);
  vec2 newTexCoord = uv;

  float multiplier = (distance < 1.0) ? ((distance-1.0)*(distance-1.0)) : 0.0;
  float addend = (sin(frequency*distance-modifiedTime)+1.0) * waveStrength * multiplier;
  newTexCoord += addend;    

  vec4 colorToAdd = sunlightColor * sunlightStrength * addend;

  gl_FragColor = texture(iChannel0, newTexCoord) + colorToAdd;
}
