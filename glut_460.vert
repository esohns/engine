attribute vec3 aPosition;
attribute vec3 aColor;

// matrices
uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

varying vec4 color;

void
main ()
{
  gl_Position = uProjectionMatrix * uModelViewMatrix * vec4 (aPosition, 1.0);
//  gl_PointSize = 1.0;
  color = vec4 (aColor, 1.0);
}
