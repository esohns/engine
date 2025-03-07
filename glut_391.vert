//precision mediump float;

attribute vec3 aPosition;
varying vec2 vPos;

void
main ()
{
  // Transform the position into normalized device coordinates (NDC)
  vec4 positionVec4 = vec4 (aPosition, 1.0);

  // Scale and shift to NDC (Normalize Device Coordinate) range [-1, 1]
  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;

  vPos = positionVec4.xy;

  // Set the final position of the vertex
  gl_Position = positionVec4;
}
