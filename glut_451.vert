attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 var_vertTexCoord;

void
main ()
{
  // Pass texture coordinates to the fragment shader
//  var_vertTexCoord = aTexCoord;

  // Transform the position into normalized device coordinates (NDC)
  vec4 positionVec4 = vec4 (aPosition, 1.0);

  // Scale and shift to NDC (Normalize Device Coordinate) range [-1, 1]
  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;

  var_vertTexCoord = positionVec4.xy;
  var_vertTexCoord.y *= -1.0;
  var_vertTexCoord.x = var_vertTexCoord.x * 0.5 + 0.5;
  var_vertTexCoord.y = var_vertTexCoord.y * 0.5 + 0.5;

  // Set the final position of the vertex
  gl_Position = positionVec4;
}
