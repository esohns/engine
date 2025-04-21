#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

vec4
lookup (sampler2D src, float x, float y)
{
  return texture(src, vec2(x / iResolution.x, y / iResolution.y));
}

float
rayStrength (vec2 raySource, vec2 rayRefDirection, vec2 coord, float seedA, float seedB, float speed)
{
  vec2 sourceToCoord = coord - raySource;
  float cosAngle = dot(normalize(sourceToCoord), rayRefDirection);
  
  return clamp(
    (0.45 + 0.15 * sin(cosAngle * seedA + iTime * speed)) +
    (0.3 + 0.2 * cos(-cosAngle * seedB + iTime * speed)),
    0.0, 1.0) *
    clamp((iResolution.x - length(sourceToCoord)) / iResolution.x, 0.5, 1.0);
}

float
bubbleStrength (vec2 startPos, vec2 waveOffset, float radius, float speed, vec2 coord)
{
  vec2 curPos = vec2(
    mod(startPos.x + waveOffset.x * 0.5, iResolution.x + radius * 2.0) - radius,
    mod(waveOffset.y - iTime * speed, iResolution.y + radius * 2.0) - radius);
  return 1.0 - smoothstep(0.0, radius, length(coord - curPos));
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  uv.y = 1.0 - uv.y;
  vec2 coord = vec2(gl_FragCoord.x, iResolution.y - gl_FragCoord.y);

  float offsetX = (0.1112 * iResolution.x * cos(1.44125 * (iTime + uv.y))) + (26.77311 * iTime);
  float offsetY = (0.08447 * iResolution.y * sin(2.14331 * (iTime + uv.x)));

  vec4 tex1 = lookup(iChannel0, coord.x + offsetX, coord.y + offsetY);
  vec4 tex2 = lookup(iChannel1, coord.x * 2.0 + offsetX * 1.6, coord.y * 2.0 + offsetY * 1.5);
  vec4 tex3 = lookup(iChannel2, coord.x * 3.0 + offsetX * 1.3, coord.y * 3.0 + offsetY * 1.2);
  vec4 tex4 = lookup(iChannel3, coord.x * 0.5 + offsetX * 0.8, coord.y * 0.5 + offsetY * 0.9);

  vec2 rayPos1 = vec2(iResolution.x * 0.7, iResolution.y * -0.4);
  vec2 rayRefDir1 = normalize(vec2(1.0, -0.116));
  float raySeedA1 = 36.2214;
  float raySeedB1 = 21.11349;
  float raySpeed1 = 1.5;

  vec2 rayPos2 = vec2(iResolution.x * 0.8, iResolution.y * -0.6);
  vec2 rayRefDir2 = normalize(vec2(1.0, 0.241));
  float raySeedA2 = 22.39910;
  float raySeedB2 = 18.0234;
  float raySpeed2 = 1.1;

  vec4 rays1 =
    vec4(1.0, 1.0, 1.0, 1.0) *
    rayStrength(rayPos1, rayRefDir1, coord, raySeedA1, raySeedB1, raySpeed1);

  vec4 rays2 =
    vec4(1.0, 1.0, 1.0, 1.0) *
    rayStrength(rayPos2, rayRefDir2, coord, raySeedA2, raySeedB2, raySpeed2);

  float bubbleScale = iResolution.x / 600.0;

  vec4 bubble1 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(0.0, 0.0), vec2(offsetX * 0.2312, 0.0), 20.0 * bubbleScale, 60.0, coord);

  vec4 bubble2 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(40.0, 400.0), vec2(offsetX * -0.06871, offsetY * 0.301), 7.0 * bubbleScale, 25.0, coord);

  vec4 bubble3 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(300.0, 70.0), vec2(offsetX * 0.19832, offsetY * 0.1351), 14.0 * bubbleScale, 45.0, coord);

  vec4 bubble4 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(500.0, 280.0), vec2(offsetX * -0.0993, offsetY * -0.2654), 12.0 * bubbleScale, 32.0, coord);

  vec4 bubble5 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(400.0, 140.0), vec2(offsetX * 0.2231, offsetY * 0.0111), 10.0 * bubbleScale, 28.0, coord);

  vec4 bubble6 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(200.0, 360.0), vec2(offsetX * 0.0693, offsetY * -0.3567), 5.0 * bubbleScale, 12.0, coord);

  vec4 bubble7 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(0.0, 0.0), vec2(offsetX * -0.32301, offsetY * 0.2349), 16.0 * bubbleScale, 51.0, coord);

  vec4 bubble8 =
    vec4(1.0, 1.0, 1.0, 1.0) *
     bubbleStrength(vec2(130.0, 23.0), vec2(offsetX * 0.1393, offsetY * -0.4013), 8.0 * bubbleScale, 24.0, coord);

  gl_FragColor =
    tex1 * 0.45 +
    tex2 * 0.25 +
    tex3 * 0.15 +
    tex4 * 0.1 +
    rays1 * 0.5 +
    rays2 * 0.4 +
    bubble1 * 0.25 +
    bubble2 * 0.1 +
    bubble3 * 0.18 +
    bubble4 * 0.13 +
    bubble5 * 0.15 +
    bubble6 * 0.05 +
    bubble7 * 0.12 +
    bubble8 * 0.11;
  
  float brightness = 1.0 - (coord.y / iResolution.y);
  gl_FragColor.x *= 0.2 + (brightness * 0.8);
  gl_FragColor.y *= 0.3 + (brightness * 0.7);
  gl_FragColor.z *= 0.4 + (brightness * 0.6);
}
