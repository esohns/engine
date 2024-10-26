//precision mediump float;

attribute vec4 aPosition;

void main (void)
{
  gl_Position = aPosition * 2.0 - 1.0, 1.0;
}
