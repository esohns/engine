//precision mediump float;

attribute vec3 aPosition;

void main (void)
{
//  gl_Position = vec4 (aPosition, 1.0);
  gl_Position = vec4 (aPosition * 2.0 - 1.0, 1.0);
}
