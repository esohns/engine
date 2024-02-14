// These are necessary definitions that let you graphics card know how to render the shader
#ifdef GL_ES
precision highp float;
#endif

// attributes, in
attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

// attributes, out
varying vec3 var_vertPos;
varying vec3 var_vertNormal;
varying vec2 var_vertTexCoord;
varying vec4 var_centerGlPosition;

// matrices
uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat3 uNormalMatrix;
uniform float u_time;

void main()
{
/*  vec3 pos = aPosition;
  vec4 posOut = uProjectionMatrix * uModelViewMatrix * vec4(pos, 1.0);
  gl_Position = posOut;

  // set out value
  var_vertPos      = pos;
  var_vertNormal   =  aNormal;
  var_vertTexCoord = aTexCoord;
  var_centerGlPosition = uProjectionMatrix * uModelViewMatrix * vec4(0., 0., 0.,1.0); */

  // copy the position data into a vec4, using 1.0 as the w component
  vec4 positionVec4 = vec4(aPosition, 1.0);

  positionVec4.xy = positionVec4.xy * 2.0 - 1.0;

  // send the vertex information on to the fragment shader
  gl_Position = positionVec4;
}
