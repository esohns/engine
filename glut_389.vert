//precision highp float;

attribute vec3 aPosition;
attribute vec2 aTexCoord;

// Pass-through to the fragment shader
varying vec2 var_vertTexCoord;

void
main ()
{
  // Transform the position into normalized device coordinates (NDC)
  vec4 positionVec4 = vec4 (aPosition, 1.0);

  // Pass texture coordinates to the fragment shader
  var_vertTexCoord = positionVec4.xy;

  // Scale and shift to NDC (Normalize Device Coordinate) range [-1, 1]
  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;

  // Set the final position of the vertex
  gl_Position = positionVec4;
}
