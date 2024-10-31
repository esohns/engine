//precision highp float;
#version 330

in vec2 vTexCoord;
in vec2 vCol;

out vec4 fragColor;

void
main ()
{
  vec2 uv = vTexCoord;
  fragColor = vec4 (vCol.rrr, 1.);
}
