#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define LOOKUP(COORD) texture(iChannel0,(COORD)/iResolution.xy)

vec4
Field (vec2 position)
{
  vec2 velocityGuess = LOOKUP (position).xy;
  vec2 positionGuess = position - velocityGuess;
  return LOOKUP (positionGuess);
}

void
main ()
{
  vec4 Energy =  Field(gl_FragCoord.xy);

  vec4 pX  =  Field(gl_FragCoord.xy + vec2(1,0));
  vec4 pY  =  Field(gl_FragCoord.xy + vec2(0,1));
  vec4 nX  =  Field(gl_FragCoord.xy - vec2(1,0));
  vec4 nY  =  Field(gl_FragCoord.xy - vec2(0,1));

  Energy.b = (pX.b + pY.b + nX.b + nY.b)/4.0;

  vec2 Force;
  Force.x = nX.b - pX.b;
  Force.y = nY.b - pY.b;
  Energy.xy += Force/4.0;

  Energy.b += (nX.x - pX.x + nY.y - pY.y)/4.;

  Energy.y -= Energy.w/300.0;

  Energy.w += (nX.x*nX.w-pX.x*pX.w+nY.y*nY.w-pY.y*pY.w)/4.;

  if(gl_FragCoord.x<1.||gl_FragCoord.y<1.||iResolution.x-gl_FragCoord.x<1.||iResolution.y-gl_FragCoord.y<1.)
    Energy.xy *= 0.;

  if (iMouse.z > 0. && length(gl_FragCoord.xy-iMouse.xy) < 15.)
    Energy.w = 1.;

  gl_FragColor = Energy;
}
