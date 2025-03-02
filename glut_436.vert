attribute vec4 aPosition;

varying vec4 v_uv;

void
main ()
{
  // Transform the position into normalized device coordinates (NDC)
  vec4 positionVec4 = aPosition;

  // Scale and shift to NDC (Normalize Device Coordinate) range [-1, 1]
  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;

  v_uv = positionVec4;
  v_uv.y *= -1.0;
  v_uv.x = v_uv.x * 0.5 + 0.5;
  v_uv.y = v_uv.y * 0.5 + 0.5;

  // Set the final position of the vertex
  gl_Position = positionVec4;
}
