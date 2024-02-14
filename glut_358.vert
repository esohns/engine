attribute vec3 aPosition;
varying vec2 uv;

void main ()
{
  gl_Position = vec4 (aPosition * 2.0 - 1.0, 1.0);
  uv = vec2 (aPosition.x, 1.0-aPosition.y);
}
