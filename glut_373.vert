//precision highp float;
#version 330

in vec4 aPosition;
//in vec2 aTexCoord;

out vec2 vTexCoord;

void
main ()
{
  vec4 positionVec4 = aPosition;
  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;
  gl_Position = positionVec4;

  vTexCoord = positionVec4.xy;
}
