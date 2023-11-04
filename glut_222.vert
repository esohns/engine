#ifdef GL_ES
precision highp float;
#endif

#extension GL_OES_standard_derivatives : enable
#extension GL_ARB_separate_shader_objects : enable

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
attribute vec4 aVertexColor;

varying out vec4 VertexPosition;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat3 uNormalMatrix;

varying out vec3 var_vertPos;
varying out vec4 var_vertCol;
varying out vec3 var_vertNormal;
varying out vec2 var_vertTexCoord;

void main()
{
  mat4 MVP = gl_ProjectionMatrix * gl_ModelViewMatrix;
  gl_Position = MVP * gl_Vertex;

  VertexPosition = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
  var_vertPos      = aPosition;
  var_vertCol      = aVertexColor;
  var_vertNormal   = aNormal;
  var_vertTexCoord = (uModelViewMatrix * gl_Vertex).xy;
}
