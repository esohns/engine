attribute vec3 aPosition;
attribute vec2 aTexCoord;

//uniform mat4 uModelViewMatrix;
//uniform mat4 uProjectionMatrix;

// Varying values passed to our fragment shader
varying vec2 vTexCoord;
varying vec3 vVertexPos;

void main ()
{
//  vTexCoord = aTexCoord;
  vTexCoord = aPosition.xy;
  vVertexPos = aPosition;

  vec4 pos = vec4 (aPosition, 1.0);

  // Apply the ModelView and Projection matricies
  //gl_Position = uProjectionMatrix * uModelViewMatrix * pos;
  pos.xy = pos.xy * 2.0 - 1.0; 
  gl_Position = pos;
}
