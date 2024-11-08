#version 330
//precision mediump float;

uniform vec2 u_resolution;
uniform float u_time;

#define FC gl_FragCoord.xy
#define R u_resolution
#define T (u_time*2.) // speeds-up global time
#define S smoothstep
// returns a color for a given value (which differs by scene)
#define hue(a) (.25+.6*cos((6.3+1.35*floor(mod(tt, scenes)))*(a)+vec3(0,83,21)))
// returns the luminocity of a given color
#define lum(a) dot(vec3(.21,.71,.07),a)
#define tt (T*.1)
#define scenes 5.
// a value between 0 and scenes, which will change in the frequency of tt
#define proc floor(mod(tt,scenes))

// randomizes the given 2d value
vec2
rnd (vec2 p)
{
  vec3 q=fract(vec3(p.x*12.9898,p.y*78.233,dot(p,p+34.56)));
  q+=dot(q,q+34.56);
  return fract(vec2(q.x*q.y,q.y*q.z));
}

void
main (void)
{
  vec2 uv = (FC-.5*R)/min(R.x, R.y),
  p = uv*1.5;
  uv *= 1.5-proc*.031;
  float k = (.805-.05*proc),
  f = pow(-log(tanh((.2+mod(T, 10.)*.2)))*.5, 5.);
  for (float i = .0; i++ <= 8.;) {
    uv = sqrt(abs(uv)/dot(uv, uv))-k+f;
    // Only if the code absolutely requires an explicit case distinction.
    // By the way, this also gives nice additional patterns. Just give it a try.
    if (dot(uv,uv) < .0025*(.2-proc*.1)) break;
  }
  // calculation of the distance to a circle at UV, which has been distorted in the loop above
  // (the smoothstep function (S) stretches the value's range - change 1.8 to smthng up to 3.8
  // for different colors) and blur the result to smooth the fine lines
  float d = S(1.8+k*.2, -1.,length(uv+mix(vec2(.125),vec2(.35),rnd(p))))-k*2.;
  vec3 col = vec3(0);
  // colorize (multiplication with the fract part gives the steps)
  col += 1.2*hue(.9+d)*max(.075,fract(max(abs(p.y), abs(p.x))*5.));
  // color grading (makes darker parts darker)
  col /= 1.+exp(-col);
  // brightens the center
  col = sqrt(col+lum(col))+S(.25,-.1,length(p));
  // vignette
  p = (FC-.5*R)/R;
  col = mix(vec3(0), col, pow(S(.5,.07, dot(p*p, p*p)), 16.));
  // write the pixel's color
  gl_FragColor = vec4(col, 1);
}
