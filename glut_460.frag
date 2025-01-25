#ifdef GL_ES
precision mediump float;
#endif

varying vec4 color;

void
main ()
{
  gl_FragColor = color;
}
