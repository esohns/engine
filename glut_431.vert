//precision highp float;

attribute vec3 aPosition;
//attribute vec2 aTexCoord;

// Pass-through to the fragment shader
//varying vec2 vTexCoord;

void
main ()
{
  // Pass texture coordinates to the fragment shader
  //vTexCoord = (aPosition.xy + 1.0) * 0.5;

  // Transform the position into normalized device coordinates (NDC)
  // Scale and shift to NDC (Normalize Device Coordinate) range [-1, 1]
  vec4 positionVec4 = vec4 (aPosition * 2.0 - 1.0, 1.0);

  // Set the final position of the vertex
  gl_Position = positionVec4;
}
