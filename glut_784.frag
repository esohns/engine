uniform vec2 iResolution;
uniform float iTime;

vec3 bgColorDown = vec3(0.2, 0.1, 0.1);
vec3 bgColorUp = vec3(0.1, 0.1, 0.2);

vec3 P1ColorIn = vec3(1.0, 0.5, 0.0);
vec3 P1ColorOut = vec3(1.0, 0.0, 0.0);

vec3 P2ColorIn = vec3(0.0, 0.5, 1.0);  //vec3(1.0, 1.0, 1.0);
vec3 P2ColorOut = vec3(0.0, 0.0, 1.0); //vec3(0.0, 0.5, 1.0);

void
main ()
{
  vec2 p = gl_FragCoord.xy / iResolution.xy;

  vec3 bgCol = mix(bgColorDown, bgColorUp, clamp(p.y * 2., 0.0, 1.0));

  float curve = 0.1 * sin((6.25 * p.x) + (2.0 * iTime));

  float lineAAnim = curve + p.y + 0.02 * sin((50.0 * p.x) + (-5.0 * iTime)) * sin(5.0 * (iTime + 0.2));
  float lineAShape = smoothstep(1.0 - clamp(distance(lineAAnim, 0.5) * 2.0, 0.0, 1.0), 1.0, 0.99);
  vec3  lineACol = (1.0 - lineAShape) * vec3(mix(P1ColorIn, P1ColorOut, lineAShape));

  float ballAShape = smoothstep(1.0 - clamp(distance(p, vec2(0.2, 0.5 - curve)) * 0.5, 0.0, 1.0), 1.0, 0.99);
  vec3 ballACol = (1.0 - ballAShape) * vec3(mix(P1ColorIn, P1ColorOut, ballAShape));

  float lineBAnim = curve + 0. + p.y + 0.02 * sin((50.0 * p.x) + (5.0 * iTime)) * sin(5.0 * iTime);
  float lineBShape = smoothstep(1.0 - clamp(distance(lineBAnim, 0.5) * 2.0, 0.0, 1.0), 1.0, 0.99);
  vec3  lineBCol = (1.0 - lineBShape) * vec3(mix(P2ColorIn, P2ColorOut, lineBShape));

  float ballBShape = smoothstep(1.0 - clamp(distance(p, vec2(0.8, 0.5 - curve)) * 0.5, 0.0, 1.0), 1.0, 0.99);
  vec3 ballBCol = (1.0 - ballBShape) * vec3(mix(P2ColorIn, P2ColorOut, ballBShape));

  vec3 fcolor =   bgCol + lineACol + lineBCol + ballACol + ballBCol;

  gl_FragColor = vec4(fcolor, 1.0);
}
