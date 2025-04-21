#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform float iTimeDelta;
uniform sampler2D iChannel0;

const vec2 DiffusionRate = vec2(1.0, 0.5);
const float KillRate = 0.062;
const float FeedRate = 0.0545;
const float Speed = 40.0;

vec2
computeLaplacian (vec2 uv, vec2 current)
{
  vec2 pixelSize = vec2(1.) / iResolution.xy;

  return (texture(iChannel0, uv + vec2(pixelSize.x, 0.0)).xy +
          texture(iChannel0, uv - vec2(pixelSize.x, 0.0)).xy +
          texture(iChannel0, uv + vec2(0.0, pixelSize.y)).xy +
          texture(iChannel0, uv - vec2(0.0, pixelSize.y)).xy) * 0.2
          +
          (texture(iChannel0, uv + pixelSize).xy +
          texture(iChannel0, uv - pixelSize).xy +
          texture(iChannel0, uv + vec2(pixelSize.x, -pixelSize.y)).xy +
          texture(iChannel0, uv - vec2(pixelSize.x, -pixelSize.y)).xy) * 0.05
          -
          current;
}

void
main ()
{
#if 0
  if(iTime < 0.5)
  {
    vec2 toMid = (iResolution.xy * 0.5 - gl_FragCoord.xy) / iResolution.y;
    toMid += sin(atan(toMid.x, toMid.y)*20.0) * 0.01;
    float midDistSq = dot(toMid, toMid);
    float initVal = pow(sin(midDistSq * 40.0) * 0.5 + 0.5, 5.0);

    gl_FragColor = vec4(1.0, initVal, 0.0, 1.0);
    return;
  }
#endif

  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec2 current = clamp(texture(iChannel0, uv).xy, vec2(0.), vec2(1.));

  vec2 laplacian = computeLaplacian(uv, current);
  vec2 diffusion = DiffusionRate * laplacian;

  float u = current.x;
  float v = current.y;    
  float reactionU = - u * v * v + FeedRate * (1. - u);
  float reactionV = u * v * v - (FeedRate + KillRate) * v;

  vec2 newValues = current + (diffusion + vec2(reactionU, reactionV)) * Speed * iTimeDelta;

  gl_FragColor = vec4(newValues, 0.0, 1.0);
}
