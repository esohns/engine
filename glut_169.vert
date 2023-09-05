#version 330

in vec4 position;

void
main ()
{
  vec4 pos4 = position;
  pos4 = 2.0 * pos4 - 1.0;
  gl_Position = pos4;
}
