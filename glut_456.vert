#version 300 es
//precision mediump float;

in vec3 aPosition;
//in vec2 aTexCoord;

//out vec4 vPosition;
//out vec2 vTexCoord;

void
main ()
{
  // Pass texture coordinates to the fragment shader
//  vTexCoord = aTexCoord;

  // Transform the position into normalized device coordinates (NDC)
  vec4 positionVec4 = vec4 (aPosition, 1.0);

  // Scale and shift to NDC (Normalize Device Coordinate) range [-1, 1]
  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;

//  vTexCoord = positionVec4.xy;
//  vTexCoord.y *= -1.0;
//  vTexCoord.x = vTexCoord.x * 0.5 + 0.5;
//  vTexCoord.y = vTexCoord.y * 0.5 + 0.5;

  // Set the final position of the vertex
  gl_Position = positionVec4;
//  vPosition = positionVec4;
}
